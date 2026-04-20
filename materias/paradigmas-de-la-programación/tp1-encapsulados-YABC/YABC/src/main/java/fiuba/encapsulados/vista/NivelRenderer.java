package fiuba.encapsulados.vista;

import fiuba.encapsulados.modelo.*;
import javafx.scene.Group;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.*;

import java.util.List;
import java.util.Map;
import java.util.Objects;

public class NivelRenderer {

    private final Pane root;
    private final SpriteManager spriteManager;
    private final Map<TanqueJugador, Movimiento> orientaciones;

    public NivelRenderer(Pane root, Map<TanqueJugador, Movimiento> orientaciones) {
        this.root = root;
        this.spriteManager = new SpriteManager();
        this.orientaciones = orientaciones;

        Image backgroundImage = new Image(Objects.requireNonNull(
                getClass().getResourceAsStream("/sprites/fondo.jpeg")
        ));
        BackgroundSize backgroundSize = new BackgroundSize(
                BackgroundSize.AUTO,
                BackgroundSize.AUTO,
                false,
                false,
                true,
                true
        );
        BackgroundImage background = new BackgroundImage(
                backgroundImage,
                BackgroundRepeat.NO_REPEAT,
                BackgroundRepeat.NO_REPEAT,
                BackgroundPosition.CENTER,
                backgroundSize
        );
        root.setBackground(new Background(background));
    }

    public void render(InfoNivel nivel, List<Disparo> disparos) {
        root.getChildren().clear();


        // Dibujar power-ups
        for (PowerUp powerUp : nivel.powerUps()) {
            ImageView iv = spriteManager.getSprite(powerUp);
            iv.setX(powerUp.getX());
            iv.setY(powerUp.getY());
            root.getChildren().add(iv);
        }

        // Dibujar jugadores
        for (TanqueJugador jugador : nivel.jugadores()) {
            Group playerGroup = new Group();
            ImageView tanqueView = spriteManager.getSprite(jugador);
            Movimiento orientacionJugador = orientaciones != null ? orientaciones.getOrDefault(jugador, Movimiento.ARRIBA) : Movimiento.ARRIBA;
            tanqueView.setRotate(anguloPara(orientacionJugador));
            playerGroup.getChildren().add(tanqueView);

            if (jugador.isInvulnerable()) {
                ImageView ringView = cargarCasco(tanqueView);

                playerGroup.getChildren().add(ringView);
                ringView.toFront();
            }

            playerGroup.setLayoutX(jugador.getX());
            playerGroup.setLayoutY(jugador.getY());
            root.getChildren().add(playerGroup);
        }

        // Dibujar enemigos
        for (Enemigo e : nivel.enemigos()) {
            ImageView iv = spriteManager.getSprite(e);
            Movimiento orientacionEnemigo = e.getDireccion();
            if (orientacionEnemigo != null) {
                iv.setRotate(anguloPara(orientacionEnemigo));
            }
            iv.setX(e.getX());
            iv.setY(e.getY());
            root.getChildren().add(iv);
        }

        // Dibujar disparos
        for (Disparo disparo : disparos) {
            if (!disparo.estaActivo()) {
                continue;
            }
            ImageView iv = spriteManager.getSprite(disparo);
            iv.setX(disparo.getX());
            iv.setY(disparo.getY());
            root.getChildren().add(iv);
        }

        // Dibujar bloques
        for (Bloque b : nivel.bloques()) {
            ImageView iv = spriteManager.getSprite(b);
            iv.setX(b.getX());
            iv.setY(b.getY());
            root.getChildren().add(iv);
        }
    }

    private ImageView cargarCasco(ImageView tanqueView) {
        ImageView ringView = spriteManager.getInvulnerableRing();
        ringView.setMouseTransparent(true);

        // Escalar el anillo  tamaño mas grande que el tanque y centrarlo
        ringView.setFitWidth(tanqueView.getImage().getWidth() * 1.5);
        ringView.setFitHeight(tanqueView.getImage().getHeight() * 1.5);

        double offsetX = -(ringView.getFitWidth() - tanqueView.getImage().getWidth()) / 2.0;
        double offsetY = -(ringView.getFitHeight() - tanqueView.getImage().getHeight()) / 2.0;
        ringView.setTranslateX(offsetX);
        ringView.setTranslateY(offsetY);
        return ringView;
    }

    private double anguloPara(Movimiento movimiento) {
        return movimiento != null ? movimiento.getAngulo() : 0;
    }

}
