package fiuba.encapsulados.modelo;

import java.util.LinkedHashSet;
import java.util.concurrent.ThreadLocalRandom;

class EnemySpawner {

    private static final int MAX_SPAWNS = 10;
    private static final long SPAWN_WINDOW_MILLIS = 60_000L;
    private static final long SPAWN_INTERVAL_MILLIS = 6_000L;
    private static final int ENEMY_SIZE = 20;

    private final InfoNivel nivel;
    private final ValidadorTanques validador = new ValidadorTanques();
    private final ThreadLocalRandom random = ThreadLocalRandom.current();
    private final int[] spawnXs;
    private final boolean[] slotBloqueado;

    private final long startMillis;
    private long lastSpawnMillis;
    private int spawned;
    private int nextSpawnSlot;

    EnemySpawner(InfoNivel nivel) {
        this.nivel = nivel;
        this.spawnXs = calcularSpawnXs(nivel.width());
        this.slotBloqueado = calcularSlotsBloqueados();
        this.startMillis = System.currentTimeMillis();
        this.lastSpawnMillis = startMillis;
        this.spawned = 0;
        this.nextSpawnSlot = 0;
    }

    void update() {
        if (spawned >= MAX_SPAWNS || spawnXs.length == 0) {
            return;
        }
        long now = System.currentTimeMillis();
        if (now - startMillis > SPAWN_WINDOW_MILLIS) {
            return;
        }
        if (now - lastSpawnMillis < SPAWN_INTERVAL_MILLIS) {
            return;
        }
        lastSpawnMillis = now;
        Enemigo nuevo = intentarCrearEnemigo();
        if (nuevo != null) {
            nivel.enemigos().add(nuevo);
            spawned++;
        }
    }

    private Enemigo intentarCrearEnemigo() {
        int spawnY = 0;
        for (int offset = 0; offset < spawnXs.length; offset++) {
            int slot = (nextSpawnSlot + offset) % spawnXs.length;
            if (slotBloqueado[slot]) {
                continue;
            }
            int x = spawnXs[slot];
            if (hayEntidadEn(x, spawnY)) {
                continue;
            }
            nextSpawnSlot = (slot + 1) % spawnXs.length;
            TipoEnemigo tipo = seleccionarTipo();
            return tipo.crear(x, spawnY);
        }
        return null;
    }

    private int[] calcularSpawnXs(int width) {
        int maxX = Math.max(0, width - ENEMY_SIZE);
        LinkedHashSet<Integer> puntos = new LinkedHashSet<>();
        puntos.add(0);
        puntos.add(Math.max(0, (width - ENEMY_SIZE) / 2));
        puntos.add(maxX);
        return puntos.stream().mapToInt(Integer::intValue).toArray();
    }

    private boolean[] calcularSlotsBloqueados() {
        boolean[] bloqueados = new boolean[spawnXs.length];
        for (int i = 0; i < spawnXs.length; i++) {
            int x = spawnXs[i];
            bloqueados[i] = existeBloqueSolido(x);
        }
        return bloqueados;
    }

    private boolean existeBloqueSolido(int x) {
        ValidadorMovimiento validador = new ValidadorMovimiento();
        for (Bloque bloque : nivel.bloques()) {
            if (bloque.esAtravesable()) {
                continue;
            }
            if (validador.seSuperpone(x, 0, bloque.getX(), bloque.getY())) {
                return true;
            }
        }
        return false;
    }

    private boolean hayEntidadEn(int x, int y) {
        return validador.hayTanqueEn(x, y, nivel);
    }

    private TipoEnemigo seleccionarTipo() {
        TipoEnemigo[] tipos = TipoEnemigo.values();
        return tipos[random.nextInt(tipos.length)];
    }
}
