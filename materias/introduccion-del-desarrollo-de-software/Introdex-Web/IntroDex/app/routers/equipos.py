from fastapi import APIRouter, HTTPException, status
from app.modelos import *
from app.routers.pokemon import *
from typing import List
from sqlmodel import select
from app.database import SessionDep

router = APIRouter()


@router.get("/pagina/{pagina}", response_model=List[EquipoPublic])
def obtener_equipos(
    session: SessionDep, pagina: int, cantidad_equipos: int = 10
) -> list[Equipo]:
    if pagina < 1 or cantidad_equipos < 1:
        raise HTTPException(
            status_code=404,
            detail="Algunos de los parámetros están siendo mal introducidas",
        )

    skip = (pagina - 1) * cantidad_equipos
    query = select(Equipo).offset(skip).limit(cantidad_equipos)
    equipos_pagina = session.exec(query).all()
    if not equipos_pagina:
        raise HTTPException(
            status_code=404, detail="No se encontraron equipos para esta página"
        )

    equipos_public = []
    for equipo in equipos_pagina:
        integrantes_publicos: List[IntegrantesEquipoPublic] = []
        ids_integrantes = []
        for integrante in equipo.integrantes:
            if integrante.id not in ids_integrantes:
                query_moves_integrante = (
                    select(Movimientos)
                    .join(IntegrantesEquipo)
                    .where(
                        IntegrantesEquipo.id == integrante.id,
                        IntegrantesEquipo.move_id == Movimientos.id,
                    )
                )
                moves_integrantes = session.exec(query_moves_integrante).all()
                integrantes_publicos.append(
                    IntegrantesEquipoPublic(
                        pokemon=integrante.pokemon,
                        movimientos=moves_integrantes,
                        naturaleza=integrante.naturaleza,
                        evs=integrante.estadisticas,
                    )
                )
                ids_integrantes.append(integrante.id)

        equipo_publico = EquipoPublic(
            id=equipo.id,
            nombre=equipo.nombre,
            generacion=equipo.generacion,
            integrantes=integrantes_publicos,
        )

        equipos_public.append(equipo_publico)

    return equipos_public


