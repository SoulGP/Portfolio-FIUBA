package fiuba.encapsulados.modelo;

import java.util.List;

public record ActualizadorEnemigos(List<Enemigo> enemigos, List<TanqueJugador> jugadores, List<Disparo> disparos,
                                   InfoNivel nivel, double deltaSegundos) implements Actualizador {
    public ActualizadorEnemigos(List<Enemigo> enemigos, List<TanqueJugador> jugadores, List<Disparo> disparos, InfoNivel nivel, double deltaSegundos) {
        this.enemigos = enemigos;
        this.jugadores = jugadores;
        this.disparos = disparos;
        this.nivel = nivel;
        this.deltaSegundos = deltaSegundos;
        configurarRestriccionesMovimientoEnemigos();
    }

    private void configurarRestriccionesMovimientoEnemigos() {
        ValidadorMovimiento validador = new ValidadorMovimiento();
        for (Enemigo enemigo : enemigos) {
            enemigo.setValidadorMovimiento(
                    (nuevoX, nuevoY) -> validador.puedeMoverEnemigo(enemigo, nuevoX, nuevoY, nivel, enemigos, jugadores)
            );
        }
    }

    @Override
    public void actualizar(ServicioSonido sonidos) {
        for (Enemigo enemigo : enemigos) {
            if (enemigo.realizoDisparo()) {
                sonidos.play(Event.DISPARO);
                enemigo.resetearDisparo();
            }
            enemigo.update((float) deltaSegundos, disparos);
        }
    }
}


