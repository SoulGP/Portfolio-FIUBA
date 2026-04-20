package fiuba.encapsulados.modelo;

import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

public class UtilidadesXML {

    public Element obtenerHijosPorTag(Element raiz, String tag) {
        NodeList nl = raiz.getElementsByTagName(tag);
        if (nl.getLength() == 0) return null;
        return (Element) nl.item(0);
    }

    public String obtenerAtributo(Element e, String atributo, String def) {
        if (e.hasAttribute(atributo))
            return e.getAttribute(atributo);
        return def;
    }

    public int getIntAttr(Element e, String atributo) {
        return Integer.parseInt(obtenerAtributo(e, atributo, "0"));
    }
}

