package fiuba.encapsulados.vista;

import javafx.scene.image.Image;

public record CicloSprite(Image[] frames) {
    public CicloSprite(Image[] frames) {
        this.frames = frames.clone();
    }

    public Image frame(int index) {
        if (frames.length == 0) {
            throw new IllegalStateException("CicloSprite sin frames");
        }
        if (index <= 0) {
            return frames[0];
        }
        if (index >= frames.length) {
            return frames[frames.length - 1];
        }
        return frames[index];
    }

    public int next(int current) {
        if (frames.length <= 1) {
            return 0;
        }
        return (current + 1) % frames.length;
    }
}


