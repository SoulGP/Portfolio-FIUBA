package fiuba.encapsulados.modelo;

public class BloqueTanque extends Bloque {
    public BloqueTanque(int x, int y) {
        super(x, y, false);
    }

    @Override
    public void recibirDisparo(Disparo disparo, GameManager gameManager, ServicioSonido sonidos) {
        sonidos.play(Event.IMPACTO_ACERO);
        disparo.recibirImpacto();
    }
}
