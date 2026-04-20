import fiuba.encapsulados.modelo.Disparo;
import fiuba.encapsulados.modelo.Enemigo;
import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class EnemigoTest {

    @Test
    void testRecibeDanioYMuere() {
        Enemigo enemigo = new Enemigo(0, 0, 1, 2);
        enemigo.takeDamage(1);
        assertTrue(enemigo.getHealth() > 0, "El enemigo debería seguir vivo con vida > 0");
        enemigo.takeDamage(1);
        assertEquals(0, enemigo.getHealth(), "El enemigo debería estar muerto con vida = 0");
    }

    @Test
    void testUpdateMueveAlEnemigo() {
        Enemigo enemigo = new Enemigo(0, 0, 1, 2);
        List<Disparo> disparos = new java.util.ArrayList<>();
        int oldX = enemigo.getX();
        int oldY = enemigo.getY();
        enemigo.update(1f, disparos); // simula 1 segundo
        boolean moved = enemigo.getX() != oldX || enemigo.getY() != oldY;
        assertTrue(moved, "El enemigo debería haberse movido");
    }
}