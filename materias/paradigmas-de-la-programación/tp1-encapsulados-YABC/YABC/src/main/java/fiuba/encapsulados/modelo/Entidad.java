package fiuba.encapsulados.modelo;

public abstract class Entidad extends Posicion {
    protected int speed;
    protected int health;

    public Entidad(int x, int y, int speed, int health) {
        super(x, y);
        this.speed = speed;
        this.health = health;
    }

    public void move(Movimiento m) {
        this.x += m.getDx() * speed;
        this.y += m.getDy() * speed;
    }

    public void takeDamage(int damage) {
        this.health -= damage;
        if (health <= 0) {
            die();
        }
    }

    public void recibirImpacto() {
        takeDamage(1);
    }

    public void disparoDestruido() {
        // Por defecto no hace nada, pero los tanques jugadores lo usan
    }

    // Acción al morir (abstracta, para que cada tipo implemente)
    protected abstract void die();

    public int getHealth() {
        return health;
    }

    public int getSpeed() {
        return speed;
    }

    public boolean esEnemigo() {
        return false;
    }
}
