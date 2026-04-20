package fiuba.encapsulados.modelo;

public interface Colision {
    void recibirDisparo(Disparo disparo, GameManager gameManager, ServicioSonido sonidos);

    int getX();

    int getY();
}