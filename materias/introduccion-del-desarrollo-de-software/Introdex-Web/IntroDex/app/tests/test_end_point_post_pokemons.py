from app.main import app
from fastapi.testclient import TestClient

client = TestClient(app)

model_pokemon_no_existente = {
    "nuevo_pokemon": {
        "imagen": "string",
        "nombre": "pkm_prueba",
        "altura": 160,
        "peso": 10,
        "especie": 1092019,
        "generacion": 1,
        "id_evolucion": 0,
        "imagen_evolucion": "string",
        "id": 1000,  # Pokemon no existente
    },
    "nuevos_movimientos": [{"move_id": 54, "id_metodo": 1, "nivel": 0}],
    "nuevos_tipos": [{"type_id": 5}, {"type_id": 7}],
    "nuevas_habilidades": [{"ability_id": 77, "es_oculta": False}],
    "nuevas_especies": [{"egg_group_id": 15}],
    "nuevas_stats": [
        {"stat_id": 1, "base_stat": 50},
        {"stat_id": 2, "base_stat": 50},
        {"stat_id": 3, "base_stat": 50},
        {"stat_id": 4, "base_stat": 50},
        {"stat_id": 5, "base_stat": 50},
        {"stat_id": 6, "base_stat": 50},
    ],
}


def test_crear_pokemon_existente():
    model_pokemon_existente = model_pokemon_no_existente
    model_pokemon_existente["nuevo_pokemon"]["id"] = 25  # Es Pikachu
    response = client.post("/pokemons", json=model_pokemon_existente)
    assert response.status_code == 400
    assert response.json() == {"detail": "El Pokemon ya existe"}


def test_crear_pokemon_no_existente():
    model_pokemon_no_existente["nuevo_pokemon"]["id"] = 1000
    response = client.post("/pokemons", json=model_pokemon_no_existente)
    client.delete(
        f"/pokemons/delete/{model_pokemon_no_existente['nuevo_pokemon']['id']}"
    )
    assert response.status_code == 201
    assert response.json()["nombre"] == "pkm_prueba"
    assert response.json()["tipos"][0]["tipos"]["nombre"] == "Tierra"
    assert response.json()["habilidades"][0]["habilidades"]["nombre"] == "Tumbos"
    assert response.json()["habilidades"][0]["es_oculta"] == False
    assert response.json()["grupo_huevo"][0]["grupo_huevo"]["nombre"] == "Desconocido"


def test_crear_pokemon_no_existente_sin_movimientos():
    test_model = model_pokemon_no_existente
    test_model["nuevos_movimientos"] = []
    response = client.post("/pokemons", json=test_model)
    assert response.status_code == 400
    assert response.json() == {"detail": "El Pokemon debe tener al menos un movimiento"}
    model_pokemon_no_existente["nuevos_movimientos"] = [
        {"move_id": 54, "id_metodo": 1, "nivel": 0}
    ]


def test_crear_pokemon_no_existente_con_movimiento_inexistente():
    model_pokemon_no_existente["nuevos_movimientos"][0]["move_id"] = 1000
    response = client.post("/pokemons", json=model_pokemon_no_existente)
    assert response.status_code == 400
    assert response.json() == {
        "detail": "El movimiento no existe (o no se ha asignado correctamente)"
    }
    model_pokemon_no_existente["nuevos_movimientos"][0]["move_id"] = 54


def test_crear_pokemon_no_existente_metodo_movimiento_inexistente():
    model_pokemon_no_existente["nuevos_movimientos"][0]["id_metodo"] = 1000
    response = client.post("/pokemons", json=model_pokemon_no_existente)
    assert response.status_code == 400
    assert response.json() == {"detail": "El método de aprendizaje no es válido"}
    model_pokemon_no_existente["nuevos_movimientos"][0]["id_metodo"] = 1


def test_crear_pokemon_no_existente_con_movimientos_repetidos():
    model_pokemon_no_existente["nuevos_movimientos"].append(
        model_pokemon_no_existente["nuevos_movimientos"][0]
    )
    response = client.post("/pokemons", json=model_pokemon_no_existente)
    assert response.status_code == 400
    assert response.json() == {"detail": "No se pueden repetir los movimientos"}
    model_pokemon_no_existente["nuevos_movimientos"].pop()


def test_crear_pokemon_no_existente_sin_tipos():
    test_model = model_pokemon_no_existente
    test_model["nuevos_tipos"] = []
    response = client.post("/pokemons", json=test_model)
    assert response.status_code == 400
    assert response.json() == {"detail": "El Pokemon debe tener al menos un tipo"}
    model_pokemon_no_existente["nuevos_tipos"] = [{"type_id": 5}, {"type_id": 7}]


def test_crear_pokemon_no_existente_con_mas_de_dos_tipos():
    model_pokemon_no_existente["nuevos_tipos"].append(
        model_pokemon_no_existente["nuevos_tipos"][0]
    )
    response = client.post("/pokemons", json=model_pokemon_no_existente)
    assert response.status_code == 400
    assert response.json() == {"detail": "El Pokemon no puede tener más de dos tipos"}
    model_pokemon_no_existente["nuevos_tipos"].pop()


def test_crear_pokemon_no_existente_con_tipo_inexistente():
    model_pokemon_no_existente["nuevos_tipos"][0]["type_id"] = 1000
    response = client.post("/pokemons", json=model_pokemon_no_existente)
    assert response.status_code == 400
    assert response.json() == {
        "detail": "El tipo no existe (o no se ha asignado correctamente)"
    }
    model_pokemon_no_existente["nuevos_tipos"][0]["type_id"] = 5