@router.post("/", status_code=status.HTTP_201_CREATED, response_model=EquipoPublic)
def crear_equipo(
    id_equipo: int,
    nombre_equipo: str,
    generacion_equipo: int,
    session: SessionDep,
    id_pkm_1: int = None,
    id_movimientos_pkm_1: list[int] = None,
    id_naturaleza_1: int = None,
    evs_pkm_1: Estadisticas = Estadisticas(
        vida=0, ataque=0, defensa=0, ataque_especial=0, defensa_especial=0, velocidad=0
    ),
    id_pkm_2: int = None,
    id_movimientos_pkm_2: list[int] = None,
    id_naturaleza_2: int = None,
    evs_pkm_2: Estadisticas = Estadisticas(
        vida=0, ataque=0, defensa=0, ataque_especial=0, defensa_especial=0, velocidad=0
    ),
    id_pkm_3: int = None,
    id_movimientos_pkm_3: list[int] = None,
    id_naturaleza_3: int = None,
    evs_pkm_3: Estadisticas = Estadisticas(
        vida=0, ataque=0, defensa=0, ataque_especial=0, defensa_especial=0, velocidad=0
    ),
    id_pkm_4: int = None,
    id_movimientos_pkm_4: list[int] = None,
    id_naturaleza_4: int = None,
    evs_pkm_4: Estadisticas = Estadisticas(
        vida=0, ataque=0, defensa=0, ataque_especial=0, defensa_especial=0, velocidad=0
    ),
    id_pkm_5: int = None,
    id_movimientos_pkm_5: list[int] = None,
    id_naturaleza_5: int = None,
    evs_pkm_5: Estadisticas = Estadisticas(
        vida=0, ataque=0, defensa=0, ataque_especial=0, defensa_especial=0, velocidad=0
    ),
    id_pkm_6: int = None,
    id_movimientos_pkm_6: list[int] = None,
    id_naturaleza_6: int = None,
    evs_pkm_6: Estadisticas = Estadisticas(
        vida=0, ataque=0, defensa=0, ataque_especial=0, defensa_especial=0, velocidad=0
    ),
):

    query = select(Equipo).where(Equipo.id == id_equipo)
    equipo = session.exec(query).first()
    if equipo:
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST, detail="Ese equipo ya existe"
        )
    if generacion_equipo not in range(1, 9):
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="La generación del equipo no es válida",
        )

    nuevo_equipo = Equipo(
        id=id_equipo, nombre=nombre_equipo, generacion=generacion_equipo, integrantes=[]
    )

    if id_pkm_1 is not None:
        verificar_datos_integrantes(
            id_pkm_1,
            generacion_equipo,
            id_movimientos_pkm_1,
            evs_pkm_1,
            id_naturaleza_1,
            session,
        )
    if id_pkm_2 is not None:
        verificar_datos_integrantes(
            id_pkm_2,
            generacion_equipo,
            id_movimientos_pkm_2,
            evs_pkm_2,
            id_naturaleza_2,
            session,
        )
    if id_pkm_3 is not None:
        verificar_datos_integrantes(
            id_pkm_3,
            generacion_equipo,
            id_movimientos_pkm_3,
            evs_pkm_3,
            id_naturaleza_3,
            session,
        )
    if id_pkm_4 is not None:
        verificar_datos_integrantes(
            id_pkm_4,
            generacion_equipo,
            id_movimientos_pkm_4,
            evs_pkm_4,
            id_naturaleza_4,
            session,
        )
    if id_pkm_5 is not None:
        verificar_datos_integrantes(
            id_pkm_5,
            generacion_equipo,
            id_movimientos_pkm_5,
            evs_pkm_5,
            id_naturaleza_5,
            session,
        )
    if id_pkm_6 is not None:
        verificar_datos_integrantes(
            id_pkm_6,
            generacion_equipo,
            id_movimientos_pkm_6,
            evs_pkm_6,
            id_naturaleza_6,
            session,
        )

    if id_pkm_1 is not None:
        asignacion_datos_integrantes(
            id_pkm_1,
            id_movimientos_pkm_1,
            evs_pkm_1,
            id_naturaleza_1,
            nuevo_equipo,
            session,
        )
    if id_pkm_2 is not None:
        asignacion_datos_integrantes(
            id_pkm_2,
            id_movimientos_pkm_2,
            evs_pkm_2,
            id_naturaleza_2,
            nuevo_equipo,
            session,
        )
    if id_pkm_3 is not None:
        asignacion_datos_integrantes(
            id_pkm_3,
            id_movimientos_pkm_3,
            evs_pkm_3,
            id_naturaleza_3,
            nuevo_equipo,
            session,
        )
    if id_pkm_4 is not None:
        asignacion_datos_integrantes(
            id_pkm_4,
            id_movimientos_pkm_4,
            evs_pkm_4,
            id_naturaleza_4,
            nuevo_equipo,
            session,
        )
    if id_pkm_5 is not None:
        asignacion_datos_integrantes(
            id_pkm_5,
            id_movimientos_pkm_5,
            evs_pkm_5,
            id_naturaleza_5,
            nuevo_equipo,
            session,
        )
    if id_pkm_6 is not None:
        asignacion_datos_integrantes(
            id_pkm_6,
            id_movimientos_pkm_6,
            evs_pkm_6,
            id_naturaleza_6,
            nuevo_equipo,
            session,
        )

    session.add(nuevo_equipo)
    session.commit()
    session.refresh(nuevo_equipo)

    integrantes_publicos: List[IntegrantesEquipoPublic] = []
    ids_integrantes = []
    for integrante in nuevo_equipo.integrantes:
        if integrante.id not in ids_integrantes:
            query_moves_integrante = (
                select(Movimientos)
                .join(IntegrantesEquipo)
                .where(
                    IntegrantesEquipo.id == integrante.id,
                    IntegrantesEquipo.move_id == Movimientos.id,
                )
            )
            moves_integrantes = session.exec(query_moves_integrante).all()
            integrantes_publicos.append(
                IntegrantesEquipoPublic(
                    pokemon=integrante.pokemon,
                    movimientos=moves_integrantes,
                    naturaleza=integrante.naturaleza,
                    evs=integrante.estadisticas,
                )
            )
            ids_integrantes.append(integrante.id)

    equipo_publico = EquipoPublic(
        id=nuevo_equipo.id,
        nombre=nuevo_equipo.nombre,
        generacion=nuevo_equipo.generacion,
        integrantes=integrantes_publicos,
    )

    return equipo_publico


