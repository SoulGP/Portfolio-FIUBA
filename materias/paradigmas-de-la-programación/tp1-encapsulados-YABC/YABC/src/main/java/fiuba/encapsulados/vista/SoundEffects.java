package fiuba.encapsulados.vista;

import fiuba.encapsulados.modelo.Event;
import fiuba.encapsulados.modelo.ServicioSonido;
import javafx.scene.media.AudioClip;
import javafx.scene.media.Media;
import javafx.scene.media.MediaPlayer;

import java.util.EnumMap;

public class SoundEffects implements ServicioSonido {
    private final EnumMap<Event, AudioClip> sfx;
    private MediaPlayer backgroundPlayer;

    public SoundEffects() {
        sfx = new EnumMap<>(Event.class);
        for (Event e : Event.values()) {
            String resourcePath = getResourcePath("%s.mp3".formatted(e.name().toLowerCase()));
            sfx.put(e, new AudioClip(resourcePath));
        }
    }

    @Override
    public void play(Event e) {
        AudioClip clip = sfx.get(e);
        if (clip != null) {
            clip.play();
        }
    }

    @Override
    public void playMenuMusicLoop() {
        detenerMusica();
        try {
            String resourcePath = getResourcePath("musica_menu.mp3");
            backgroundPlayer = new MediaPlayer(new Media(resourcePath));
            backgroundPlayer.setCycleCount(MediaPlayer.INDEFINITE);
            backgroundPlayer.play();
        } catch (RuntimeException ex) {
            backgroundPlayer = null;
            System.err.println("No se pudo iniciar la musica de menu: " + ex.getMessage());
        }
    }

    @Override
    public void detenerMusica() {
        if (backgroundPlayer != null) {
            try {
                backgroundPlayer.stop();
                backgroundPlayer.dispose();
            } catch (Exception ignored) {
                // no-op
            } finally {
                backgroundPlayer = null;
            }
        }
    }

    private String getResourcePath(String path) {
        String resourcePath = "/sounds/%s".formatted(path);
        var resource = getClass().getResource(resourcePath);
        if (resource == null) {
            throw new RuntimeException("could not load resource '" + resourcePath + "'");
        }
        return resource.toExternalForm();
    }
}
