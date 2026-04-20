package fiuba.encapsulados.modelo;

public class BloqueBase extends Bloque {
    private int impactos;

    public BloqueBase(int x, int y) {
        super(x, y, false);
    }

    @Override
    public boolean esBase() {
        return true;
    }

    @Override
    public void recibirDisparo(Disparo disparo, GameManager gameManager, ServicioSonido sonidos) {
        impactos++;
        if (impactos >= 1) {
            gameManager.onBloqueDestruido(this);
            gameManager.onBaseDestruida();
            disparo.recibirImpacto();
        }
    }
}
