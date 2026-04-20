package fiuba.encapsulados.modelo;

import java.util.List;
import java.util.Map;

public record InfoNivel(int width, int height, int cols, int rows,
                        List<TanqueJugador> jugadores, Map<TanqueJugador, ControlesJugador> controlesPorJugador,
                        List<Enemigo> enemigos,
                        List<Bloque> bloques,
                        List<PowerUp> powerUps) {
}