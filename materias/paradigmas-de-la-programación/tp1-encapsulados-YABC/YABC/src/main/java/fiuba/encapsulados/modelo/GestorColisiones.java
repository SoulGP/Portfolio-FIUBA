package fiuba.encapsulados.modelo;

import java.util.ArrayList;
import java.util.List;

public class GestorColisiones {
    private static final int ANCHO_DISPARO = 5;
    private static final int ALTO_DISPARO = 5;
    private static final int ANCHO_OBJETIVO = 20;
    private static final int ALTO_OBJETIVO = 20;

    private final ServicioSonido sonidos;

    public GestorColisiones(ServicioSonido sonidos) {
        this.sonidos = sonidos;
    }

    public void detectarColisiones(List<Disparo> disparos, InfoNivel nivel, GameManager gameManager) {
        detectarColisionesEntreDisparos(disparos);
        List<Colision> objetivos = obtenerObjetivos(nivel);
        verificarColisionesConObjetivos(disparos, objetivos, gameManager);
    }

    private List<Colision> obtenerObjetivos(InfoNivel nivel) {
        List<Colision> objetivos = new ArrayList<>();
        objetivos.addAll(nivel.enemigos());
        objetivos.addAll(nivel.bloques());
        objetivos.addAll(nivel.jugadores());
        return objetivos;
    }

    private void detectarColisionesEntreDisparos(List<Disparo> disparos) {
        for (int i = 0; i < disparos.size(); i++) {
            Disparo disparo1 = disparos.get(i);
            if (!disparo1.estaActivo()) continue;

            for (int j = i + 1; j < disparos.size(); j++) {
                Disparo disparo2 = disparos.get(j);
                if (!disparo2.estaActivo()) continue;

                if (sonDePropietariosDiferentes(disparo1, disparo2)
                        && colisiona(disparo1.getX(), disparo1.getY(), disparo2.getX(), disparo2.getY(), ANCHO_DISPARO, ALTO_DISPARO)) {
                    disparo1.die();
                    disparo2.die();
                }
            }
        }
    }

    private boolean sonDePropietariosDiferentes(Disparo d1, Disparo d2) {
        return d1.getPropietario() != d2.getPropietario();
    }

    private void verificarColisionesConObjetivos(List<Disparo> disparos, List<Colision> objetivos, GameManager gameManager) {
        for (Disparo disparo : disparos) {
            if (!disparo.estaActivo()) continue;
            for (Colision obj : objetivos) {
                if (obj == disparo.getPropietario()) continue;
                if (colisiona(disparo.getX(), disparo.getY(), obj.getX(), obj.getY(), ANCHO_OBJETIVO, ALTO_OBJETIVO)) {
                    obj.recibirDisparo(disparo, gameManager, sonidos);
                }
            }
        }
    }

    private boolean colisiona(int dx, int dy, int ox, int oy, int ancho, int alto) {
        return dx < ox + ancho &&
                dx + ANCHO_DISPARO > ox &&
                dy < oy + alto &&
                dy + ALTO_DISPARO > oy;
    }
}

