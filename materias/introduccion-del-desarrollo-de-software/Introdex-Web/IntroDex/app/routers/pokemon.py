from fastapi import APIRouter, HTTPException, status
from app.modelos import *
from sqlmodel import select
from app.database import SessionDep

router = APIRouter()


@router.get("/", response_model=list[PokemonPublic])
def get_pokemons(session: SessionDep) -> list[PokemonPublic]:
    pokemons = session.exec(select(Pokemon)).all()

    if not pokemons:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND, detail="No Pokémon found"
        )

    pokemons_public = []
    for pokemon in pokemons:
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
            tipos=tipos,
            habilidades=habilidades,
            grupo_huevo=grupo_huevo,
            stats=stats,
            evoluciones=evoluciones,
        )
        pokemons_public.append(pokemon_public)

    return pokemons_public


@router.get(
    "/id/{pokemon_id}",
    responses={status.HTTP_404_NOT_FOUND: {"model": Error}},
    response_model=PokemonPublic,
)
def show_pokemon_por_id(session: SessionDep, pokemon_id: int) -> PokemonPublic:
    pokemon = session.exec(select(Pokemon).where(Pokemon.id == pokemon_id)).first()

    if not pokemon:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND, detail="Pokemon not found"
        )

    tipos = session.exec(
        select(Tipos).join(TiposPokemon).where(TiposPokemon.pokemon_id == pokemon.id)
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

    grupo_huevo = session.exec(
        select(GrupoHuevo)
        .join(GrupoHuevoPokemon)
        .where(GrupoHuevoPokemon.species_id == pokemon.especie)
    ).all()

    stats = session.exec(
        select(StatsDelPokemon).where(StatsDelPokemon.pokemon_id == pokemon.id)
    ).all()

    movimientos = session.exec(
        select(MovimientosPokemon).where(MovimientosPokemon.pokemon_id == pokemon.id)
    ).all()

    evoluciones = session.exec(
        select(PokemonEvoluciones).where(PokemonEvoluciones.pokemon_id == pokemon.id)
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
        tipos=tipos,
        habilidades=habilidades,
        grupo_huevo=grupo_huevo,
        stats=stats,
        evoluciones=evoluciones,
        movimientos=movimientos,
    )

    if pokemon_public:
        return pokemon_public
    raise HTTPException(
        status_code=status.HTTP_404_NOT_FOUND, detail="Pokemon not found"
    )


@router.get(
    "/nombre/{nombre}",
    responses={status.HTTP_404_NOT_FOUND: {"model": Error}},
    response_model=PokemonPublic,
)
def show_pokemon_por_name(session: SessionDep, nombre: str) -> PokemonPublic:
    pokemon = session.exec(
        select(Pokemon).where(Pokemon.nombre == nombre.lower())
    ).first()

    if not pokemon:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND, detail="Pokemon not exist"
        )

    tipos = session.exec(
        select(Tipos).join(TiposPokemon).where(TiposPokemon.pokemon_id == pokemon.id)
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

    grupo_huevo = session.exec(
        select(GrupoHuevo)
        .join(GrupoHuevoPokemon)
        .where(GrupoHuevoPokemon.species_id == pokemon.especie)
    ).all()

    stats = session.exec(
        select(StatsDelPokemon).where(StatsDelPokemon.pokemon_id == pokemon.id)
    ).all()

    movimientos = session.exec(
        select(MovimientosPokemon).where(MovimientosPokemon.pokemon_id == pokemon.id)
    ).all()

    evoluciones = session.exec(
        select(PokemonEvoluciones).where(PokemonEvoluciones.pokemon_id == pokemon.id)
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
        tipos=tipos,
        habilidades=habilidades,
        grupo_huevo=grupo_huevo,
        stats=stats,
        movimientos=movimientos,
        evoluciones=evoluciones,
    )

    if pokemon_public:
        return pokemon_public
    raise HTTPException(
        status_code=status.HTTP_404_NOT_FOUND, detail="Pokemon not found"
    )


@router.post(
    "/", status_code=status.HTTP_201_CREATED, response_model=PokemonCreatePublic
)
def crear_pokemon(
    nuevo_pokemon: Pokemon,
    session: SessionDep,
    nuevos_movimientos: List[MovimientosPokemon] = [
        {"move_id": 0, "id_metodo": 1, "nivel": 0}
    ],
    nuevos_tipos: List[TiposPokemon] = [{"type_id": 0}, {"type_id": 0}],
    nuevas_habilidades: List[HabilidadesPokemon] = [
        {"ability_id": 0, "es_oculta": False}
    ],
    nuevas_especies: List[GrupoHuevoPokemon] = [{"egg_group_id": 0}],
    nuevas_stats: List[StatsDelPokemon] = [
        {"stat_id": 1, "base_stat": 0},
        {"stat_id": 2, "base_stat": 0},
        {"stat_id": 3, "base_stat": 0},
        {"stat_id": 4, "base_stat": 0},
        {"stat_id": 5, "base_stat": 0},
        {"stat_id": 6, "base_stat": 0},
    ],
):
    pokemon_coincidente = session.exec(
        select(Pokemon).where(Pokemon.id == nuevo_pokemon.id)
    ).first()
    if pokemon_coincidente:
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST, detail="El Pokemon ya existe"
        )
    verificar_nuevos_datos(
        nuevos_movimientos,
        nuevos_tipos,
        nuevas_habilidades,
        nuevas_stats,
        nuevas_especies,
        session,
    )
    asignar_nuevos_datos(
        nuevo_pokemon,
        nuevos_movimientos,
        nuevos_tipos,
        nuevas_habilidades,
        nuevas_stats,
        nuevas_especies,
        session,
    )
    flushear_en_sesion(nuevo_pokemon, session, "pokemon")
    session.commit()
    print("yep", nuevo_pokemon.stats)
    return nuevo_pokemon


