package fiuba.encapsulados.modelo;

import org.w3c.dom.Document;
import org.xml.sax.SAXException;

import javax.xml.XMLConstants;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;
import javax.xml.validation.Validator;
import java.io.File;

public class ValidadorXML {
    public boolean validar(File xmlFile, File xsdFile) {
        try {
            // creación del interprete y reglas a seguir
            SchemaFactory factory = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI);
            Schema schema = factory.newSchema(xsdFile);

            Validator validator = schema.newValidator();

            // Creación de arbol DOM a partir del XML
            DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
            DocumentBuilder parser = dbf.newDocumentBuilder();
            Document document = parser.parse(xmlFile);

            validator.validate(new javax.xml.transform.dom.DOMSource(document));
            return true;

        } catch (SAXException e) {
            System.err.println("Error de validación XML: " + e.getMessage());
            return false;
        } catch (java.io.IOException e) {
            System.err.println("Error de E/S: " + e.getMessage());
            return false;
        } catch (Exception e) {
            System.err.println("Error al procesar XML/XSD: " + e.getMessage());
            return false;
        }
    }
}
