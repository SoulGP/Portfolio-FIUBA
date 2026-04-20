
import fiuba.encapsulados.modelo.Disparo;
import fiuba.encapsulados.modelo.Movimiento;
import fiuba.encapsulados.modelo.TanqueJugador;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

class TanqueJugadorTest {

    @Test
    void testMuereCuandoPierdeTodaLaVida() {
        TanqueJugador jugador = new TanqueJugador(0, 0, "Jugador1");

        jugador.takeDamage(3);
        assertEquals(0, jugador.getHealth(), "El jugador debería tener health=0 tras morir");
    }


    @Test
    void testSoloUnDisparoActivo() {
        TanqueJugador jugador = new TanqueJugador(0, 0, "Jugador1");
        Disparo d1 = jugador.disparar(Movimiento.DERECHA);
        assertNotNull(d1, "El primer disparo debe crearse");

        Disparo d2 = jugador.disparar(Movimiento.ARRIBA);
        assertNull(d2, "No debe permitirse un segundo disparo activo");

        d1.die();
        jugador.disparoDestruido();

        Disparo d3 = jugador.disparar(Movimiento.IZQUIERDA);
        assertNotNull(d3, "Tras destruir el disparo, debe poder disparar de nuevo");
    }

    @Test
    void testInvulnerabilidad() {
        TanqueJugador jugador = new TanqueJugador(0, 0, "Jugador1");
        jugador.setInvulnerable(true);
        int vidaAntes = jugador.getHealth();

        jugador.takeDamage(2);
        assertEquals(vidaAntes, jugador.getHealth(), "El jugador no debe recibir daño estando invulnerable");
    }
}
