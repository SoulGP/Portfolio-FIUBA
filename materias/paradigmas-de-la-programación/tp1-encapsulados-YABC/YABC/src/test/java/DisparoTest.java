
import fiuba.encapsulados.modelo.Disparo;
import fiuba.encapsulados.modelo.Movimiento;
import fiuba.encapsulados.modelo.TanqueJugador;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

class DisparoTest {

    @Test
    void testDisparoSeMueveArriba() {
        TanqueJugador jugador = new TanqueJugador(0, 0,"Jugador1");
        Disparo disparo = new Disparo(0, 0, 1, Movimiento.ARRIBA, jugador);

        disparo.update();
        assertEquals(0, disparo.getX(), "El disparo no debe moverse en X si va ARRIBA");
        assertEquals(-1, disparo.getY(), "El disparo debe moverse hacia arriba en Y");
    }

    @Test
    void testDisparoSeDestruyeConImpacto() {
        TanqueJugador jugador = new TanqueJugador(0, 0,"Jugador1");
        Disparo disparo = new Disparo(0, 0, 1, Movimiento.ARRIBA, jugador);

        assertTrue(disparo.estaActivo(), "El disparo debe empezar activo");
        disparo.recibirImpacto();
        assertFalse(disparo.estaActivo(), "El disparo debe estar destruido tras impacto");
    }
}
