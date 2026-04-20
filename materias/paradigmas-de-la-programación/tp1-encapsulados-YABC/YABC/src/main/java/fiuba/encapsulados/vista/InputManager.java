package fiuba.encapsulados.vista;

import fiuba.encapsulados.modelo.*;
import javafx.scene.Scene;
import javafx.scene.input.KeyCode;

import java.util.*;

public class InputManager {
    private final Set<KeyCode> teclasPresionadas = EnumSet.noneOf(KeyCode.class);
    private final Map<TanqueJugador, Set<Entrada>> entradasPorJugador = new HashMap<>();
    private final List<Disparo> disparos;
    private final InfoNivel nivel;
    private final Map<TanqueJugador, Movimiento> orientaciones;
    private final Map<TanqueJugador, ControlesJugador> controlesPorJugador;

    private final Map<ControlesJugador, Map<KeyCode, Entrada>> mapeoTeclas;
    private final Map<ControlesJugador, KeyCode> teclaDisparo;

    public InputManager(List<Disparo> disparos, InfoNivel nivel, Map<TanqueJugador, Movimiento> orientaciones, Map<TanqueJugador, ControlesJugador> controlesPorJugador) {
        this.disparos = disparos;
        this.nivel = nivel;
        this.orientaciones = orientaciones;
        this.controlesPorJugador = controlesPorJugador;
        // Inicializa mapeos por instancia (evita estado global)
        Map<ControlesJugador, Map<KeyCode, Entrada>> mapeo = new EnumMap<>(ControlesJugador.class);
        Map<ControlesJugador, KeyCode> disparosMap = new EnumMap<>(ControlesJugador.class);

        mapeo.put(ControlesJugador.JUGADOR1, Map.of(
                KeyCode.W, Entrada.ARRIBA,
                KeyCode.S, Entrada.ABAJO,
                KeyCode.A, Entrada.IZQUIERDA,
                KeyCode.D, Entrada.DERECHA
        ));
        disparosMap.put(ControlesJugador.JUGADOR1, KeyCode.SPACE);

        mapeo.put(ControlesJugador.JUGADOR2, Map.of(
                KeyCode.UP, Entrada.ARRIBA,
                KeyCode.DOWN, Entrada.ABAJO,
                KeyCode.LEFT, Entrada.IZQUIERDA,
                KeyCode.RIGHT, Entrada.DERECHA
        ));
        disparosMap.put(ControlesJugador.JUGADOR2, KeyCode.ENTER);

        this.mapeoTeclas = Collections.unmodifiableMap(mapeo);
        this.teclaDisparo = Collections.unmodifiableMap(disparosMap);
        for (TanqueJugador jugador : nivel.jugadores()) {
            entradasPorJugador.put(jugador, EnumSet.noneOf(Entrada.class));
        }
    }

    public void configurarInput(Scene scene) {
        scene.setOnKeyPressed(event -> {
            KeyCode code = event.getCode();
            boolean nuevaPulsacion = teclasPresionadas.add(code);
            manejarMovimientoPresionado(code);
            if (nuevaPulsacion) {
                manejarDisparo(code);
            }
        });

        scene.setOnKeyReleased(event -> {
            teclasPresionadas.remove(event.getCode());
            manejarMovimientoLiberado(event.getCode());
        });
    }

    private void manejarMovimientoPresionado(KeyCode code) {
        for (TanqueJugador jugador : nivel.jugadores()) {
            ControlesJugador perfil = controlesPorJugador.get(jugador);
            if (perfil == null) continue;
            Entrada entrada = mapeoTeclas.getOrDefault(perfil, Map.of()).get(code);
            if (entrada != null && entrada != Entrada.DISPARO) {
                entradasPorJugador.get(jugador).add(entrada);
            }
        }
    }

    private void manejarMovimientoLiberado(KeyCode code) {
        for (TanqueJugador jugador : nivel.jugadores()) {
            ControlesJugador perfil = controlesPorJugador.get(jugador);
            if (perfil == null) continue;
            Entrada entrada = mapeoTeclas.getOrDefault(perfil, Map.of()).get(code);
            if (entrada != null && entrada != Entrada.DISPARO) {
                entradasPorJugador.get(jugador).remove(entrada);
            }
        }
    }

    private void manejarDisparo(KeyCode code) {
        List<TanqueJugador> jugadores = nivel.jugadores();
        for (TanqueJugador jugador : jugadores) {
            ControlesJugador perfil = controlesPorJugador.get(jugador);
            if (perfil == null) continue;
            KeyCode tecla = teclaDisparo.get(perfil);
            if (tecla != null && tecla == code) {
                Movimiento orientacion = orientaciones.getOrDefault(jugador, Movimiento.ARRIBA);
                Disparo disparo = jugador.disparar(orientacion);
                if (disparo != null) {
                    disparos.add(disparo);
                }
            }
        }
    }

    public Map<TanqueJugador, Set<Entrada>> getEntradasPorJugador() {
        return entradasPorJugador;
    }
}
