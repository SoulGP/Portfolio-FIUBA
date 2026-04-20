package fiuba.encapsulados.modelo;

public class ValidadorTanques {
    public boolean hayTanqueEn(int x, int y, InfoNivel nivel) {
        ValidadorMovimiento validador = new ValidadorMovimiento();
        for (TanqueJugador jugador : nivel.jugadores()) {
            if (validador.seSuperpone(x, y, jugador.getX(), jugador.getY())) {
                return true;
            }
        }
        for (Enemigo enemigo : nivel.enemigos()) {
            if (validador.seSuperpone(x, y, enemigo.getX(), enemigo.getY())) {
                return true;
            }
        }
        return false;
    }
}
