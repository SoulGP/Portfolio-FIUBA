import pytest
from fastapi.testclient import TestClient
from sqlmodel import Session, SQLModel, create_engine
from sqlmodel.pool import StaticPool

from app.main import app
from app.database import get_session
from app.modelos import *


@pytest.fixture(name="db_session")
def db_session_fixture():
    engine = create_engine(
        "sqlite://",
        connect_args={"check_same_thread": False},
        poolclass=StaticPool,
    )
    SQLModel.metadata.create_all(engine)
    with Session(engine) as session:
        tipo_normal = Tipos(type_id=1, nombre="Normal")

        habilidad_flexibilidad = Habilidades(id=7, nombre="Flexibilidad")
        habilidad_impostor = Habilidades(id=150, nombre="Impostor")

        movimiento_transformacion = Movimientos(
            id=144,
            nombre="Transformación",
            tipo=1,
            categoria=1,
            potencia=None,
            precision=None,
            usos=10,
            generacion=1,
            efecto=58,
        )
        movimiento_mofa = Movimientos(
            id=269,
            nombre="Mofa",
            tipo=12,
            categoria=3,
            potencia=None,
            precision=100,
            usos=20,
            generacion=3,
            efecto=58,
        )

        metodo_aprender_movimiento = MetodoAprenderMovimiento(id=1, nombre="Nivel")

        stat_vida = Stats(id=1, nombre="Vida")
        stat_ataque = Stats(id=2, nombre="Ataque")
        stat_defensa = Stats(id=3, nombre="Defensa")
        stat_ataque_especial = Stats(id=4, nombre="Ataque Especial")
        stat_defensa_especial = Stats(id=5, nombre="Defensa Especial")
        stat_velocidad = Stats(id=6, nombre="Velocidad")

        grupo_huevo_ditto = GrupoHuevo(id=13, nombre="Ditto")

        pokemon_ditto = Pokemon(
            id=132,
            imagen="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/132.png",
            nombre="ditto",
            altura=3,
            peso=40,
            especie=13,
            generacion=1,
            id_evolucion=None,
            imagen_evolucion="",
        )

        session.add_all(
            [
                tipo_normal,
                habilidad_flexibilidad,
                habilidad_impostor,
                movimiento_transformacion,
                movimiento_mofa,
                metodo_aprender_movimiento,
                stat_vida,
                stat_ataque,
                stat_defensa,
                stat_ataque_especial,
                stat_defensa_especial,
                stat_velocidad,
                grupo_huevo_ditto,
                pokemon_ditto,
            ]
        )
        session.commit()

        session.add_all(
            [
                TiposPokemon(type_id=1, pokemon_id=132),
            ]
        )
        session.commit()

        session.add_all(
            [
                HabilidadesPokemon(pokemon_id=132, ability_id=7, es_oculta=False),
                HabilidadesPokemon(pokemon_id=132, ability_id=150, es_oculta=True),
            ]
        )
        session.commit()

        session.add_all(
            [
                GrupoHuevoPokemon(species_id=13, egg_group_id=13),
            ]
        )
        session.commit()

        session.add_all(
            [
                MovimientosPokemon(pokemon_id=132, move_id=144, id_metodo=1, nivel=0),
            ]
        )
        session.commit()

        session.add_all(
            [
                StatsDelPokemon(pokemon_id=132, stat_id=1, base_stat=48),
                StatsDelPokemon(pokemon_id=132, stat_id=2, base_stat=48),
                StatsDelPokemon(pokemon_id=132, stat_id=3, base_stat=48),
                StatsDelPokemon(pokemon_id=132, stat_id=4, base_stat=48),
                StatsDelPokemon(pokemon_id=132, stat_id=5, base_stat=48),
                StatsDelPokemon(pokemon_id=132, stat_id=6, base_stat=48),
            ]
        )
        session.commit()

        yield session


@pytest.fixture(name="client")
def client_fixture(db_session: Session):
    def get_db_override():
        yield db_session

    app.dependency_overrides[get_session] = get_db_override

    with TestClient(app) as client:
        yield client

    app.dependency_overrides.clear()
