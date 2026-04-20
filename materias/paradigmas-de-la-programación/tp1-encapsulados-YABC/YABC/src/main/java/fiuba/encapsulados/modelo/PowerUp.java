package fiuba.encapsulados.modelo;

public abstract class PowerUp extends Posicion {

    public PowerUp(int x, int y) {
        super(x, y);
    }

    public abstract void aplicar(TanqueJugador jugador, InfoNivel nivel, GameManager gameManager);
}