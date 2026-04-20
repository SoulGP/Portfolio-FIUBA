package fiuba.encapsulados.modelo;

public class BloqueLadrillo extends Bloque {
    private static final int MAX_IMPACTOS = 3;
    private int impactos;

    public BloqueLadrillo(int x, int y) {
        super(x, y, false);
    }

    @Override
    public void recibirDisparo(Disparo disparo, GameManager gameManager, ServicioSonido sonidos) {
        impactos++;
        sonidos.play(Event.IMPACTO_LADRILLO);
        if (impactos >= MAX_IMPACTOS) {
            gameManager.onBloqueDestruido(this);
        }
        disparo.recibirImpacto();
    }
}
