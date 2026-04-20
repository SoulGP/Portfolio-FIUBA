package fiuba.encapsulados.vista;

import javafx.scene.control.Button;

public class Boton {
    private static final String BUTTON_STYLE = "-fx-background-color: #00130b; -fx-text-fill: white; -fx-font-size: 16px; -fx-font-weight: bold; -fx-background-radius: 6;";
    private static final String BUTTON_HOVER_STYLE = "-fx-background-color: #c1cf4d; -fx-text-fill: white; -fx-font-size: 16px; -fx-font-weight: bold; -fx-background-radius: 6;";

    public Button crearBoton(String texto, Runnable accion) {
        Button button = new Button(texto);
        button.setPrefWidth(260);
        button.setStyle(BUTTON_STYLE);
        button.setOnAction(event -> {
            if (accion != null) {
                accion.run();
            }
        });
        button.setOnMouseEntered(event -> button.setStyle(BUTTON_HOVER_STYLE));
        button.setOnMouseExited(event -> button.setStyle(BUTTON_STYLE));
        button.setFocusTraversable(false);
        return button;
    }
}