def asignacion_datos_integrantes(
    id_pokemon: int,
    id_movimientos_seleccionados: list[int],
    ptos_evs: Estadisticas,
    id_naturaleza: int,
    equipo_a_asignar: Equipo,
    session: SessionDep,
) -> None:
    nuevo_id = asignar_nueva_id_miembro(session)

    movimientos_elegidos: List["Movimientos"] = []
    for id_movimiento in id_movimientos_seleccionados:
        movimiento = buscar_movimientos_por_id(id_movimiento, session)
        movimientos_elegidos.append(movimiento)

    for movimiento_seleccionado, movimiento_elegido in zip(
        id_movimientos_seleccionados, movimientos_elegidos
    ):
        nuevo_integrante = IntegrantesEquipo(
            id=nuevo_id,
            pokemon_id=id_pokemon,
            pokemon=buscar_pokemon_por_id(id_pokemon, session),
            equipo_id=equipo_a_asignar.id,
            equipo=equipo_a_asignar,
            move_id=movimiento_seleccionado,
            movimientos=movimiento_elegido,
            naturaleza_id=id_naturaleza,
            naturaleza=buscar_naturaleza_por_id(id_naturaleza, session),
        )
        session.add(nuevo_integrante)
        session.commit()
        session.refresh(nuevo_integrante)

    equipo_a_asignar.integrantes.append(nuevo_integrante)

    evs_nuevo_miembro = Estadisticas(
        member_id=nuevo_id,
        vida=ptos_evs.vida,
        ataque=ptos_evs.ataque,
        defensa=ptos_evs.defensa,
        ataque_especial=ptos_evs.ataque_especial,
        defensa_especial=ptos_evs.defensa_especial,
        velocidad=ptos_evs.velocidad,
    )
    session.add(evs_nuevo_miembro)
    session.commit()
    session.refresh(evs_nuevo_miembro)


def verificar_datos_integrantes(
    id_pokemon: int,
    generacion_equipo: int,
    id_movimientos_seleccionados: list[int],
    ptos_evs: Estadisticas,
    id_naturaleza: int,
    session: SessionDep,
) -> None:
    verificar_naturalezas(id_naturaleza, session)
    verificar_generacion_del_pokemon(id_pokemon, generacion_equipo, session)
    verificar_movimientos_pokemon(id_pokemon, session, id_movimientos_seleccionados)
    verificar_evs(ptos_evs)


def asignar_nueva_id_miembro(session: SessionDep) -> int:
    query = select(IntegrantesEquipo)
    integrantes = session.exec(query).all()
    if integrantes:
        ultimo_integrante = max(integrantes, key=lambda integrante: integrante.id)
        nuevo_id = ultimo_integrante.id + 1
    else:
        nuevo_id = 1
    return nuevo_id


def buscar_pokemon_por_id(id_pokemon: int, session: SessionDep) -> Pokemon:
    query = select(Pokemon).where(Pokemon.id == id_pokemon)
    pokemon = session.exec(query).first()
    if not pokemon:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND, detail="Pokemon no encontrado."
        )
    return pokemon


def buscar_movimientos_por_id(id_movimiento: int, session: SessionDep) -> Movimientos:
    query = select(Movimientos).where(Movimientos.id == id_movimiento)
    movimiento = session.exec(query).first()
    if not movimiento:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND, detail="Movimiento no encontrado."
        )
    return movimiento


def buscar_movimientos_del_pokemon(
    id_pokemon: int, session: SessionDep
) -> list[MovimientosPokemon]:
    query = select(MovimientosPokemon).where(
        MovimientosPokemon.pokemon_id == id_pokemon
    )
    movimientos = session.exec(query).all()
    if not movimientos:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Movimientos del pokemon no encontrados.",
        )
    return movimientos


def buscar_naturaleza_por_id(id_naturaleza: int, session: SessionDep) -> Naturaleza:
    query = select(Naturaleza).where(Naturaleza.id == id_naturaleza)
    naturaleza = session.exec(query).first()
    if not naturaleza:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND, detail="Naturaleza no encontrada."
        )
    return naturaleza


def verificar_naturalezas(id_naturaleza: int, session: SessionDep):
    query = select(Naturaleza).where(Naturaleza.id == id_naturaleza)
    naturaleza = session.exec(query).first()
    if not naturaleza:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND, detail="Naturaleza no encontrada."
        )


def verificar_evs(evs: Estadisticas):
    if (
        evs.vida > 255
        or evs.ataque > 255
        or evs.defensa > 255
        or evs.ataque_especial > 255
        or evs.defensa_especial > 255
        or evs.velocidad > 255
    ):
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="Los EVs no pueden ser mayores a 255",
        )
    if (
        evs.vida < 0
        or evs.ataque < 0
        or evs.defensa < 0
        or evs.ataque_especial < 0
        or evs.defensa_especial < 0
        or evs.velocidad < 0
    ):
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="Los EVs no pueden ser menores a 0",
        )

    suma_evs = (
        evs.vida
        + evs.ataque
        + evs.defensa
        + evs.ataque_especial
        + evs.defensa_especial
        + evs.velocidad
    )

    if suma_evs > 510:
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="La suma de los EVs no puede ser mayor a 510",
        )


