
import fiuba.encapsulados.modelo.*;
import org.junit.jupiter.api.Test;

import java.util.List;
import java.util.Map;

import static org.junit.jupiter.api.Assertions.*;

public class ColisionTest {
    @Test
    void testDisparoImpactaEnemigoAutomaticamente() {
        TanqueJugador jugador = new TanqueJugador(0, 0, "Jugador1");
        Enemigo enemigo = new Enemigo(3, 0, 1, 1);

        Disparo bala = jugador.disparar(Movimiento.DERECHA);
        List<Disparo> disparos = List.of(bala);

        InfoNivel nivel = new InfoNivel(
                800, 600, 40, 30,
                List.of(jugador),
                Map.of(), // controlesPorJugador
                List.of(enemigo),
                List.of(), // bloques
                List.of()  // powerUps
        );
        GameManager gameManager = new GameManager(nivel);

        GestorColisiones gestor = new GestorColisiones();
        for (int i = 0; i < 3; i++) {
            bala.update();
            gestor.detectarColisiones(disparos, nivel, gameManager);
        }

        assertEquals(0, enemigo.getHealth(), "El enemigo debe morir al recibir un disparo");
        assertFalse(bala.estaActivo(), "El disparo debe estar destruido tras el impacto");
    }
}