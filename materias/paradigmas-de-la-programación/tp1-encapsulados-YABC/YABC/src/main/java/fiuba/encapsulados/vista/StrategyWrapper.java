package fiuba.encapsulados.vista;

import fiuba.encapsulados.modelo.Posicion;

public class StrategyWrapper<T extends Posicion> {
    private final Class<T> tipo;
    private final LogicaAnimaciones.AnimationStrategy<? super T> estrategia;

    StrategyWrapper(Class<T> tipo, LogicaAnimaciones.AnimationStrategy<? super T> estrategia) {
        this.tipo = tipo;
        this.estrategia = estrategia;
    }

    CicloSprite ciclo(Posicion entidad) {
        return estrategia.ciclo(tipo.cast(entidad));
    }
}