def verificar_generacion_del_pokemon(
    id_pokemon: int, generacion_equipo: int, session: SessionDep
):
    pokemon = buscar_pokemon_por_id(id_pokemon, session)
    if pokemon.generacion > generacion_equipo:
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="El pokemon no pertenece a la generación del equipo (tiene que ser menor o igual)",
        )


def verificar_movimientos_pokemon(
    id_pokemon: int, session: SessionDep, id_movimientos: list[int] = []
) -> bool:
    if not id_movimientos or len(id_movimientos) < 1:
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="El pokemon debe tener al menos un movimiento",
        )
    if len(id_movimientos) > 4:
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="El pokemon no puede tener más de 4 movimientos",
        )
    movimientos_aprendibles = buscar_movimientos_del_pokemon(id_pokemon, session)
    for id_movimiento in id_movimientos:
        if not session.exec(
            select(Movimientos).where(Movimientos.id == id_movimiento)
        ).first():
            raise HTTPException(
                status_code=status.HTTP_404_NOT_FOUND,
                detail="Movimiento no encontrado.",
            )
        if id_movimiento not in [
            movimiento_pokemon.move_id for movimiento_pokemon in movimientos_aprendibles
        ]:
            raise HTTPException(
                status_code=status.HTTP_400_BAD_REQUEST,
                detail="El pokemon no puede aprender ese movimiento",
            )

    return True


@router.put("/editar/{id_equipo}")
def editar_equipo(
    id_equipo: int,
    nombre_equipo: str,
    generacion_equipo: int,
    session: SessionDep,
    id_pkm_1: int = None,
    id_movimientos_pkm_1: list[int] = None,
    id_naturaleza_1: int = None,
    evs_pkm_1: Estadisticas = Estadisticas(
        vida=0, ataque=0, defensa=0, ataque_especial=0, defensa_especial=0, velocidad=0
    ),
    id_pkm_2: int = None,
    id_movimientos_pkm_2: list[int] = None,
    id_naturaleza_2: int = None,
    evs_pkm_2: Estadisticas = Estadisticas(
        vida=0, ataque=0, defensa=0, ataque_especial=0, defensa_especial=0, velocidad=0
    ),
    id_pkm_3: int = None,
    id_movimientos_pkm_3: list[int] = None,
    id_naturaleza_3: int = None,
    evs_pkm_3: Estadisticas = Estadisticas(
        vida=0, ataque=0, defensa=0, ataque_especial=0, defensa_especial=0, velocidad=0
    ),
    id_pkm_4: int = None,
    id_movimientos_pkm_4: list[int] = None,
    id_naturaleza_4: int = None,
    evs_pkm_4: Estadisticas = Estadisticas(
        vida=0, ataque=0, defensa=0, ataque_especial=0, defensa_especial=0, velocidad=0
    ),
    id_pkm_5: int = None,
    id_movimientos_pkm_5: list[int] = None,
    id_naturaleza_5: int = None,
    evs_pkm_5: Estadisticas = Estadisticas(
        vida=0, ataque=0, defensa=0, ataque_especial=0, defensa_especial=0, velocidad=0
    ),
    id_pkm_6: int = None,
    id_movimientos_pkm_6: list[int] = None,
    id_naturaleza_6: int = None,
    evs_pkm_6: Estadisticas = Estadisticas(
        vida=0, ataque=0, defensa=0, ataque_especial=0, defensa_especial=0, velocidad=0
    ),
):

    equipo = session.exec(select(Equipo).where(Equipo.id == id_equipo)).first()
    if not equipo:
        raise HTTPException(
            status_code=404,
            detail=f"El equipo con id {id_equipo} no ha sido encontrado",
        )
    if generacion_equipo and generacion_equipo not in range(1, 9):
        raise HTTPException(
            status_code=400, detail="La generación del equipo no es válida"
        )

    integrantes_a_eliminar = session.exec(
        select(IntegrantesEquipo).where(IntegrantesEquipo.equipo_id == id_equipo)
    ).all()
    for integrante in integrantes_a_eliminar:
        evs = session.exec(
            select(Estadisticas).where(Estadisticas.member_id == integrante.id)
        ).first()
        session.delete(evs)
        session.delete(integrante)
    session.commit()

    if nombre_equipo:
        equipo.nombre = nombre_equipo
    if generacion_equipo:
        equipo.generacion = generacion_equipo

    for i in range(1, 7):
        id_pokemon = locals().get(f"id_pkm_{i}")
        id_movimientos = locals().get(f"id_movimientos_pkm_{i}")
        id_naturaleza = locals().get(f"id_naturaleza_{i}")
        evs_pokemon = locals().get(f"evs_pkm_{i}")

        if id_pokemon is not None:
            verificar_datos_integrantes(
                id_pokemon,
                generacion_equipo or equipo.generacion,
                id_movimientos,
                evs_pokemon,
                id_naturaleza,
                session,
            )
            asignacion_datos_integrantes(
                id_pokemon, id_movimientos, evs_pokemon, id_naturaleza, equipo, session
            )

    session.add(equipo)
    session.commit()
    session.refresh(equipo)

    integrantes_publicos: List[IntegrantesEquipoPublic] = []
    ids_integrantes = []
    for integrante in equipo.integrantes:
        if integrante.id not in ids_integrantes:
            moves_integrantes = session.exec(
                (
                    select(Movimientos)
                    .join(IntegrantesEquipo)
                    .where(
                        IntegrantesEquipo.id == integrante.id,
                        IntegrantesEquipo.move_id == Movimientos.id,
                    )
                )
            ).all()
            integrantes_publicos.append(
                IntegrantesEquipoPublic(
                    pokemon=integrante.pokemon,
                    movimientos=moves_integrantes,
                    naturaleza=integrante.naturaleza,
                    evs=integrante.estadisticas,
                )
            )
            ids_integrantes.append(integrante.id)

    equipo_publico = EquipoPublic(
        id=equipo.id,
        nombre=equipo.nombre,
        generacion=equipo.generacion,
        integrantes=integrantes_publicos,
    )

    return equipo_publico


