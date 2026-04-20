package fiuba.encapsulados.modelo;

public enum Movimiento {
    ARRIBA(0, -1, 0),
    ABAJO(0, 1, 180),
    IZQUIERDA(-1, 0, 270),
    DERECHA(1, 0, 90);

    private final int dx;
    private final int dy;
    private final int angulo;

    Movimiento(int dx, int dy, int angulo) {
        this.dx = dx;
        this.dy = dy;
        this.angulo = angulo;
    }

    public int getDx() {
        return dx;
    }

    public int getDy() {
        return dy;
    }

    public int getAngulo() {
        return angulo;
    }
}
