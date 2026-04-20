package fiuba.encapsulados.modelo;

import java.util.HashMap;
import java.util.Map;

public class IdentificadorTipoBloque {
    private final Map<String, TipoBloque> hash = new HashMap<>();

    public IdentificadorTipoBloque() {
        for (TipoBloque t : TipoBloque.values()) {
            hash.put(t.getTipo(), t);
        }
    }

    public TipoBloque identificarBloque(String tipo) {
        return hash.get(tipo);
    }
}
