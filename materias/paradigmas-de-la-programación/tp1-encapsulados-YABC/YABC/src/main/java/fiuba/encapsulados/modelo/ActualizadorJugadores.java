package fiuba.encapsulados.modelo;

import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

public record ActualizadorJugadores(List<TanqueJugador> jugadores,
                                    Map<TanqueJugador, ControlesJugador> controlesPorJugador,
                                    Map<TanqueJugador, Movimiento> orientaciones, ValidadorMovimiento validador,
                                    Map<TanqueJugador, Set<Entrada>> entradasPorJugador,
                                    InfoNivel nivel, GameManager gameManager) implements Actualizador {

    @Override
    public void actualizar(ServicioSonido sonidos) {
        for (int i = 0; i < jugadores.size(); i++) {
            TanqueJugador jugador = jugadores.get(i);
            if (jugador.realizoDisparo()) {
                sonidos.play(Event.DISPARO);
                jugador.resetearDisparo();
            }

            Movimiento movimiento = obtenerMovimiento(jugador);

            if (movimiento != null) {
                orientaciones.put(jugador, movimiento);
                int nuevoX = jugador.getX() + movimiento.getDx() * jugador.getSpeed();
                int nuevoY = jugador.getY() + movimiento.getDy() * jugador.getSpeed();
                if (validador.puedeMoverJugador(jugador, nuevoX, nuevoY, nivel, jugadores)) {
                    jugador.setPosicion(nuevoX, nuevoY);
                }
            }
            verificarPowerUps(jugador);
        }
    }

    private void verificarPowerUps(TanqueJugador jugador) {
        if (nivel.powerUps().isEmpty()) {
            return;
        }

        Iterator<PowerUp> iterator = nivel.powerUps().iterator();
        while (iterator.hasNext()) {
            PowerUp powerUp = iterator.next();
            if (validador.seSuperpone(jugador.getX(), jugador.getY(), powerUp.getX(), powerUp.getY())) {
                iterator.remove();
                powerUp.aplicar(jugador, nivel, gameManager);
                break;
            }

        }

    }

    private Movimiento obtenerMovimiento(TanqueJugador jugador) {
        Set<Entrada> entradas = entradasPorJugador.get(jugador);
        if (entradas == null || entradas.isEmpty()) return null;
        if (entradas.contains(Entrada.ARRIBA)) return Movimiento.ARRIBA;
        if (entradas.contains(Entrada.ABAJO)) return Movimiento.ABAJO;
        if (entradas.contains(Entrada.IZQUIERDA)) return Movimiento.IZQUIERDA;
        if (entradas.contains(Entrada.DERECHA)) return Movimiento.DERECHA;
        return null;
    }
}

