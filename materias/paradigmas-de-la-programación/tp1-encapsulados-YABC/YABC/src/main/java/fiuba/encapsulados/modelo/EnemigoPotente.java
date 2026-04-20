package fiuba.encapsulados.modelo;

import java.util.List;

public class EnemigoPotente extends Enemigo {
    public EnemigoPotente(int x, int y) {
        super(x, y, 2, 1);
    }

    @Override
    public void disparar(List<Disparo> disparos) {
        int disparoX = getX() + 10; // Ajusta para centrar el disparo
        int disparoY = getY() + 10;
        Disparo nuevoDisparo = new Disparo(disparoX, disparoY, 10, getDireccion(), this, true);
        disparos.add(nuevoDisparo);
    }
}
