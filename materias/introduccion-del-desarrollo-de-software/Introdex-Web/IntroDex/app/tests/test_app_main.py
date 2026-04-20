from fastapi.testclient import TestClient
from app.main import app

client = TestClient(app)


def test_mensaje_bienvenida() -> None:
    response = client.get("/")
    assert response.status_code == 200
    assert response.json() == {
        "mensaje": "Bienvenido a la API de pokemon del Equipo Rocket!"
    }
