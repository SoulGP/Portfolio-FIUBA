package fiuba.encapsulados.modelo;

import java.io.File;
import java.net.URL;

public class CargadorNiveles {
    public InfoNivel cargarNivel(String resourcePath) throws Exception {
        URL resource = getClass().getResource(resourcePath);
        if (resource == null) {
            throw new IllegalStateException("No se encontró el recurso de nivel: " + resourcePath);
        }
        ParseadorXML parseador = new ParseadorXML();
        return parseador.parse(new File(resource.toURI()));
    }
}
