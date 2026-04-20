from fastapi.testclient import TestClient
from app.main import app

client = TestClient(app)


def test_obtener_naturalezas() -> None:
    response = client.get("/naturalezas")
    assert response.status_code == 200
    assert len(response.json()) == 25
