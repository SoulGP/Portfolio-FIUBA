package fiuba.encapsulados.modelo;

public enum TipoBloque {
    BRICK("brickBlock") {
        @Override
        public Bloque crear(int x, int y) {
            return new BloqueLadrillo(x, y);
        }
    },
    WATER("waterBlock") {
        @Override
        public Bloque crear(int x, int y) {
            return new BloqueAgua(x, y);
        }
    },
    STEEL("steelBlock") {
        @Override
        public Bloque crear(int x, int y) {
            return new BloqueAcero(x, y);
        }
    },
    FOREST("forestBlock") {
        @Override
        public Bloque crear(int x, int y) {
            return new BloqueBosque(x, y);
        }
    },
    BASE("baseBlock") {
        @Override
        public Bloque crear(int x, int y) {
            return new BloqueBase(x, y);
        }
    };

    private final String tipo;

    TipoBloque(String tipo) {
        this.tipo = tipo;
    }

    public String getTipo() {
        return tipo;
    }

    public abstract Bloque crear(int x, int y);
}

