from fastapi.testclient import TestClient

from app.modelos import *


def test_obtener_movimiento_id_existente(client: TestClient) -> None:
    response = client.get("/movimientos/id/144")
    assert response.status_code == 200


def test_obtener_movimiento_nombre_existente(client: TestClient) -> None:
    response = client.get("/movimientos/nombre/Transformación")
    assert response.status_code == 200


def test_no_existe_movimiento(client: TestClient) -> None:
    response = client.get("/movimientos/id/9999999999999999")
    assert response.status_code == 404
    assert response.json()["detail"] == "Movimiento not found"

    response = client.get("/movimientos/nombre/dadafgadasdafasd")
    assert response.status_code == 404
    assert response.json()["detail"] == "Movimiento not found"


def test_verificar_datos_por_movimiento_id_existente(client: TestClient) -> None:
    response = client.get("/movimientos/id/144")

    data = response.json()
    assert data["nombre"] == "Transformación"


def test_verificar_datos_por_nombre_existente(client: TestClient) -> None:
    response = client.get("/movimientos/nombre/Transformación")
    assert response.status_code == 200

    data = response.json()
    assert data["id"] == 144


def test_show_movimiento_por_id_existente(client: TestClient) -> None:
    response = client.get("/movimientos/id/144")
    assert response.status_code == 200

    data = response.json()
    assert data["nombre"] == "Transformación"


def test_show_movimiento_por_id_no_existente(client: TestClient) -> None:
    response = client.get("/movimientos/id/9999")
    assert response.status_code == 404
    assert response.json()["detail"] == "Movimiento not found"


def test_show_movimiento_por_id_invalid_id(client: TestClient) -> None:
    response = client.get("/movimientos/id/abc")
    assert response.status_code == 422
