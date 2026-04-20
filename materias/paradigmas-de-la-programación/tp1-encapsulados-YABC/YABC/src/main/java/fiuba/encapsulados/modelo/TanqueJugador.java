package fiuba.encapsulados.modelo;


public class TanqueJugador extends Entidad implements Colision {
    private static final int VELOCIDAD_INICIAL = 4;
    private static final int VIDA = 3;
    private static final int CELDA_TANQUE = 20;
    private static final int CELDA_BALA = 5;
    private final String nombre;
    private int disparosActivos;
    private boolean invulnerable;
    private boolean disparoPotente;
    private boolean paralizado = false;
    private boolean realizoDisparo = false;
    private long tiempoParalizadoHasta = 0;

    public TanqueJugador(int x, int y, String nombre) {
        super(x, y, VELOCIDAD_INICIAL, VIDA);
        this.nombre = nombre;
        this.disparosActivos = 0;
        this.invulnerable = false;
        this.disparoPotente = false;
    }

    public boolean realizoDisparo() {
        return realizoDisparo;
    }

    public void resetearDisparo() {
        realizoDisparo = false;
    }

    public Disparo disparar(Movimiento direccion) {
        if (disparosActivos < 1) {
            disparosActivos++;
            int centroX = x + CELDA_TANQUE / 2 - CELDA_BALA / 2;
            int centroY = y + CELDA_TANQUE / 2 - CELDA_BALA / 2;
            realizoDisparo = true;
            // 🔹 Pasamos la info de si es potente al disparo
            return new Disparo(centroX, centroY, 10, direccion, this, disparoPotente);
        } else {
            return null;
        }
    }


    public void disparoDestruido() {
        if (disparosActivos > 0) disparosActivos--;
    }

    public String getNombre() {
        return nombre;
    }

    public boolean isInvulnerable() {
        return invulnerable;
    }

    public void setInvulnerable(boolean invulnerable) {
        this.invulnerable = invulnerable;
    }

    public void setDisparoPotente(boolean disparoPotente) {
        this.disparoPotente = disparoPotente;
    }

    @Override
    public void takeDamage(int damage) {
        if (!invulnerable) {
            super.takeDamage(damage);
        }
    }

    @Override
    protected void die() {
        this.health = 0;
    }

    public boolean estaParalizado() {
        return paralizado && System.currentTimeMillis() < tiempoParalizadoHasta;
    }

    public void paralizar(int milisegundos) {
        paralizado = true;
        tiempoParalizadoHasta = System.currentTimeMillis() + milisegundos;
    }


    @Override
    public void recibirDisparo(Disparo disparo, GameManager gameManager, ServicioSonido sonidos) {
        sonidos.play(Event.IMPACTO_ACERO);
        if (disparo.getPropietario().esEnemigo()) {
            this.takeDamage(1);
            disparo.die();
            if (this.health <= 0) {
                gameManager.onJugadorMuere(this);
            }
        } else {
            disparo.die();
            this.paralizar(2000); // 2 segundos
        }
    }
}

