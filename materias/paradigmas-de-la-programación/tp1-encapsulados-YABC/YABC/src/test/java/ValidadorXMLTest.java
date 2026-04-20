import static org.junit.jupiter.api.Assertions.*;
import java.io.File;

import fiuba.encapsulados.modelo.ValidadorXML;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

public class ValidadorXMLTest {
    private static File xmlFile;
    private static File xsdFile;
    private static ValidadorXML validador;

    @BeforeAll
    static void setup() {
        // Ajustá las rutas según tu proyecto
        xmlFile = new File("src/test/resources/Level0.xml");
        xsdFile = new File("src/test/resources/levelConfig.xsd");
        validador = new ValidadorXML();
    }

    @Test
    void testXMLValidoContraXSD() {
        boolean resultado = validador.validar(xmlFile, xsdFile);
        assertTrue(resultado, "El archivo XML debería ser válido contra el XSD");
    }

    @Test
    void testXMLInvalidoContraXSD() {
        File xmlInvalido = new File("src/test/resources/LevelInvalido.xml");
        boolean resultado = validador.validar(xmlInvalido, xsdFile);
        assertFalse(resultado, "El archivo XML inválido NO debería pasar la validación");
    }
}