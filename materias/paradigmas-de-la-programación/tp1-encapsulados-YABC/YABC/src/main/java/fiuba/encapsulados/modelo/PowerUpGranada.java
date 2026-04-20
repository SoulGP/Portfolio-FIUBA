package fiuba.encapsulados.modelo;

import java.util.ArrayList;
import java.util.List;

public class PowerUpGranada extends PowerUp {
    public PowerUpGranada(int x, int y) {
        super(x, y);
    }

    @Override
    public void aplicar(TanqueJugador jugador, InfoNivel nivel, GameManager gameManager) {
        List<Enemigo> enemigos = new ArrayList<>(nivel.enemigos());
        for (Enemigo enemigo : enemigos) {
            enemigo.takeDamage(enemigo.getHealth());
            gameManager.onEnemyDeath(enemigo);
        }
    }
}