from fastapi import APIRouter, HTTPException, status
from app.modelos import *
from sqlmodel import select
from app.database import SessionDep
from typing import List

router = APIRouter()


@router.get("/", response_model=List[MovimientosPublic])
def obtener_movimientos(session: SessionDep):
    query = select(Movimientos)
    movimientos = session.exec(query).all()
    if not movimientos:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Movimientos not found",
        )
    return movimientos


def verificar_move_id(move_id: int, session: SessionDep) -> bool:
    movimiento = session.exec(
        select(Movimientos).where(Movimientos.id == move_id)
    ).first()
    if not movimiento:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Movimiento not found",
        )
    return True


@router.get(
    "/{move_id}/pokemon",
    responses={status.HTTP_404_NOT_FOUND: {"model": Error}},
    response_model=List[PokemonPublic],
)
def show_pokemones_por_id(session: SessionDep, move_id: int) -> List[PokemonPublic]:
    verificar_move_id(move_id, session)
    movimientos_pokemon = session.exec(
        select(MovimientosPokemon).where(MovimientosPokemon.move_id == move_id)
    ).all()

    pokemones = []

    for movimiento_pokemon in movimientos_pokemon:
        pokemon = session.exec(
            select(Pokemon).where(Pokemon.id == movimiento_pokemon.pokemon_id)
        ).first()

        if pokemon:
            tipos = session.exec(
                select(Tipos)
                .join(TiposPokemon)
                .where(TiposPokemon.pokemon_id == pokemon.id)
            ).all()

            habilidades = session.exec(
                select(Habilidades)
                .join(HabilidadesPokemon)
                .where(HabilidadesPokemon.pokemon_id == pokemon.id)
            ).all()

            grupo_huevo = session.exec(
                select(GrupoHuevo)
                .join(GrupoHuevoPokemon)
                .where(GrupoHuevoPokemon.species_id == pokemon.especie)
            ).all()

            stats = session.exec(
                select(StatsDelPokemon).where(StatsDelPokemon.pokemon_id == pokemon.id)
            ).all()

            movimiento: List[MovimientosPokemon] = []
            for movimiento_pokemon in movimientos_pokemon:
                if movimiento_pokemon.pokemon_id == pokemon.id:
                    movimiento.append(movimiento_pokemon)

            evoluciones = session.exec(
                select(PokemonEvoluciones).where(
                    PokemonEvoluciones.pokemon_id == pokemon.id
                )
            ).all()

            for evolucion in evoluciones:
                evolucion.pokemon = session.exec(
                    select(Pokemon).where(Pokemon.id == evolucion.evolution_id)
                ).first()

            pokemon_public = PokemonPublic(
                id=pokemon.id,
                nombre=pokemon.nombre,
                imagen=pokemon.imagen,
                altura=pokemon.altura,
                peso=pokemon.peso,
                generacion=pokemon.generacion,
                evoluciones=evoluciones,
                tipos=tipos,
                habilidades=habilidades,
                grupo_huevo=grupo_huevo,
                stats=stats,
                movimientos=movimiento,
            )

            pokemones.append(pokemon_public)

    if not pokemones:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="No Pokémon were found that learn that movement through pre-established methods",
        )

    return pokemones


@router.get(
    "/id/{move_id}",
    responses={status.HTTP_404_NOT_FOUND: {"model": Error}},
    response_model=MovimientosPublic,
)
def show_movimiento_por_id(session: SessionDep, move_id: int) -> MovimientosPublic:
    movimiento = session.exec(
        select(Movimientos).where(Movimientos.id == move_id)
    ).first()

    if not movimiento:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND, detail="Movimiento not found"
        )

    movimiento_public = MovimientosPublic(
        id=movimiento.id,
        nombre=movimiento.nombre,
        class_tipo=movimiento.class_tipo,
        class_categoria=movimiento.class_categoria
        or CategoriaMovimiento(nombre="desconocido"),
        potencia=movimiento.potencia,
        precision=movimiento.precision,
        usos=movimiento.usos,
        generacion=movimiento.generacion,
        class_efecto=movimiento.class_efecto
        or EfectoMovimiento(descripcion="Sin efecto"),
    )

    if movimiento_public:
        return movimiento_public
    raise HTTPException(
        status_code=status.HTTP_404_NOT_FOUND, detail="Movimiento not found"
    )


@router.get(
    "/nombre/{nombre}",
    responses={status.HTTP_404_NOT_FOUND: {"model": Error}},
    response_model=MovimientosPublic,
)
def show_movimiento_por_nombre(session: SessionDep, nombre: str) -> MovimientosPublic:
    movimiento = session.exec(
        select(Movimientos).where(Movimientos.nombre == nombre)
    ).first()

    if not movimiento:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND, detail="Movimiento not found"
        )

    movimiento_public = MovimientosPublic(
        id=movimiento.id,
        nombre=movimiento.nombre,
        class_tipo=movimiento.class_tipo,
        class_categoria=movimiento.class_categoria
        or CategoriaMovimiento(nombre="desconocido"),
        potencia=movimiento.potencia,
        precision=movimiento.precision,
        usos=movimiento.usos,
        generacion=movimiento.generacion,
        class_efecto=movimiento.class_efecto
        or EfectoMovimiento(descripcion="Sin efecto"),
    )

    if movimiento_public:
        return movimiento_public
    raise HTTPException(
        status_code=status.HTTP_404_NOT_FOUND, detail="Movimiento not found"
    )
