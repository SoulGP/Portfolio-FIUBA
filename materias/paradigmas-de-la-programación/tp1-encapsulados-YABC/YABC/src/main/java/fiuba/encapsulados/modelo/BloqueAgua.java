package fiuba.encapsulados.modelo;

public class BloqueAgua extends Bloque {
    public BloqueAgua(int x, int y) {
        super(x, y, false);
    }

    @Override
    public void recibirDisparo(Disparo disparo, GameManager gameManager, ServicioSonido sonidos) {
    }

}