def get_movimientos_validos(session: SessionDep, pokemon_id: int) -> set:
    movimientos = (
        session.exec(Movimientos.id)
        .join(MovimientosPokemon)
        .filter(MovimientosPokemon.pokemon_id == pokemon_id)
        .all()
    )
    return {movimiento.id for movimiento in movimientos}


@router.get("/id/{equipo_id}", responses={status.HTTP_404_NOT_FOUND: {"model": Error}})
def obtener_equipo_por_id(session: SessionDep, equipo_id: int):
    equipo = buscar_equipo(session, equipo_id)
    integrantes_publicos: List[IntegrantesEquipoPublic] = []
    ids_integrantes = []
    for integrante in equipo.integrantes:
        if integrante.id not in ids_integrantes:
            query_moves_integrante = (
                select(Movimientos)
                .join(IntegrantesEquipo)
                .where(
                    IntegrantesEquipo.id == integrante.id,
                    IntegrantesEquipo.move_id == Movimientos.id,
                )
            )
            moves_integrantes = session.exec(query_moves_integrante).all()
            integrantes_publicos.append(
                IntegrantesEquipoPublic(
                    pokemon=integrante.pokemon,
                    movimientos=moves_integrantes,
                    naturaleza=integrante.naturaleza,
                    evs=integrante.estadisticas,
                )
            )
            ids_integrantes.append(integrante.id)

    equipo_publico = EquipoPublic(
        id=equipo.id,
        nombre=equipo.nombre,
        generacion=equipo.generacion,
        integrantes=integrantes_publicos,
    )

    return equipo_publico


@router.delete("/eliminar/{equipo_id}")
def eliminar_equipo(session: SessionDep, equipo_id: int):
    equipo = session.get(Equipo, equipo_id)
    if not equipo:
        raise HTTPException(
            status_code=404,
            detail=f"El equipo con id {equipo_id} no ha sido encontrado",
        )

    integrantes = session.exec(
        select(IntegrantesEquipo).where(IntegrantesEquipo.equipo_id == equipo_id)
    ).all()
    for integrante in integrantes:
        evs = session.exec(
            select(Estadisticas).where(Estadisticas.member_id == integrante.id)
        ).first()
        if evs:
            session.delete(evs)
        session.delete(integrante)

    session.delete(equipo)
    session.commit()

    return {
        "detail": f"El equipo con id {equipo_id} y sus integrantes han sido eliminados"
    }


def buscar_equipo(session: SessionDep, equipo_id: int) -> Equipo:
    query = select(Equipo).where(Equipo.id == equipo_id)
    equipo = session.exec(query).first()
    if equipo:
        return equipo
    raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Team not found")
