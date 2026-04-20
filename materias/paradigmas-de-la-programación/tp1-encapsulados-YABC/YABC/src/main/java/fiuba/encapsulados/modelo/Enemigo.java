package fiuba.encapsulados.modelo;

import java.util.List;
import java.util.Random;
import java.util.function.BiPredicate;

public class Enemigo extends Entidad implements Colision {
    private final Random rand;
    private Movimiento direccion;
    private float tiempoConducta; // segundos que dura en la direccion actual
    private float tiempoEnLugar;
    private float tiempoDesdeUltimoDisparo = 0f;
    private boolean realizoDisparo = false;
    private BiPredicate<Integer, Integer> validadorMovimiento;

    public Enemigo(int x, int y, int speed, int health) {
        super(x, y, speed, health);
        this.rand = new Random();
        this.direccion = randomDirection();
        this.tiempoConducta = 1 + rand.nextInt(5);
        this.tiempoEnLugar = 0f;
    }

    public void setValidadorMovimiento(BiPredicate<Integer, Integer> validadorMovimiento) {
        this.validadorMovimiento = validadorMovimiento;
    }

    public Movimiento getDireccion() {
        return direccion;
    }

    public boolean realizoDisparo() {
        return realizoDisparo;
    }

    public void resetearDisparo() {
        realizoDisparo = false;
    }

    private Movimiento randomDirection() {
        Movimiento[] direcciones = Movimiento.values();
        return direcciones[rand.nextInt(direcciones.length)];
    }

    public void disparar(List<Disparo> disparos) {
        int disparoX = getX() + 10; // Ajusta para centrar el disparo
        int disparoY = getY() + 10;
        Disparo nuevoDisparo = new Disparo(disparoX, disparoY, 5, direccion, this);
        disparos.add(nuevoDisparo);
        realizoDisparo = true;
    }

    // Actualiza el enemigo segun la IA
    public void update(float deltaTime, List<Disparo> disparos) {

        int oldX = x;
        int oldY = y;

        int nuevoX = x + direccion.getDx() * speed;
        int nuevoY = y + direccion.getDy() * speed;

        if (validadorMovimiento == null || validadorMovimiento.test(nuevoX, nuevoY)) {
            setPosicion(nuevoX, nuevoY);
        }

        tiempoDesdeUltimoDisparo += deltaTime;
        if (tiempoDesdeUltimoDisparo > 2.0f) {
            disparar(disparos);
            tiempoDesdeUltimoDisparo = 0f;
        }

        if (x == oldX && y == oldY) {
            tiempoEnLugar += deltaTime;
        } else {
            tiempoEnLugar = 0;
        }
        tiempoConducta -= deltaTime;

        // Cambiar direccion si termina el tiempo de conducta o si esta mas de 2 seg en el mismo lugar
        if (tiempoConducta <= 0 || tiempoEnLugar > 2) {
            direccion = randomDirection();
            tiempoConducta = 1 + rand.nextInt(5);
            tiempoEnLugar = 0;
        }

    }

    @Override
    public boolean esEnemigo() {
        return true;
    }

    @Override
    protected void die() {
        this.health = 0;
    }

    @Override
    public void recibirImpacto() {
        takeDamage(1); // cada bala le quita 1 de vida
    }

    @Override
    public void recibirDisparo(Disparo disparo, GameManager gameManager, ServicioSonido sonidos) {
        if (disparo.isPotente()) {
            takeDamage(getHealth()); // muerte instantanea
        } else {
            sonidos.play(Event.IMPACTO_ACERO);
            recibirImpacto();
        }
        if (getHealth() <= 0) {
            gameManager.onEnemyDeath(this);
        }
        disparo.recibirImpacto();
    }
}
