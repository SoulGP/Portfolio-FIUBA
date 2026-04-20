package fiuba.encapsulados.modelo;

import java.util.HashMap;
import java.util.Map;

public class IdentificadorTipoEnemigo {
    private final Map<String, TipoEnemigo> hash = new HashMap<>();

    public IdentificadorTipoEnemigo() {
        for (TipoEnemigo t : TipoEnemigo.values()) {
            hash.put(t.getTipo(), t);
        }
    }

    public TipoEnemigo identificarEnemigo(String tipo) {
        return hash.get(tipo);
    }
}
