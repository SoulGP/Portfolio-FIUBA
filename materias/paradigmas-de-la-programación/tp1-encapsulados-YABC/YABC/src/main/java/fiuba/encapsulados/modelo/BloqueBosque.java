package fiuba.encapsulados.modelo;

public class BloqueBosque extends Bloque {
    public BloqueBosque(int x, int y) {
        super(x, y, true);
    }

    @Override
    public void recibirDisparo(Disparo disparo, GameManager gameManager, ServicioSonido sonidos) {
    }

}
