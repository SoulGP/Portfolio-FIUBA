package fiuba.encapsulados.modelo;

import java.util.Timer;
import java.util.TimerTask;

public class PowerUpCasco extends PowerUp {
    private static final int DURACION_MS = 10000;

    public PowerUpCasco(int x, int y) {
        super(x, y);
    }

    @Override
    public void aplicar(TanqueJugador jugador, InfoNivel nivel, GameManager gameManager) {
        jugador.setInvulnerable(true);

        new Timer(true).schedule(new TimerTask() {
            @Override
            public void run() {
                jugador.setInvulnerable(false);
            }
        }, DURACION_MS);
    }
}