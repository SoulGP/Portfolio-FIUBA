package fiuba.encapsulados.modelo;

public enum TipoEnemigo {
    FAST("fastEnemy") {
        @Override
        public Enemigo crear(int x, int y) {
            return new EnemigoRapido(x, y);
        }
    },
    REGULAR("regularEnemy") {
        @Override
        public Enemigo crear(int x, int y) {
            return new EnemigoBasico(x, y);
        }
    },
    HEAVY("heavyEnemy") {
        @Override
        public Enemigo crear(int x, int y) {
            return new EnemigoBlindado(x, y);
        }
    },
    POWERFUL("powerfulEnemy") {
        @Override
        public Enemigo crear(int x, int y) {
            return new EnemigoPotente(x, y);
        }
    };


    private final String tipo;

    TipoEnemigo(String tipo) {
        this.tipo = tipo;
    }

    public String getTipo() {
        return tipo;
    }

    public abstract Enemigo crear(int x, int y);
}

