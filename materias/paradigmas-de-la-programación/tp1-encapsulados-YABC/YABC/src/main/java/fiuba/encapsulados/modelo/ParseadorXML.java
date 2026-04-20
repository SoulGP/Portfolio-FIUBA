package fiuba.encapsulados.modelo;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class ParseadorXML {
    private final IdentificadorTipoEnemigo identificadorEnemigo = new IdentificadorTipoEnemigo();
    private final IdentificadorTipoBloque identificadorBloque = new IdentificadorTipoBloque();
    private final UtilidadesXML util = new UtilidadesXML();

    public InfoNivel parse(File xmlFile) throws Exception {
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        DocumentBuilder builder = factory.newDocumentBuilder();
        Document doc = builder.parse(xmlFile);

        Element root = doc.getDocumentElement();
        Element level = util.obtenerHijosPorTag(root, "level");
        if (level == null) return null;

        int width = util.getIntAttr(level, "width");
        int height = util.getIntAttr(level, "height");
        int cols = util.getIntAttr(level, "cols");
        int rows = util.getIntAttr(level, "rows");

        List<TanqueJugador> jugadores = new ArrayList<>();
        List<Enemigo> enemigos = new ArrayList<>();
        List<Bloque> bloques = new ArrayList<>();
        List<PowerUp> powerUps = new ArrayList<>();
        NodeList nl;

        Element players = util.obtenerHijosPorTag(level, "players");
        if (players == null) return null;
        Map<TanqueJugador, ControlesJugador> controlesPorJugador = new java.util.HashMap<>(Map.of());
        nl = players.getElementsByTagName("player");
        for (int i = 0; i < nl.getLength(); i++) {
            Element p = (Element) nl.item(i);
            String id = util.obtenerAtributo(p, "id", "player");
            int x = util.getIntAttr(p, "x");
            int y = util.getIntAttr(p, "y");
            TanqueJugador jugador = new TanqueJugador(x, y, id);
            jugadores.add(jugador);
            controlesPorJugador.put(jugador, i == 0 ? ControlesJugador.JUGADOR1 : ControlesJugador.JUGADOR2);
        }

        Element enemies = util.obtenerHijosPorTag(level, "enemies");
        if (enemies == null) return null;
        nl = enemies.getElementsByTagName("enemy");
        for (int i = 0; i < nl.getLength(); i++) {
            Element e = (Element) nl.item(i);
            String type = util.obtenerAtributo(e, "type", "");
            int x = util.getIntAttr(e, "x");
            int y = util.getIntAttr(e, "y");
            Enemigo enemigo = crearEnemigo(type, x, y);
            enemigos.add(enemigo);
        }

        Element staticObjects = util.obtenerHijosPorTag(level, "staticObjects");
        if (staticObjects == null) return null;
        nl = staticObjects.getElementsByTagName("staticObject");
        for (int i = 0; i < nl.getLength(); i++) {
            Element so = (Element) nl.item(i);
            String type = util.obtenerAtributo(so, "type", "");
            int x = util.getIntAttr(so, "x");
            int y = util.getIntAttr(so, "y");
            Bloque bloque = crearBloque(type, x, y);
            bloques.add(bloque);
        }

        return new InfoNivel(width, height, cols, rows, jugadores, controlesPorJugador, enemigos, bloques, powerUps);
    }

    private Enemigo crearEnemigo(String type, int x, int y) {
        TipoEnemigo tipo = identificadorEnemigo.identificarEnemigo(type);
        if (tipo != null)
            return tipo.crear(x, y);
        return null;
    }

    private Bloque crearBloque(String type, int x, int y) {
        TipoBloque tipo = identificadorBloque.identificarBloque(type);
        if (tipo != null)
            return tipo.crear(x, y);
        return null;
    }
}





