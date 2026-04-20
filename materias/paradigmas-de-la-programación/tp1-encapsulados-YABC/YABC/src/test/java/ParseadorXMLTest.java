import fiuba.encapsulados.modelo.*;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import java.io.File;

import static org.junit.jupiter.api.Assertions.*;

class ParseadorXMLTest {

    private static File xmlValido;
    private static ParseadorXML parseador;

    @BeforeAll
    static void setup() {
        xmlValido = new File("src/test/resources/Level0.xml");
        parseador = new ParseadorXML();
    }

    @Test
    void testParseNivelValido() throws Exception {
        InfoNivel nivel = parseador.parse(xmlValido);

        assertNotNull(nivel);
        assertEquals(800, nivel.width());
        assertEquals(600, nivel.height());
        assertFalse(nivel.jugadores().isEmpty(), "Debe haber al menos un jugador");
        assertFalse(nivel.enemigos().isEmpty(), "Debe haber al menos un enemigo");
    }

    @Test
    void testBloquesReconocidos() throws Exception {
        InfoNivel nivel = parseador.parse(xmlValido);

        assertNotNull(nivel);
        boolean tieneBase = false;
        for (Bloque bloque : nivel.bloques()) {
            if (bloque.esBase()) {
                tieneBase = true;
                break;
            }
        }

        assertTrue(tieneBase, "Debe haber un bloque base en el nivel");
    }

    @Test
    void testJugadoresPosiciones() throws Exception {
        InfoNivel nivel = parseador.parse(xmlValido);

        assertNotNull(nivel);
        assertEquals(2, nivel.jugadores().size(), "Debe haber 2 jugadores");

        assertEquals(570, nivel.jugadores().get(0).getX());
        assertEquals(490, nivel.jugadores().get(0).getY());

        assertEquals(710, nivel.jugadores().get(1).getX());
        assertEquals(490, nivel.jugadores().get(1).getY());
    }

    @Test
    void testEnemigosPosiciones() throws Exception {
        InfoNivel nivel = parseador.parse(xmlValido);

        // Verifico algunos enemigos puntuales
        assertNotNull(nivel);
        assertTrue(nivel.enemigos().stream().anyMatch(e -> e.getX() == 330 && e.getY() == 30));
        assertTrue(nivel.enemigos().stream().anyMatch(e -> e.getX() == 730 && e.getY() == 30));
        assertTrue(nivel.enemigos().stream().anyMatch(e -> e.getX() == 90 && e.getY() == 570));
    }

    @Test
    void testBloquesPosiciones() throws Exception {
        InfoNivel nivel = parseador.parse(xmlValido);

        // Verifico algunos bloques específicos
        assertNotNull(nivel);
        assertTrue(nivel.bloques().stream().anyMatch(b -> b.getX() == 450 && b.getY() == 90));
        assertTrue(nivel.bloques().stream().anyMatch(b -> b.getX() == 270 && b.getY() == 110));
        assertTrue(nivel.bloques().stream().anyMatch(b -> b.getX() == 510 && b.getY() == 570));
    }
}

