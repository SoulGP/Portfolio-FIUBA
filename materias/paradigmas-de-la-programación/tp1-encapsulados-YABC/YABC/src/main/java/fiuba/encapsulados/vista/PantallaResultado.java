package fiuba.encapsulados.vista;

import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

public record PantallaResultado(Stage stage, boolean victoria, Runnable volverAlMenu) {
    private static final double WIDTH = 800;
    private static final double HEIGHT = 600;

    public void mostrar() {
        VBox root = new VBox(30);
        root.setAlignment(Pos.CENTER);
        Boton boton = new Boton();

        if (victoria) {
            root.setStyle("-fx-background-color: linear-gradient(#0a0a0a 0%, #1a4d1a 60%, #2d7a2d 100%);");
        } else {
            root.setStyle("-fx-background-color: linear-gradient(#0a0a0a 0%, #4d1a1a 60%, #7a2d2d 100%);");
        }

        Label tituloLabel = new Label(victoria ? "¡VICTORIA!" : "DERROTA");
        tituloLabel.setStyle("-fx-font-size: 72px; -fx-font-weight: bold; -fx-text-fill: " +
                (victoria ? "#c1cf4d" : "#cf4d4d") + ";");

        Label mensajeLabel = new Label(victoria ?
                "¡Felicitaciones! Has completado todos los niveles" :
                "Has sido derrotado. ¡Inténtalo de nuevo!");
        mensajeLabel.setStyle("-fx-font-size: 20px; -fx-text-fill: white;");
        mensajeLabel.setWrapText(true);
        mensajeLabel.setMaxWidth(600);
        mensajeLabel.setAlignment(Pos.CENTER);

        Button volverButton = boton.crearBoton("Volver al Menú", volverAlMenu);

        root.getChildren().addAll(tituloLabel, mensajeLabel, volverButton);

        Scene scene = new Scene(root, WIDTH, HEIGHT);
        stage.setScene(scene);
        stage.show();
    }
}

