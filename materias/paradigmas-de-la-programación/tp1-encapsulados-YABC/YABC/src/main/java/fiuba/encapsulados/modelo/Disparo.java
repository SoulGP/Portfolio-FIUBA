package fiuba.encapsulados.modelo;

public class Disparo extends Entidad {

    private final Movimiento direccion;
    private final Entidad propietario;
    private final boolean potente; // ⭐ nuevo flag
    private boolean activo;

    public Disparo(int x, int y, int speed, Movimiento direccion, Entidad propietario, boolean potente) {
        super(x, y, speed, 1);
        this.direccion = direccion;
        this.propietario = propietario;
        this.activo = true;
        this.potente = potente;
    }

    // Sobrecarga para no romper código viejo (por defecto NO es potente)
    public Disparo(int x, int y, int speed, Movimiento direccion, Entidad propietario) {
        this(x, y, speed, direccion, propietario, false);
    }

    @Override
    public void die() {
        this.health = 0;
        this.activo = false;
    }

    public void update() {
        if (activo) {
            move(direccion);
        }
    }

    public void recibirImpacto() {
        die(); // el disparo siempre se destruye al impactar
    }

    public boolean estaActivo() {
        return activo;
    }

    public Entidad getPropietario() {
        return propietario;
    }

    public boolean isPotente() {
        return potente;
    }
}
