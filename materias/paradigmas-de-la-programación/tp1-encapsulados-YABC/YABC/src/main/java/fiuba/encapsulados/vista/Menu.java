package fiuba.encapsulados.vista;

import javafx.application.Platform;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

import java.util.Objects;

public class Menu {
    private static final double WIDTH = 800;
    private static final double HEIGHT = 600;

    private final Stage stage;
    private final Runnable onSinglePlayer;
    private final Runnable onTwoPlayers;
    private final Image logoImage;

    private final Boton boton = new Boton();

    public Menu(Stage stage, Runnable onSinglePlayer, Runnable onTwoPlayers) {
        this.stage = stage;
        this.onSinglePlayer = onSinglePlayer;
        this.onTwoPlayers = onTwoPlayers;
        this.logoImage = loadLogoImage();
    }

    private Image loadLogoImage() {
        return new Image(Objects.requireNonNull(getClass().getResourceAsStream("/sprites/logo.png"),
                "No se encontro el recurso /sprites/logo.png"));
    }

    public void mostrar() {
        stage.setTitle("Battle City - TP FIUBA");
        stage.getIcons().setAll(logoImage);

        VBox root = new VBox(24);
        root.setAlignment(Pos.CENTER);
        root.setStyle("-fx-background-color: linear-gradient(#0a0a0a 0%, #334819 60%, #355f14 100%);");

        ImageView logoView = new ImageView(logoImage);
        logoView.setPreserveRatio(true);
        logoView.setFitWidth(360);

        Button singlePlayerButton = boton.crearBoton("Iniciar 1 Jugador", onSinglePlayer);
        Button twoPlayersButton = boton.crearBoton("Iniciar 2 Jugadores", onTwoPlayers);
        Button exitButton = boton.crearBoton("Salir", () -> {
            stage.close();
            Platform.exit();
        });

        root.getChildren().addAll(logoView, singlePlayerButton, twoPlayersButton, exitButton);

        Scene scene = new Scene(root, WIDTH, HEIGHT);
        stage.setScene(scene);
        stage.setResizable(false);
        stage.sizeToScene();
        stage.show();
        Platform.runLater(() -> {
            double windowWidth = stage.getWidth();
            double windowHeight = stage.getHeight();
            stage.setMinWidth(windowWidth);
            stage.setMaxWidth(windowWidth);
            stage.setMinHeight(windowHeight);
            stage.setMaxHeight(windowHeight);
        });
    }
}
