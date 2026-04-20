package fiuba.encapsulados.vista;

import fiuba.encapsulados.modelo.*;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;

import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public class SpriteManager {

    private static final String PLAYER1_ID = "player1";
    private static final String PLAYER2_ID = "player2";

    private final Map<Class<?>, Image> staticSprites = new HashMap<>();
    private final LogicaAnimaciones logicaAnimaciones = new LogicaAnimaciones();

    private final Image invulnerableRing;

    public SpriteManager() {
        precargarSprite(BloqueLadrillo.class, "sprites/BrickBlock20x20.png");
        precargarSprite(BloqueAcero.class, "sprites/SteelBlock20x20.png");
        precargarSprite(BloqueAgua.class, "sprites/Water20x20.png");
        precargarSprite(BloqueBosque.class, "sprites/Forest20x20.png");
        precargarSprite(BloqueBase.class, "sprites/base20x20.png");
        precargarSprite(BloqueTanque.class, "sprites/TankDestroyed_20x20.png");

        registrarAnimacionJugador(PLAYER1_ID, "sprites/Player1Tank0_20x20.png", "sprites/Player1Tank1_20x20.png");
        registrarAnimacionJugador(PLAYER2_ID, "sprites/Player2Tank0_20x20.png", "sprites/Player2Tank1_20x20.png");
        registrarAnimacion(TanqueJugador.class, "sprites/Player1Tank0_20x20.png", "sprites/Player1Tank1_20x20.png");

        registrarAnimacion(EnemigoBasico.class, "sprites/EnemyTankRegular0_20x20.png", "sprites/EnemyTankRegular1_20x20.png");
        registrarAnimacion(EnemigoRapido.class, "sprites/EnemyTankFast0_20x20.png", "sprites/EnemyTankFast1_20x20.png");
        registrarAnimacion(EnemigoBlindado.class, "sprites/EnemyTankHeavy0_20x20.png", "sprites/EnemyTankHeavy1_20x20.png");
        registrarAnimacion(EnemigoPotente.class, "sprites/EnemyTankPowerful0_20x20.png", "sprites/EnemyTankPowerful1_20x20.png");
        registrarAnimacion(Enemigo.class, "sprites/EnemyTankRegular0_20x20.png", "sprites/EnemyTankRegular1_20x20.png");

        precargarSprite(Disparo.class, "sprites/Shot.png");

        precargarSprite(PowerUpGranada.class, "sprites/PowerUp-Grenade20x20.png");
        precargarSprite(PowerUpCasco.class, "sprites/PowerUp-Helmet20x20.png");
        precargarSprite(PowerUpEstrella.class, "sprites/PowerUp-Star20x20.png");
        precargarSprite(PowerUpPala.class, "sprites/PowerUp-Shovel20x20.png");

        invulnerableRing = load("sprites/InvulnerableRing20x20.png");

        precargarSprite(Object.class, "sprites/WhiteBlock20x20.png");
    }

    private void precargarSprite(Class<?> clazz, String path) {
        Image img = load(path);
        staticSprites.put(clazz, img);
    }

    private void registrarAnimacion(Class<?> clazz, String... paths) {
        logicaAnimaciones.registrarAnimacion(clazz, loadFrames(paths));
    }

    private void registrarAnimacionJugador(String jugadorId, String... paths) {
        logicaAnimaciones.registrarAnimacionJugador(jugadorId, loadFrames(paths));
    }

    private Image[] loadFrames(String... paths) {
        Image[] frames = new Image[paths.length];
        for (int i = 0; i < paths.length; i++) {
            frames[i] = load(paths[i]);
        }
        return frames;
    }

    private Image load(String path) {
        InputStream is = getClass().getClassLoader().getResourceAsStream(path);
        if (is == null) {
            System.err.println("Sprite no encontrado: " + path);
            return new Image(Objects.requireNonNull(
                    getClass().getResourceAsStream("/sprites/WhiteBlock20x20.png")
            ));
        }
        return new Image(is);
    }

    public ImageView getSprite(Posicion entidad) {
        Image img = obtenerImagen(entidad);
        return new ImageView(img);
    }

    public ImageView getInvulnerableRing() {
        return new ImageView(invulnerableRing);
    }

    private Image obtenerImagen(Posicion entidad) {
        Image animada = logicaAnimaciones.obtenerImagen(entidad);
        if (animada != null) {
            return animada;
        }

        Image img = staticSprites.get(entidad.getClass());
        if (img != null) {
            return img;
        }
        return staticSprites.get(Object.class);
    }
}


