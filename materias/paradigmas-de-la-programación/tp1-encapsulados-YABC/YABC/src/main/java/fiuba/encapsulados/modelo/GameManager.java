package fiuba.encapsulados.modelo;

import java.util.concurrent.ThreadLocalRandom;

public class GameManager {
    private static final double PROBABILIDAD_POWER_UP = 0.2;
    private static final int POWER_UP_SIZE = 20;
    private static final int MAX_INTENTOS_POWER_UP = 50;
    private static final int NIVELES_TOTALES = 3;

    private final ServicioSonido sonidos;
    private final IdentificadorTipoPowerUp identificador = new IdentificadorTipoPowerUp();
    private final EnemySpawner enemySpawner;
    private InfoNivel nivel;
    private int nivelActual = 0;
    private boolean juegoTerminado = false;
    private boolean victoria = false;
    private boolean cambiarNivel = false;

    public GameManager(InfoNivel nivel, ServicioSonido sonidos) {
        this.nivel = nivel;
        this.enemySpawner = new EnemySpawner(nivel);
        this.sonidos = sonidos;
    }

    public void onEnemyDeath(Enemigo enemigo) {
        nivel.enemigos().remove(enemigo);
        nivel.bloques().add(new BloqueTanque(enemigo.getX(), enemigo.getY()));
        sonidos.play(Event.MUERTE);
        intentarGenerarPowerUp();
        if (nivel.enemigos().isEmpty()) {
            avanzarNivel();
        }
    }

    public void onBloqueDestruido(Bloque bloque) {
        nivel.bloques().remove(bloque);
    }

    public void onBaseDestruida() {
        sonidos.play(Event.DESTRUCCION_BASE);
        juegoTerminado = true;
        victoria = false;
    }

    public void onJugadorMuere(TanqueJugador jugador) {
        if (jugador.getHealth() <= 0) {
            nivel.jugadores().remove(jugador);
            nivel.bloques().add(new BloqueTanque(jugador.getX(), jugador.getY()));
            sonidos.play(Event.MUERTE);
            boolean algunVivo = nivel.jugadores().stream().anyMatch(j -> j.getHealth() > 0);
            if (!algunVivo) {
                juegoTerminado = true;
                victoria = false;
            }
        }
    }

    private void intentarGenerarPowerUp() {
        if (!nivel.powerUps().isEmpty()) {
            return;
        }
        if (ThreadLocalRandom.current().nextDouble() >= PROBABILIDAD_POWER_UP) {
            return;
        }
        PowerUp powerUp = crearPowerUpAleatorio();
        if (powerUp != null) {
            nivel.powerUps().add(powerUp);
        }
    }

    private PowerUp crearPowerUpAleatorio() {
        int maxX = Math.max(0, nivel.width() - POWER_UP_SIZE);
        int maxY = Math.max(0, nivel.height() - POWER_UP_SIZE);
        for (int intento = 0; intento < MAX_INTENTOS_POWER_UP; intento++) {
            int x = ThreadLocalRandom.current().nextInt(0, maxX + 1);
            int y = ThreadLocalRandom.current().nextInt(0, maxY + 1);
            if (ocupaBloque(x, y)) {
                continue;
            }
            return identificador.crearAleatorio(x, y);
        }
        return null;
    }

    private boolean ocupaBloque(int x, int y) {
        ValidadorMovimiento validador = new ValidadorMovimiento();
        for (Bloque bloque : nivel.bloques()) {
            if (validador.seSuperpone(x, y, bloque.getX(), bloque.getY())) {
                return true;
            }
        }
        return false;
    }

    private void avanzarNivel() {
        if (nivelActual < NIVELES_TOTALES - 1) {
            nivelActual++;
            cambiarNivel = true;
        } else {
            juegoTerminado = true;
            victoria = true;
        }
    }

    public void actualizarSpawns() {
        if (!juegoTerminado) {
            enemySpawner.update();
        }
    }

    public boolean isJuegoTerminado() {
        return juegoTerminado;
    }

    public boolean isVictoria() {
        return victoria;
    }

    public int getNivelActual() {
        return nivelActual;
    }

    public InfoNivel getNivel() {
        return nivel;
    }

    public void setNivel(InfoNivel nivel) {
        this.nivel = nivel;
    }

    public boolean debeCargarSiguienteNivel() {
        return cambiarNivel;
    }

    public void confirmarCambioNivel() {
        cambiarNivel = false;
    }
}

