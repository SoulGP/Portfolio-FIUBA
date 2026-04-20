package fiuba.encapsulados.modelo;

import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

public class PowerUpPala extends PowerUp {

    private static final int TILE_SIZE = 20;

    public PowerUpPala(int x, int y) {
        super(x, y);
    }

    @Override
    public void aplicar(TanqueJugador jugador, InfoNivel nivel, GameManager gameManager) {
        List<Bloque> bloques = nivel.bloques();
        if (bloques.isEmpty()) {
            return;
        }

        Set<Coordenada> objetivos = new HashSet<>();
        for (Bloque bloque : bloques) {
            if (bloque.esBase()) {
                agregarPerimetro(bloque.getX(), bloque.getY(), nivel.width(), nivel.height(), objetivos);
            }
        }

        if (objetivos.isEmpty()) {
            return;
        }

        ValidadorTanques validador = new ValidadorTanques();
        for (Coordenada coord : objetivos) {
            if (validador.hayTanqueEn(coord.x, coord.y, nivel)) {
                continue;
            }
            reemplazarPorAcero(coord.x, coord.y, bloques);
        }
    }

    private void agregarPerimetro(int baseX, int baseY, int width, int height, Set<Coordenada> destinos) {
        int maxX = Math.max(0, width - TILE_SIZE);
        int maxY = Math.max(0, height - TILE_SIZE);
        int[] offsets = {-TILE_SIZE, 0, TILE_SIZE};
        for (int dx : offsets) {
            for (int dy : offsets) {
                if (dx == 0 && dy == 0) {
                    continue;
                }
                int x = baseX + dx;
                int y = baseY + dy;
                if (x < 0 || x > maxX || y < 0 || y > maxY) {
                    continue;
                }
                destinos.add(new Coordenada(x, y));
            }
        }
    }

    private void reemplazarPorAcero(int x, int y, List<Bloque> bloques) {
        Iterator<Bloque> it = bloques.iterator();
        while (it.hasNext()) {
            Bloque existente = it.next();
            if (existente.getX() == x && existente.getY() == y) {
                if (existente.esBase() || existente.esAcero()) {
                    return;
                }
                it.remove();
                break;
            }
        }
        bloques.add(new BloqueAcero(x, y));
    }

    private record Coordenada(int x, int y) {
    }
}
