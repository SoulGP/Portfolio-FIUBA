package fiuba.encapsulados.modelo;

import java.util.concurrent.ThreadLocalRandom;

public class IdentificadorTipoPowerUp {
    public PowerUp crearAleatorio(int x, int y) {
        TipoPowerUp[] values = TipoPowerUp.values();
        int index = ThreadLocalRandom.current().nextInt(values.length);
        return values[index].crear(x, y);
    }
}