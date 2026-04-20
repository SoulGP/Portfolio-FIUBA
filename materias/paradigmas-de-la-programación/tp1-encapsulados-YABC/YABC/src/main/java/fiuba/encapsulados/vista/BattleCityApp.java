package fiuba.encapsulados.vista;


import fiuba.encapsulados.modelo.*;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.scene.Scene;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


public class BattleCityApp extends Application {

    private final List<Disparo> disparos = new ArrayList<>();
    private final Map<TanqueJugador, Movimiento> orientaciones = new HashMap<>();
    private final SoundEffects soundEffects = new SoundEffects();
    private GestorColisiones gestorColisiones;
    private Stage primaryStage;
    private InfoNivel nivel;
    private GameManager gameManager;
    private NivelRenderer renderer;
    private InputManager inputManager;
    private Gameloop gameLoop;

    public static void main(String[] args) {
        Application.launch();
    }

    @Override

    public void start(Stage stage) {
        soundEffects.playMenuMusicLoop();
        this.primaryStage = stage;

        Menu menu = new Menu(stage, () -> iniciarJuego(1), () -> iniciarJuego(2));

        menu.mostrar();
    }

    private void iniciarJuego(int cantidadJugadores) {
        if (gameLoop != null) {
            gameLoop.stop();
        }

        disparos.clear();
        orientaciones.clear();

        final double width = 800;
        final double height = 600;

        Pane root = new Pane();

        root.setPrefSize(width, height);

        try {
            CargadorNiveles cargador = new CargadorNiveles();
            InfoNivel nivelBase = cargador.cargarNivel("/levels/GeneratedLevels/Level0.xml");
            nivel = ajustarJugadores(nivelBase, cantidadJugadores);
        } catch (Exception e) {
            throw new IllegalStateException("No se pudo iniciar el juego", e);
        }

        gestorColisiones = new GestorColisiones(soundEffects);
        gameManager = new GameManager(nivel, soundEffects);
        renderer = new NivelRenderer(root, orientaciones);
        inputManager = new InputManager(disparos, nivel, orientaciones, nivel.controlesPorJugador());

        Scene scene = new Scene(root, width, height);

        inputManager.configurarInput(scene);


        primaryStage.setTitle("Battle City - TP FIUBA");

        primaryStage.setScene(scene);
        primaryStage.setResizable(false);
        primaryStage.sizeToScene();
        primaryStage.show();
        Platform.runLater(() -> {
            double windowWidth = primaryStage.getWidth();
            double windowHeight = primaryStage.getHeight();
            primaryStage.setMinWidth(windowWidth);
            primaryStage.setMaxWidth(windowWidth);
            primaryStage.setMinHeight(windowHeight);
            primaryStage.setMaxHeight(windowHeight);
        });

        renderer.render(nivel, disparos);

        gameLoop = new Gameloop(this::actualizar);

        gameLoop.start();
    }

    private InfoNivel ajustarJugadores(InfoNivel base, int cantidadJugadores) {
        List<TanqueJugador> jugadores = base.jugadores();
        if (jugadores.isEmpty()) {
            return base;
        }

        int limite = Math.max(1, Math.min(cantidadJugadores, jugadores.size()));
        List<TanqueJugador> seleccion = new ArrayList<>(jugadores.subList(0, limite));
        Map<TanqueJugador, ControlesJugador> controlesSeleccionados = new HashMap<>();
        for (TanqueJugador jugador : seleccion) {
            ControlesJugador controles = base.controlesPorJugador().get(jugador);
            if (controles != null) {
                controlesSeleccionados.put(jugador, controles);
            }
        }
        return new InfoNivel(base.width(), base.height(), base.cols(), base.rows(), seleccion, controlesSeleccionados, base.enemigos(), base.bloques(), new ArrayList<>(base.powerUps()));
    }

    private void actualizar() {
        new ActualizadorJugadores(
                nivel.jugadores(),
                nivel.controlesPorJugador(),
                orientaciones,
                new ValidadorMovimiento(),
                inputManager.getEntradasPorJugador(),
                nivel,
                gameManager
        ).actualizar(soundEffects);
        gameManager.actualizarSpawns();
        new ActualizadorEnemigos(nivel.enemigos(), nivel.jugadores(), disparos, nivel, 1.0 / 60).actualizar(soundEffects);
        new ActualizadorDisparos(disparos, gameManager, gestorColisiones).actualizar(soundEffects);

        renderer.render(nivel, disparos);

        if (gameManager.debeCargarSiguienteNivel()) {
            cargarSiguienteNivel();
            gameManager.confirmarCambioNivel();
        }

        if (gameManager.isJuegoTerminado()) {
            gameLoop.stop();
            soundEffects.detenerMusica();
            mostrarPantallaResultado();
        }
    }

    private void mostrarPantallaResultado() {
        boolean victoria = gameManager.isVictoria();
        PantallaResultado pantallaResultado = new PantallaResultado(
                primaryStage,
                victoria,
                this::volverAlMenu
        );
        pantallaResultado.mostrar();
    }

    private void volverAlMenu() {
        soundEffects.playMenuMusicLoop();
        Menu menu = new Menu(primaryStage, () -> iniciarJuego(1), () -> iniciarJuego(2));
        menu.mostrar();
    }

    private void cargarSiguienteNivel() {
        disparos.clear();
        orientaciones.clear();

        try {
            CargadorNiveles cargador = new CargadorNiveles();
            String rutaNivel = "/levels/GeneratedLevels/Level" + gameManager.getNivelActual() + ".xml";
            InfoNivel nivelBase = cargador.cargarNivel(rutaNivel);

            int cantidadJugadores = nivel.jugadores().size();
            nivel = ajustarJugadores(nivelBase, cantidadJugadores);

            gameManager.setNivel(nivel);
            inputManager = new InputManager(disparos, nivel, orientaciones, nivel.controlesPorJugador());

            Scene scene = primaryStage.getScene();
            inputManager.configurarInput(scene);

        } catch (Exception e) {
            System.err.println("Error al cargar el siguiente nivel: " + e.getMessage());
        }
    }

}
