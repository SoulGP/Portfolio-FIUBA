package fiuba.encapsulados.modelo;

import java.util.Iterator;
import java.util.List;

public record ActualizadorDisparos(List<Disparo> disparos, GameManager gameManager,
                                   GestorColisiones gestorColisiones) implements Actualizador {

    @Override
    public void actualizar(ServicioSonido sonidos) {
        for (Disparo disparo : disparos) {
            if (!disparo.estaActivo()) continue;
            disparo.update();
            if (fueraDeLimites(disparo)) {
                disparo.die();
            }
        }
        gestorColisiones.detectarColisiones(disparos, gameManager.getNivel(), gameManager);
        removerDisparosInactivos();
    }

    private boolean fueraDeLimites(Posicion entidad) {
        return entidad.getX() < 0 || entidad.getX() > gameManager.getNivel().width()
                || entidad.getY() < 0 || entidad.getY() > gameManager.getNivel().height();
    }

    private void removerDisparosInactivos() {
        Iterator<Disparo> iterator = disparos.iterator();
        while (iterator.hasNext()) {
            Disparo disparo = iterator.next();
            if (!disparo.estaActivo()) {
                Entidad propietario = disparo.getPropietario();
                propietario.disparoDestruido();
                iterator.remove();
            }
        }
    }
}

