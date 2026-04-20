package com.example.davici;

import clojure.java.api.Clojure;
import clojure.lang.IFn;
import java.nio.IntBuffer;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.control.TextArea;
import javafx.scene.image.ImageView;
import javafx.scene.image.PixelFormat;
import javafx.scene.image.WritableImage;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;
import javafx.util.Duration;

public class App extends Application {

    private static final int W = 256;
    private static final int H = 256;
    private final ExecutorService pool = Executors.newWorkStealingPool();

    private IFn require;
    private IFn nth;
    private IFn evalFrame;

    private volatile int t = 0;
    private volatile String code = "";
    private volatile String lastError = null;
    private final AtomicBoolean rendering = new AtomicBoolean(false);
    private final AtomicInteger codeGen = new AtomicInteger(0);

    private Label status;
    private ImageView view;
    private WritableImage canvas;

    private Timeline timeline;

    @Override
    public void start(Stage stage) {
        String initial = System.getProperty("davici.initialCode", "");
        this.code = initial;

        require = Clojure.var("clojure.core", "require");
        require.invoke(Clojure.read("davici.vm"));
        evalFrame = Clojure.var("davici.vm", "eval-frame");
        nth = Clojure.var("clojure.core", "nth");

        var editor = new TextArea(initial);
        editor.setPrefColumnCount(60);
        editor.setPrefRowCount(8);
        editor
            .textProperty()
            .addListener((obs, oldV, newV) -> {
                code = newV == null ? "" : newV;
                t = 0; // reiniciar tiempo
                lastError = null;
                codeGen.incrementAndGet();
                updateStatus();
                tick();

                if (
                    timeline.getStatus() != Timeline.Status.RUNNING &&
                    !code.isBlank()
                ) {
                    timeline.play();
                }
            });

        status = new Label("t: 0");
        view = new ImageView();
        canvas = new WritableImage(W, H);
        view.setImage(canvas);
        view.setFitWidth(W);
        view.setFitHeight(H);
        view.setPreserveRatio(true);

        var root = new BorderPane();
        var top = new VBox(editor, status);
        root.setTop(top);
        root.setCenter(view);

        var scene = new Scene(root);
        stage.setScene(scene);
        stage.setTitle("Davici - Animación 256x256");
        stage.setResizable(false);
        stage.show();

        // Timeline ~10 FPS (cada 100 ms), 256 cuadros
        timeline = new Timeline(
            new KeyFrame(Duration.millis(100), e -> tick())
        );
        timeline.setCycleCount(Timeline.INDEFINITE);

        if (!code.isBlank()) {
            tick();
            timeline.play();
        }
    }

    private void tick() {
        String currentCode = this.code;
        if (currentCode.length() > 1024) {
            lastError = "Código supera 1024 caracteres";
            updateStatus();
            return;
        }

        if (currentCode.isBlank()) {
            updateStatus();
            return;
        }

        if (!rendering.compareAndSet(false, true)) {
            return;
        }
        int currentT = this.t;
        int gen = codeGen.get();
        // Render en background
        CompletableFuture.supplyAsync(
            () -> renderFrame(currentCode, currentT),
            pool
        ).whenComplete((argb, ex) -> {
            rendering.set(false);
            if (ex != null) {
                lastError = ex.getCause() != null
                    ? ex.getCause().getMessage()
                    : ex.getMessage();
            } else if (argb != null) {
                if (codeGen.get() == gen) {
                    WritableImage fxImage = new WritableImage(W, H);
                    fxImage
                        .getPixelWriter()
                        .setPixels(
                            0,
                            0,
                            W,
                            H,
                            PixelFormat.getIntArgbPreInstance(),
                            IntBuffer.wrap(argb),
                            W
                        );
                    Platform.runLater(() -> view.setImage(fxImage));
                    t = (currentT + 1) & 0xFF;
                    lastError = null;
                }
            }
            updateStatus();
        });
    }

    private void updateStatus() {
        Platform.runLater(() -> {
            if (lastError != null && !lastError.isBlank()) {
                status.setText("Error: " + lastError);
            } else {
                status.setText("t: " + t);
            }
        });
    }

    private int[] renderFrame(String code, int t) {
        Object res = evalFrame.invoke(code, t);
        if (!(res instanceof int[])) {
            throw new IllegalStateException("eval-frame no devolvió int[]");
        }
        return (int[]) res;
    }
}
