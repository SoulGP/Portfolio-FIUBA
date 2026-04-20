package fiuba.encapsulados.vista;

import fiuba.encapsulados.modelo.Enemigo;
import fiuba.encapsulados.modelo.Posicion;
import fiuba.encapsulados.modelo.TanqueJugador;
import javafx.scene.image.Image;

import java.util.HashMap;
import java.util.IdentityHashMap;
import java.util.Map;
import java.util.Objects;

class LogicaAnimaciones {
    private final Map<Class<?>, CicloSprite> animacionesPorClase = new HashMap<>();
    private final Map<String, CicloSprite> animacionesPorJugador = new HashMap<>();
    private final Map<Object, FrameState> estados = new IdentityHashMap<>();
    private final Map<Class<?>, StrategyWrapper<?>> estrategias = new HashMap<>();

    LogicaAnimaciones() {
        registrarEstrategias();
    }

    void registrarAnimacion(Class<?> tipo, Image... frames) {
        Objects.requireNonNull(tipo, "tipo");
        CicloSprite ciclo = crearCiclo(frames);
        if (ciclo != null) {
            animacionesPorClase.put(tipo, ciclo);
        }
    }

    void registrarAnimacionJugador(String jugadorId, Image... frames) {
        String llave = normalizarJugadorId(jugadorId);
        if (llave == null) {
            return;
        }
        CicloSprite ciclo = crearCiclo(frames);
        if (ciclo != null) {
            animacionesPorJugador.put(llave, ciclo);
        }
    }

    Image obtenerImagen(Posicion entidad) {
        if (entidad == null) {
            return null;
        }

        CicloSprite ciclo = cicloPara(entidad);
        if (ciclo == null) {
            return null;
        }

        return framePara(entidad, ciclo, entidad.getX(), entidad.getY());
    }

    private Image framePara(Object entidad, CicloSprite ciclo, int x, int y) {
        FrameState estado = estados.get(entidad);
        if (estado == null) {
            estados.put(entidad, new FrameState(x, y, 0));
            return ciclo.frame(0);
        }

        boolean seMovio = estado.x != x || estado.y != y;
        int indice = estado.frameIndex;
        if (seMovio) {
            indice = ciclo.next(indice);
        }
        estados.put(entidad, new FrameState(x, y, indice));
        return ciclo.frame(indice);
    }

    private CicloSprite crearCiclo(Image... frames) {
        if (frames == null || frames.length == 0) {
            return null;
        }
        return new CicloSprite(frames);
    }

    private CicloSprite cicloPara(Posicion entidad) {
        StrategyWrapper<?> wrapper = estrategiaPara(entidad.getClass());
        if (wrapper == null) {
            return null;
        }
        return wrapper.ciclo(entidad);
    }

    private CicloSprite cicloParaJugador(TanqueJugador jugador) {
        String llave = normalizarJugadorId(jugador.getNombre());
        CicloSprite ciclo = llave != null ? animacionesPorJugador.get(llave) : null;
        if (ciclo == null) {
            ciclo = animacionesPorClase.get(TanqueJugador.class);
        }
        return ciclo;
    }

    private StrategyWrapper<?> estrategiaPara(Class<?> tipo) {
        Class<?> actual = tipo;
        while (actual != null) {
            StrategyWrapper<?> wrapper = estrategias.get(actual);
            if (wrapper != null) {
                return wrapper;
            }
            actual = actual.getSuperclass();
        }
        return null;
    }

    private void registrarEstrategias() {
        registrarEstrategia(TanqueJugador.class, this::cicloParaJugador);
        registrarEstrategia(Enemigo.class, enemigo -> {
            CicloSprite ciclo = animacionesPorClase.get(enemigo.getClass());
            if (ciclo == null) {
                ciclo = animacionesPorClase.get(Enemigo.class);
            }
            return ciclo;
        });
        registrarEstrategia(Posicion.class, posicion -> animacionesPorClase.get(posicion.getClass()));
    }

    private <T extends Posicion> void registrarEstrategia(Class<T> tipo, AnimationStrategy<? super T> estrategia) {
        estrategias.put(tipo, new StrategyWrapper<>(tipo, estrategia));
    }

    private String normalizarJugadorId(String jugadorId) {
        if (jugadorId == null || jugadorId.isBlank()) {
            return null;
        }
        return jugadorId.toLowerCase();
    }

    interface AnimationStrategy<T extends Posicion> {
        CicloSprite ciclo(T entidad);
    }

    private record FrameState(int x, int y, int frameIndex) {
    }
}