def test_crear_pokemon_no_existente_con_tipos_repetidos():
    model_pokemon_no_existente["nuevos_tipos"] = [{"type_id": 5}, {"type_id": 5}]
    response = client.post("/pokemons", json=model_pokemon_no_existente)
    assert response.status_code == 400
    assert response.json() == {"detail": "No se pueden repetir los tipos"}
    model_pokemon_no_existente["nuevos_tipos"] = [{"type_id": 5}, {"type_id": 7}]


def test_crear_pokemon_no_existente_sin_habilidades():
    model_pokemon_no_existente["nuevas_habilidades"] = []
    response = client.post("/pokemons", json=model_pokemon_no_existente)
    assert response.status_code == 400
    assert response.json() == {"detail": "El Pokemon debe tener al menos una habilidad"}
    model_pokemon_no_existente["nuevas_habilidades"] = [
        {"ability_id": 77, "es_oculta": False}
    ]


def test_crear_pokemon_no_existente_con_habilidad_inexistente():
    model_pokemon_no_existente["nuevas_habilidades"][0]["ability_id"] = 1000
    response = client.post("/pokemons", json=model_pokemon_no_existente)
    assert response.status_code == 400
    assert response.json() == {
        "detail": "La habilidad no existe (o no se ha asignado correctamente)"
    }
    model_pokemon_no_existente["nuevas_habilidades"][0]["ability_id"] = 77


def test_crear_pokemon_no_existente_con_habilidades_repetidas():
    model_pokemon_no_existente["nuevas_habilidades"].append(
        model_pokemon_no_existente["nuevas_habilidades"][0]
    )
    response = client.post("/pokemons", json=model_pokemon_no_existente)
    assert response.status_code == 400
    assert response.json() == {"detail": "No se pueden repetir las habilidades"}
    model_pokemon_no_existente["nuevas_habilidades"].pop()


def test_crear_pokemon_no_existente_sin_grupo_huevo():
    model_pokemon_no_existente["nuevas_especies"] = []
    response = client.post("/pokemons", json=model_pokemon_no_existente)
    assert response.status_code == 400
    assert response.json() == {
        "detail": "El Pokemon debe tener al menos un grupo huevo"
    }
    model_pokemon_no_existente["nuevas_especies"] = [{"egg_group_id": 15}]


def test_crear_pokemon_no_existente_con_grupo_huevo_inexistente():
    model_pokemon_no_existente["nuevas_especies"][0]["egg_group_id"] = 1000
    response = client.post("/pokemons", json=model_pokemon_no_existente)
    assert response.status_code == 400
    assert response.json() == {
        "detail": "El grupo huevo no existe (o no se ha asignado correctamente)"
    }
    model_pokemon_no_existente["nuevas_especies"][0]["egg_group_id"] = 15


def test_crear_pokemon_no_existente_con_grupos_huevo_repetidos():
    model_pokemon_no_existente["nuevas_especies"].append(
        model_pokemon_no_existente["nuevas_especies"][0]
    )
    response = client.post("/pokemons", json=model_pokemon_no_existente)
    assert response.status_code == 400
    assert response.json() == {"detail": "No se pueden repetir los grupos huevo"}
    model_pokemon_no_existente["nuevas_especies"].pop()


def test_crear_pokemon_no_existente_sin_stats():
    model_pokemon_no_existente["nuevas_stats"] = []
    response = client.post("/pokemons", json=model_pokemon_no_existente)
    assert response.status_code == 400
    assert response.json() == {"detail": "El Pokemon debe tener 6 stats"}
    model_pokemon_no_existente["nuevas_stats"] = [
        {"stat_id": 1, "base_stat": 50},
        {"stat_id": 2, "base_stat": 50},
        {"stat_id": 3, "base_stat": 50},
        {"stat_id": 4, "base_stat": 50},
        {"stat_id": 5, "base_stat": 50},
        {"stat_id": 6, "base_stat": 50},
    ]


def test_crear_pokemon_no_existente_con_stats_de_mas():
    model_pokemon_no_existente["nuevas_stats"].append(
        model_pokemon_no_existente["nuevas_stats"][0]
    )
    response = client.post("/pokemons", json=model_pokemon_no_existente)
    assert response.status_code == 400
    assert response.json() == {"detail": "El Pokemon debe tener 6 stats"}
    model_pokemon_no_existente["nuevas_stats"].pop()


def test_crear_pokemon_no_existente_con_stats_repetidos():
    model_pokemon_no_existente["nuevas_stats"][0]["stat_id"] = 2
    response = client.post("/pokemons", json=model_pokemon_no_existente)
    assert response.status_code == 400
    assert response.json() == {"detail": "No se pueden repetir los stats"}
    model_pokemon_no_existente["nuevas_stats"][0]["stat_id"] = 1


def test_crear_pokemon_no_existente_con_stat_inexistente():
    model_pokemon_no_existente["nuevas_stats"][0]["stat_id"] = 1000
    response = client.post("/pokemons", json=model_pokemon_no_existente)
    assert response.status_code == 400
    assert response.json() == {
        "detail": "El stat no existe (o no se ha asignado correctamente, asegúrate de que los ids de los stats sean del 1 al 6)"
    }
    model_pokemon_no_existente["nuevas_stats"][0]["stat_id"] = 1


def test_crear_pokemon_no_existente_con_stat_invalido():
    model_pokemon_no_existente["nuevas_stats"][0]["base_stat"] = -1
    response = client.post("/pokemons", json=model_pokemon_no_existente)
    assert response.status_code == 400
    assert response.json() == {"detail": "El stat no puede ser negativo"}
    model_pokemon_no_existente["nuevas_stats"][0]["base_stat"] = 50