def verificar_nuevos_datos(
    nuevos_movimientos: List[MovimientosPokemon],
    nuevos_tipos: List[TiposPokemon],
    nuevas_habilidades: List[HabilidadesPokemon],
    nuevas_stats: List[StatsDelPokemon],
    nuevas_especies: List[GrupoHuevoPokemon],
    session: SessionDep,
):
    if not nuevos_movimientos:
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="El Pokemon debe tener al menos un movimiento",
        )
    else:
        id_movimientos = []
        for movimiento in nuevos_movimientos:
            movimiento_coincidente = session.exec(
                select(Movimientos).where(Movimientos.id == movimiento.move_id)
            ).first()
            if not movimiento_coincidente:
                raise HTTPException(
                    status_code=status.HTTP_400_BAD_REQUEST,
                    detail="El movimiento no existe (o no se ha asignado correctamente)",
                )
            if movimiento.move_id in id_movimientos:
                raise HTTPException(
                    status_code=status.HTTP_400_BAD_REQUEST,
                    detail="No se pueden repetir los movimientos",
                )
            if (
                movimiento.id_metodo != 1
                and movimiento.id_metodo != 2
                and movimiento.id_metodo != 4
            ):
                raise HTTPException(
                    status_code=status.HTTP_400_BAD_REQUEST,
                    detail="El método de aprendizaje no es válido",
                )
            id_movimientos.append(movimiento.move_id)
    if not nuevos_tipos:
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="El Pokemon debe tener al menos un tipo",
        )
    elif len(nuevos_tipos) > 2:
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="El Pokemon no puede tener más de dos tipos",
        )
    else:
        id_tipos = []
        for tipo in nuevos_tipos:
            tipo_coincidente = session.exec(
                select(Tipos).where(Tipos.id == tipo.type_id)
            ).first()
            if not tipo_coincidente:
                raise HTTPException(
                    status_code=status.HTTP_400_BAD_REQUEST,
                    detail="El tipo no existe (o no se ha asignado correctamente)",
                )
            if tipo.type_id in id_tipos:
                raise HTTPException(
                    status_code=status.HTTP_400_BAD_REQUEST,
                    detail="No se pueden repetir los tipos",
                )
            id_tipos.append(tipo.type_id)
    if not nuevas_habilidades:
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="El Pokemon debe tener al menos una habilidad",
        )
    else:
        id_habilidades = []
        for habilidad in nuevas_habilidades:
            habilidad_coincidente = session.exec(
                select(HabilidadesPokemon).where(
                    HabilidadesPokemon.ability_id == habilidad.ability_id
                )
            ).first()
            if not habilidad_coincidente:
                raise HTTPException(
                    status_code=status.HTTP_400_BAD_REQUEST,
                    detail="La habilidad no existe (o no se ha asignado correctamente)",
                )
            if habilidad.ability_id in id_habilidades:
                raise HTTPException(
                    status_code=status.HTTP_400_BAD_REQUEST,
                    detail="No se pueden repetir las habilidades",
                )
            id_habilidades.append(habilidad.ability_id)
    if not nuevas_stats or len(nuevas_stats) != 6:
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="El Pokemon debe tener 6 stats",
        )
    else:
        id_stats = []
        for stat in nuevas_stats:
            stat_coincidente = session.exec(
                select(StatsDelPokemon).where(StatsDelPokemon.stat_id == stat.stat_id)
            ).first()
            if not stat_coincidente:
                raise HTTPException(
                    status_code=status.HTTP_400_BAD_REQUEST,
                    detail="El stat no existe (o no se ha asignado correctamente, asegúrate de que los ids de los stats sean del 1 al 6)",
                )
            if stat.base_stat < 0:
                raise HTTPException(
                    status_code=status.HTTP_400_BAD_REQUEST,
                    detail="El stat no puede ser negativo",
                )
            if stat.stat_id in id_stats:
                raise HTTPException(
                    status_code=status.HTTP_400_BAD_REQUEST,
                    detail="No se pueden repetir los stats",
                )
            id_stats.append(stat.stat_id)
    if not nuevas_especies:
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="El Pokemon debe tener al menos un grupo huevo",
        )
    else:
        id_especies = []
        for especie in nuevas_especies:
            especie_coincidente = session.exec(
                select(GrupoHuevo).where(GrupoHuevo.id == especie.egg_group_id)
            ).first()
            if not especie_coincidente:
                raise HTTPException(
                    status_code=status.HTTP_400_BAD_REQUEST,
                    detail="El grupo huevo no existe (o no se ha asignado correctamente)",
                )
            if especie.egg_group_id in id_especies:
                raise HTTPException(
                    status_code=status.HTTP_400_BAD_REQUEST,
                    detail="No se pueden repetir los grupos huevo",
                )
            id_especies.append(especie.egg_group_id)


