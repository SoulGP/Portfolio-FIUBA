package fiuba.encapsulados.modelo;

public abstract class Bloque extends Posicion implements Colision {
    protected Boolean atravesable;

    public Bloque(int x, int y, Boolean atravesable) {
        super(x, y);
        this.atravesable = atravesable;
    }

    public boolean esAtravesable() {
        return atravesable;
    }

    public boolean esBase() {
        return false;
    }

    public boolean esAcero() {
        return false;
    }
}