import fiuba.encapsulados.modelo.Movimiento;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class MovimientoTest {
    @Test
    void testArriba() {
        assertEquals(0, Movimiento.ARRIBA.getDx(), "ARRIBA debe tener dx=0");
        assertEquals(-1, Movimiento.ARRIBA.getDy(), "ARRIBA debe tener dy=-1");
    }

    @Test
    void testAbajo() {
        assertEquals(0, Movimiento.ABAJO.getDx(), "ABAJO debe tener dx=0");
        assertEquals(1, Movimiento.ABAJO.getDy(), "ABAJO debe tener dy=1");
    }

    @Test
    void testIzquierda() {
        assertEquals(-1, Movimiento.IZQUIERDA.getDx(), "IZQUIERDA debe tener dx=-1");
        assertEquals(0, Movimiento.IZQUIERDA.getDy(), "IZQUIERDA debe tener dy=0");
    }

    @Test
    void testDerecha() {
        assertEquals(1, Movimiento.DERECHA.getDx(), "DERECHA debe tener dx=1");
        assertEquals(0, Movimiento.DERECHA.getDy(), "DERECHA debe tener dy=0");
    }
}