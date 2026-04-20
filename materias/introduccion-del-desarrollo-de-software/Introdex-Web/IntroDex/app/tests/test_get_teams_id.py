from fastapi.testclient import TestClient
from app.main import app

client = TestClient(app)


def test_team_id_existe():
    response = client.get("/equipos/id/1")
    assert response.status_code == 200

    data = response.json()
    integrantes = data["integrantes"]

    assert 1 <= len(integrantes) <= 6


def test_team_id_inexistente():
    response = client.get("/equipos/id/111")
    assert response.status_code == 404
