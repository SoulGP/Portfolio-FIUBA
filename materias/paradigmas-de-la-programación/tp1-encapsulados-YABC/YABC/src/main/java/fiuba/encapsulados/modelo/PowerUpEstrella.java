package fiuba.encapsulados.modelo;

public class PowerUpEstrella extends PowerUp {
    public PowerUpEstrella(int x, int y) {
        super(x, y);
    }

    @Override
    public void aplicar(TanqueJugador jugador, InfoNivel nivel, GameManager gameManager) {
        jugador.setDisparoPotente(true);
    }
}