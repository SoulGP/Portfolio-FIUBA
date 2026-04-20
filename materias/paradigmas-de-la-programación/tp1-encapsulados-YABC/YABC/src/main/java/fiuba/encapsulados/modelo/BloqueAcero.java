package fiuba.encapsulados.modelo;

public class BloqueAcero extends Bloque {
    public BloqueAcero(int x, int y) {
        super(x, y, false);
    }

    @Override
    public boolean esAcero() {
        return true;
    }

    @Override
    public void recibirDisparo(Disparo disparo, GameManager gameManager, ServicioSonido sonidos) {
        sonidos.play(Event.IMPACTO_ACERO);
        disparo.recibirImpacto();
    }
}
