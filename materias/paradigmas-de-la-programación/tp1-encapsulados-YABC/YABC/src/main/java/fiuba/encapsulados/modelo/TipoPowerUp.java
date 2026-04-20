package fiuba.encapsulados.modelo;

public enum TipoPowerUp {
    GRANADA {
        @Override
        public PowerUp crear(int x, int y) {
            return new PowerUpGranada(x, y);
        }
    },
    CASCO {
        @Override
        public PowerUp crear(int x, int y) {
            return new PowerUpCasco(x, y);
        }
    },
    ESTRELLA {
        @Override
        public PowerUp crear(int x, int y) {
            return new PowerUpEstrella(x, y);
        }
    },
    PALA {
        @Override
        public PowerUp crear(int x, int y) {
            return new PowerUpPala(x, y);
        }
    };

    public abstract PowerUp crear(int x, int y);
}