def asignar_nuevos_datos(
    nuevo_pokemon: Pokemon,
    nuevos_movimientos: List[MovimientosPokemon],
    nuevos_tipos: List[TiposPokemon],
    nuevas_habilidades: List[HabilidadesPokemon],
    nuevas_stats: List[StatsDelPokemon],
    nuevas_especies: List[GrupoHuevoPokemon],
    session: SessionDep,
):
    for pkm_movimiento in nuevos_movimientos:
        pkm_movimiento.move_id = pkm_movimiento.move_id
        pkm_movimiento.pokemon_id = nuevo_pokemon.id
        pkm_movimiento.pokemon = nuevo_pokemon
        pkm_movimiento.movimientos = session.exec(
            select(Movimientos).where(Movimientos.id == pkm_movimiento.move_id)
        ).first()
        pkm_movimiento.metodo = session.exec(
            select(MetodoAprenderMovimiento).where(
                MetodoAprenderMovimiento.pokemon_move_method_id
                == pkm_movimiento.id_metodo
            )
        ).first()
        if pkm_movimiento.id_metodo != 1:
            pkm_movimiento.nivel = 0
        nuevo_pokemon.movimientos.append(pkm_movimiento)
        flushear_en_sesion(pkm_movimiento, session, "movimiento")
    for pkm_tipo in nuevos_tipos:
        pkm_tipo.pokemon_id = nuevo_pokemon.id
        pkm_tipo.pokemon = nuevo_pokemon
        pkm_tipo.tipos = session.exec(
            select(Tipos).where(Tipos.id == pkm_tipo.type_id)
        ).first()
        nuevo_pokemon.tipos.append(pkm_tipo)
        flushear_en_sesion(pkm_tipo, session, "tipo")
    for pkm_habilidad in nuevas_habilidades:
        pkm_habilidad.pokemon_id = nuevo_pokemon.id
        pkm_habilidad.pokemon = nuevo_pokemon
        pkm_habilidad.habilidades = session.exec(
            select(Habilidades).where(Habilidades.id == pkm_habilidad.ability_id)
        ).first()
        nuevo_pokemon.habilidades.append(pkm_habilidad)
        flushear_en_sesion(pkm_habilidad, session, "habilidad")
    for pkm_stat in nuevas_stats:
        pkm_stat.pokemon_id = nuevo_pokemon.id
        pkm_stat.pokemon = nuevo_pokemon
        query = select(Stats).where(Stats.id == pkm_stat.stat_id)
        pkm_stat.stats = session.exec(query).first()
        nuevo_pokemon.stats.append(pkm_stat)
        flushear_en_sesion(pkm_stat, session, "stat")
    for pkm_especie in nuevas_especies:
        pkm_especie.species_id = nuevo_pokemon.id
        pkm_especie.pokemon = nuevo_pokemon
        pkm_especie.grupo_huevo = session.exec(
            select(GrupoHuevo).where(GrupoHuevo.id == pkm_especie.egg_group_id)
        ).first()
        nuevo_pokemon.grupo_huevo.append(pkm_especie)
        flushear_en_sesion(pkm_especie, session, "especie")


