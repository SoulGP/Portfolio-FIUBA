package fiuba.encapsulados.modelo;

import java.util.List;

public class ValidadorMovimiento {
    private static final int ANCHO = 20;
    private static final int ALTO = 20;

    public boolean puedeMoverJugador(Entidad entidad, int nuevoX, int nuevoY, InfoNivel nivel, List<TanqueJugador> jugadores) {
        if (chocaTerreno(nuevoX, nuevoY, nivel)) return false;
        for (Enemigo enemigo : nivel.enemigos()) {
            if (seSuperpone(nuevoX, nuevoY, enemigo.getX(), enemigo.getY())) {
                return false;
            }
        }
        for (TanqueJugador jugador : jugadores) {
            if (jugador.estaParalizado() && jugador == entidad) return false;
            if (jugador != entidad && seSuperpone(nuevoX, nuevoY, jugador.getX(), jugador.getY())) {
                return false;
            }
        }
        return true;
    }

    public boolean puedeMoverEnemigo(Enemigo entidad, int nuevoX, int nuevoY, InfoNivel nivel, List<Enemigo> enemigos, List<TanqueJugador> jugadores) {
        if (chocaTerreno(nuevoX, nuevoY, nivel)) return false;
        for (TanqueJugador jugador : jugadores) {
            if (seSuperpone(nuevoX, nuevoY, jugador.getX(), jugador.getY())) {
                return false;
            }
        }
        for (Enemigo otro : enemigos) {
            if (otro != entidad && seSuperpone(nuevoX, nuevoY, otro.getX(), otro.getY())) {
                return false;
            }
        }
        return true;
    }

    private boolean chocaTerreno(int nuevoX, int nuevoY, InfoNivel nivel) {
        if (nuevoX < 0 || nuevoX >= nivel.width() - ANCHO || nuevoY < 0 || nuevoY >= nivel.height() - ALTO) {
            return true;
        }

        for (Bloque bloque : nivel.bloques()) {
            if (!bloque.esAtravesable() && seSuperpone(nuevoX, nuevoY, bloque.getX(), bloque.getY())) {
                return true;
            }
        }
        return false;
    }

    public boolean seSuperpone(int x1, int y1, int x2, int y2) {
        return x1 < x2 + ANCHO &&
                x1 + ANCHO > x2 &&
                y1 < y2 + ALTO &&
                y1 + ALTO > y2;
    }
}

