package fiuba.encapsulados.vista;

import javafx.animation.AnimationTimer;

public class Gameloop {
    private final Runnable onUpdate;
    private AnimationTimer timer;
    private long ultimaActualizacion = -1;

    public Gameloop(Runnable onUpdate) {
        this.onUpdate = onUpdate;
    }

    public void start() {
        timer = new AnimationTimer() {
            @Override
            public void handle(long now) {
                if (ultimaActualizacion < 0) {
                    ultimaActualizacion = now;
                    return;
                }
                ultimaActualizacion = now;
                onUpdate.run();
            }
        };
        timer.start();
    }

    public void stop() {
        if (timer != null) {
            timer.stop();
        }
    }
}