def flushear_en_sesion(nuevo_dato, session: SessionDep, text_nuevo_dato: str):
    try:
        print(text_nuevo_dato, nuevo_dato)
        session.add(nuevo_dato)
        session.flush()
        session.refresh(nuevo_dato)
    except Exception as e:
        session.rollback()
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail=f"Error al asignar {text_nuevo_dato} en la base de datos",
        )


@router.delete("/delete/{id}", responses={status.HTTP_404_NOT_FOUND: {"model": Error}})
def get_pokemon(id: int, session: SessionDep) -> Pokemon:
    pokemon = session.exec(select(Pokemon).where(Pokemon.id == id)).first()
    if not pokemon:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND, detail="Pokemon not found"
        )
    pkm_tipo = session.exec(
        select(TiposPokemon).where(TiposPokemon.pokemon_id == pokemon.id)
    ).all()
    for tipo in pkm_tipo:
        pkm_tipo_debilidades = session.exec(
            select(DebilidadesTipo).where(
                DebilidadesTipo.damage_type_id == tipo.type_id
            )
        ).all()
        for debilidad in pkm_tipo_debilidades:
            session.delete(debilidad)
        session.delete(tipo)
    pkm_habilidad = session.exec(
        select(HabilidadesPokemon).where(HabilidadesPokemon.pokemon_id == pokemon.id)
    ).all()
    for habilidad in pkm_habilidad:
        session.delete(habilidad)
    pkm_movimiento = session.exec(
        select(MovimientosPokemon).where(MovimientosPokemon.pokemon_id == pokemon.id)
    ).all()
    for movimiento in pkm_movimiento:
        session.delete(movimiento)
    pkm_stat = session.exec(
        select(StatsDelPokemon).where(StatsDelPokemon.pokemon_id == pokemon.id)
    ).all()
    for stat in pkm_stat:
        session.delete(stat)
    pkm_especie = session.exec(
        select(GrupoHuevoPokemon).where(GrupoHuevoPokemon.species_id == pokemon.especie)
    ).all()
    for especie in pkm_especie:
        session.delete(especie)
    pkm_evolucion = session.exec(
        select(PokemonEvoluciones).where(PokemonEvoluciones.pokemon_id == pokemon.id)
    ).all()
    for evolucion in pkm_evolucion:
        session.delete(evolucion)
    session.delete(pokemon)
    session.commit()
    return pokemon


@router.get("/{pokemon_id}/movimientos", response_model=list[MovimientosPokemonPublic])
def obtener_movimientos_del_pokemon(
    pokemon_id: int, session: SessionDep
) -> list[MovimientosPokemon]:
    verificar_pkm(pokemon_id, session)
    pokemon_movimientos = session.exec(
        select(MovimientosPokemon).where(MovimientosPokemon.pokemon_id == pokemon_id)
    ).all()
    if not pokemon_movimientos:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="El pokemon no tiene movimientos.",
        )
    for pokemon_movimiento in pokemon_movimientos:
        pokemon_movimiento.movimientos = session.exec(
            select(Movimientos).where(Movimientos.id == pokemon_movimiento.move_id)
        ).first()
        pokemon_movimiento.movimientos.class_tipo = session.exec(
            select(Tipos).where(Tipos.id == pokemon_movimiento.movimientos.tipo)
        ).first()
        pokemon_movimiento.movimientos.class_categoria = session.exec(
            select(CategoriaMovimiento)
            .join(Movimientos)
            .where(CategoriaMovimiento.id == pokemon_movimiento.movimientos.categoria)
        ).first()
        pokemon_movimiento.movimientos.class_efecto = session.exec(
            select(EfectoMovimiento)
            .join(Movimientos)
            .where(EfectoMovimiento.id == pokemon_movimiento.movimientos.efecto)
        ).first()
        pokemon_movimiento.metodo = session.exec(
            select(MetodoAprenderMovimiento).where(
                MetodoAprenderMovimiento.pokemon_move_method_id
                == pokemon_movimiento.id_metodo
            )
        ).first()
    return pokemon_movimientos


def verificar_pkm(pokemon_id: int, session: SessionDep) -> bool:
    pokemon = session.exec(select(Pokemon).where(Pokemon.id == pokemon_id)).first()
    if not pokemon:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Pokemon no encontrado.",
        )
    return True
