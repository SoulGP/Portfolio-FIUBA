from pydantic import BaseModel
from typing import Optional, List
from sqlmodel import (
    SQLModel,
    Field,
    Column,
    ForeignKey,
    Relationship,
    Integer,
    Text,
    Boolean,
)


class PokemonBase(SQLModel):
    imagen: str = Field(sa_column=Column("imagen", Text, nullable=False))
    nombre: str = Field(sa_column=Column("identifier", Text, nullable=False))
    altura: int = Field(sa_column=Column("height", Integer, nullable=False))
    peso: int = Field(sa_column=Column("weight", Integer, nullable=False))
    especie: int = Field(sa_column=Column("species_id", Integer, nullable=False))
    generacion: int = Field(sa_column=Column("generation_id", Integer, nullable=False))


class Pokemon(PokemonBase, table=True):
    __tablename__ = "pokemon"
    id: int = Field(sa_column=Column("pokemon_id", Integer, primary_key=True))
    tipos: List["TiposPokemon"] = Relationship(back_populates="pokemon")
    habilidades: List["HabilidadesPokemon"] = Relationship(back_populates="pokemon")
    grupo_huevo: List["GrupoHuevoPokemon"] = Relationship(back_populates="pokemon")
    stats: List["StatsDelPokemon"] = Relationship(back_populates="pokemon")
    evoluciones: List["PokemonEvoluciones"] | None = Relationship(
        back_populates="pokemon"
    )
    movimientos: List["MovimientosPokemon"] | None = Relationship(
        back_populates="pokemon"
    )
    integrante: List["IntegrantesEquipo"] | None = Relationship(
        back_populates="pokemon"
    )


class PokemonPublic(SQLModel):
    id: int
    nombre: str
    imagen: str
    altura: int
    peso: int
    generacion: int
    evoluciones: List["PokemonEvolucionesPublic"] | None = None
    tipos: List["TiposPublic"]
    habilidades: List["HabilidadesPublic"]
    grupo_huevo: List["GrupoHuevoPublic"]
    stats: List["StatsDelPokemonPublic"]
    movimientos: List["MovimientosPokemonPublic"] | None = None


class PokemonCreatePublic(SQLModel):
    imagen: str
    nombre: str
    altura: int
    peso: int
    especie: int
    generacion: int
    evoluciones: List["PokemonEvoluciones"] | None = None
    tipos: List["TiposPokemonPublic"]
    habilidades: List["HabilidadesPokemonPublic"]
    grupo_huevo: List["GrupoHuevoPokemonPublic"]
    stats: List["StatsDelPokemonPublic"]
    movimientos: List["MovimientosPokemonPublic"]


class PokemonCreate(PokemonBase):
    pass


class PokemonEvoluciones(SQLModel, table=True):
    __tablename__ = "pokemon_evoluciones"
    pokemon_id: int = Field(sa_column=Column("pokemon_id", Integer, primary_key=True))
    evolution_id: int = Field(
        sa_column=Column(
            "evolution_id", Integer, ForeignKey("pokemon.pokemon_id"), primary_key=True
        )
    )
    imagen_evolucion: str = Field(sa_column=Column(Text, nullable=False))
    pokemon: Pokemon = Relationship(back_populates="evoluciones")


class PokemonSigEvolucion(SQLModel):
    nombre: str


class PokemonEvolucionesPublic(SQLModel):
    pokemon: PokemonSigEvolucion
    imagen_evolucion: str


class Tipos(SQLModel, table=True):
    __tablename__ = "tipos"
    id: int = Field(sa_column=Column("type_id", Integer, primary_key=True))
    nombre: str = Field(sa_column=Column("identifier", Text, nullable=False))
    pokemon: List["TiposPokemon"] = Relationship(back_populates="tipos")


class TiposPublic(SQLModel):
    nombre: str


class TiposCreate(SQLModel):
    pass


class TiposPublic(SQLModel):
    nombre: str


class TiposPokemon(SQLModel, table=True):
    __tablename__ = "tipo_pokemon"
    type_id: int = Field(
        sa_column=Column(Integer, ForeignKey("tipos.type_id"), primary_key=True)
    )
    pokemon_id: int = Field(
        sa_column=Column(Integer, ForeignKey("pokemon.pokemon_id"), primary_key=True)
    )
    pokemon: Pokemon = Relationship(back_populates="tipos")
    tipos: Tipos = Relationship(back_populates="pokemon")
    debilidades: List["DebilidadesTipo"] = Relationship(back_populates="tipos")


class TiposPokemonPublic(SQLModel):
    tipos: TiposPublic


class DebilidadesTipo(SQLModel, table=True):
    __tablename__ = "debilidades_tipo"
    damage_type_id: int = Field(
        sa_column=Column(
            "damage_type_id",
            Integer,
            ForeignKey("tipo_pokemon.type_id"),
            primary_key=True,
        )
    )
    target_type_id: int = Field(
        sa_column=Column("target_type_id", Integer, primary_key=True)
    )
    damage_factor: int = Field(
        sa_column=Column("damage_factor", Integer, nullable=True)
    )
    tipos: TiposPokemon = Relationship(back_populates="debilidades")


class HabilidadesBase(SQLModel):
    nombre: str = Field(sa_column=Column("identifier", Text, nullable=False))


class Habilidades(HabilidadesBase, table=True):
    __tablename__ = "habilidades"
    id: int = Field(sa_column=Column("ability_id", Integer, primary_key=True))
    pokemon: List["HabilidadesPokemon"] = Relationship(back_populates="habilidades")


class HabilidadesCreate(HabilidadesBase):
    pass


class HabilidadesPublic(SQLModel):
    nombre: str


class HabilidadesPokemon(SQLModel, table=True):
    __tablename__ = "pokemon_habilidad"
    pokemon_id: int = Field(
        sa_column=Column(Integer, ForeignKey("pokemon.pokemon_id"), primary_key=True)
    )
    ability_id: int = Field(
        sa_column=Column(
            Integer, ForeignKey("habilidades.ability_id"), primary_key=True
        )
    )
    es_oculta: bool = Field(sa_column=Column("is_hidden", Boolean, nullable=False))
    pokemon: Pokemon = Relationship(back_populates="habilidades")
    habilidades: Habilidades = Relationship(back_populates="pokemon")


class HabilidadesPokemonPublic(SQLModel):
    habilidades: HabilidadesPublic
    es_oculta: bool


class GrupoHuevo(SQLModel, table=True):
    __tablename__ = "grupo_huevo"
    id: int = Field(sa_column=Column("egg_group_id", Integer, primary_key=True))
    nombre: str = Field(sa_column=Column("identifier", Text, nullable=False))
    pokemon: List["GrupoHuevoPokemon"] = Relationship(back_populates="grupo_huevo")


class GrupoHuevoCreate(SQLModel):
    pass


class GrupoHuevoPublic(SQLModel):
    nombre: str


class GrupoHuevoPokemon(SQLModel, table=True):
    __tablename__ = "pokemon_grupo_huevo"
    species_id: int = Field(
        sa_column=Column(Integer, ForeignKey("pokemon.species_id"), primary_key=True)
    )
    egg_group_id: int = Field(
        sa_column=Column(
            Integer, ForeignKey("grupo_huevo.egg_group_id"), primary_key=True
        )
    )
    pokemon: Pokemon = Relationship(back_populates="grupo_huevo")
    grupo_huevo: GrupoHuevo = Relationship(back_populates="pokemon")


class GrupoHuevoPokemonPublic(SQLModel):
    grupo_huevo: GrupoHuevoPublic


class Stats(SQLModel, table=True):
    __tablename__ = "stats"
    id: int = Field(sa_column=Column("stat_id", Integer, primary_key=True))
    nombre: str = Field(sa_column=Column("identifier", Text, nullable=False))
    pokemon: List["StatsDelPokemon"] = Relationship(back_populates="stats")


class StatsPublic(SQLModel):
    nombre: str


class StatsCreate(SQLModel):
    pass


class StatsDelPokemon(SQLModel, table=True):
    __tablename__ = "pokemon_stats"
    pokemon_id: int = Field(
        sa_column=Column(Integer, ForeignKey("pokemon.pokemon_id"), primary_key=True)
    )
    stat_id: int = Field(
        sa_column=Column(Integer, ForeignKey("stats.stat_id"), primary_key=True)
    )
    base_stat: int = Field(sa_column=Column("base_stat", Integer))
    pokemon: Pokemon = Relationship(back_populates="stats")
    stats: Stats = Relationship(back_populates="pokemon")


class StatsDelPokemonPublic(SQLModel):
    stats: StatsPublic
    base_stat: int


class MovimientosBase(SQLModel):
    nombre: str = Field(sa_column=Column("identifier", Text, nullable=False))
    tipo: int = Field(
        sa_column=Column(
            "type_id", Integer, ForeignKey("tipos.type_id"), nullable=False
        )
    )
    categoria: int = Field(
        sa_column=Column(
            "damage_class_id",
            Integer,
            ForeignKey("categoria.move_damage_class_id"),
            nullable=False,
        )
    )
    potencia: int = Field(sa_column=Column("power", Integer))
    precision: int = Field(sa_column=Column("accuracy", Integer))
    usos: int = Field(sa_column=Column("pp", Integer, nullable=False))
    generacion: int = Field(sa_column=Column("generation_id", Integer, nullable=False))
    efecto: int = Field(
        sa_column=Column(
            "effect_id", Integer, ForeignKey("efecto.effect_id"), nullable=False
        )
    )


class Movimientos(MovimientosBase, table=True):
    __tablename__ = "movimiento"
    id: int = Field(sa_column=Column("move_id", Integer, primary_key=True))
    class_tipo: Tipos = Relationship()
    class_categoria: "CategoriaMovimiento" = Relationship()
    class_efecto: "EfectoMovimiento" = Relationship()
    pokemon: List["MovimientosPokemon"] = Relationship(back_populates="movimientos")
    integrante: List["IntegrantesEquipo"] = Relationship(
        back_populates="movimientos",
        sa_relationship_kwargs={
            "primaryjoin": "Movimientos.id == IntegrantesEquipo.move_id"
        },
    )


class MovimientosPublic(SQLModel):
    id: int
    nombre: str
    class_tipo: TiposPublic
    class_categoria: "CategoriaMovimientoPublic"
    potencia: Optional[int]
    precision: Optional[int]
    usos: Optional[int]
    generacion: int
    class_efecto: "EfectoMovimientoPublic"


class MovimientosCreate(MovimientosBase):
    pass


class MovimientosPokemon(SQLModel, table=True):
    __tablename__ = "pokemon_movimientos"
    pokemon_id: int = Field(
        sa_column=Column(Integer, ForeignKey("pokemon.pokemon_id"), primary_key=True)
    )
    move_id: int = Field(
        sa_column=Column(Integer, ForeignKey("movimiento.move_id"), primary_key=True)
    )
    pokemon: Pokemon = Relationship(back_populates="movimientos")
    movimientos: Movimientos = Relationship(back_populates="pokemon")
    id_metodo: int = Field(
        sa_column=Column(
            "pokemon_move_method_id",
            Integer,
            ForeignKey("metodo_aprender_movimiento.pokemon_move_method_id"),
        )
    )
    metodo: "MetodoAprenderMovimiento" = Relationship()
    nivel: int = Field(sa_column=Column("level", Integer))


class MovimientosPokemonPublic(SQLModel):
    movimientos: MovimientosPublic
    metodo: "MetodoAprenderMovimientoPublic"
    nivel: int


class MetodoAprenderMovimiento(SQLModel, table=True):
    __tablename__ = "metodo_aprender_movimiento"
    pokemon_move_method_id: int = Field(
        sa_column=Column(
            "pokemon_move_method_id", Integer, nullable=False, primary_key=True
        )
    )
    nombre: str = Field(sa_column=Column("identifier", Text, nullable=False))


class MetodoAprenderMovimientoPublic(SQLModel):
    nombre: str


class CategoriaMovimiento(SQLModel, table=True):
    __tablename__ = "categoria"
    id: int = Field(sa_column=Column("move_damage_class_id", Integer, primary_key=True))
    nombre: str = Field(sa_column=Column("identifier", Text, nullable=False))


class CategoriaMovimientoPublic(SQLModel):
    nombre: str


class EfectoMovimiento(SQLModel, table=True):
    __tablename__ = "efecto"
    id: int = Field(sa_column=Column("effect_id", Integer, primary_key=True))
    descripcion: str = Field(sa_column=Column("description", Text, nullable=False))


class EfectoMovimientoPublic(SQLModel):
    descripcion: str


class NaturalezaBase(SQLModel):
    nombre: str = Field(sa_column=Column("identifier", Text, nullable=False))
    stat_perjudicada: int = Field(
        sa_column=Column("decreased_stat_id", Integer, nullable=False)
    )
    stat_mejorada: int = Field(
        sa_column=Column("increased_stat_id", Integer, nullable=False)
    )


class Naturaleza(NaturalezaBase, table=True):
    __tablename__ = "naturaleza"
    id: int = Field(sa_column=Column("id", Integer, primary_key=True))


class NaturalezaPublic(SQLModel):
    nombre: str


class EstadisticasBase(SQLModel):
    vida: int = Field(sa_column=Column("hp", Integer, nullable=False))
    ataque: int = Field(sa_column=Column("attack", Integer, nullable=False))
    defensa: int = Field(sa_column=Column("defense", Integer, nullable=False))
    ataque_especial: int = Field(
        sa_column=Column("special-attack", Integer, nullable=False)
    )
    defensa_especial: int = Field(
        sa_column=Column("special-defense", Integer, nullable=False)
    )
    velocidad: int = Field(sa_column=Column("speed", Integer, nullable=False))


class Estadisticas(EstadisticasBase, table=True):
    __tablename__ = "estadisticas"
    member_id: int = Field(
        sa_column=Column(
            "member_id",
            Integer,
            ForeignKey("integrante_equipo.member_id"),
            primary_key=True,
        )
    )
    integrante: "IntegrantesEquipo" = Relationship(back_populates="estadisticas")


class EstadisticasPublic(SQLModel):
    vida: int
    ataque: int
    defensa: int
    ataque_especial: int
    defensa_especial: int
    velocidad: int


class IntegrantesEquipo(SQLModel, table=True):
    __tablename__ = "integrante_equipo"
    id: int = Field(sa_column=Column("member_id", Integer, primary_key=True))
    pokemon_id: int = Field(
        sa_column=Column(
            "pokemon_id", Integer, ForeignKey("pokemon.pokemon_id"), primary_key=True
        )
    )
    pokemon: Optional[Pokemon] = Relationship(back_populates="integrante")
    equipo_id: int = Field(
        sa_column=Column("equipo_id", Integer, ForeignKey("equipo.id"))
    )
    equipo: Optional["Equipo"] = Relationship(back_populates="integrantes")
    move_id: int = Field(
        sa_column=Column(
            "move_id", Integer, ForeignKey("movimiento.move_id"), primary_key=True
        )
    )
    movimientos: List[Movimientos] = Relationship()
    naturaleza_id: int = Field(
        sa_column=Column(
            "nature_id", Integer, ForeignKey("naturaleza.id"), primary_key=True
        )
    )
    naturaleza: Naturaleza = Relationship()
    estadisticas: Estadisticas = Relationship(back_populates="integrante")


class IntegrantesEquipoPublic(SQLModel):
    pokemon: Pokemon
    movimientos: List[MovimientosPublic]
    naturaleza: NaturalezaPublic
    evs: EstadisticasPublic


class Equipo(SQLModel, table=True):
    __tablename__ = "equipo"
    id: int = Field(sa_column=Column("id", Integer, primary_key=True))
    nombre: str = Field(sa_column=Column("identifier", Text, nullable=False))
    generacion: int = Field(sa_column=Column("generation_id", Integer, nullable=False))
    integrantes: List["IntegrantesEquipo"] = Relationship(back_populates="equipo")


class EquipoPublic(SQLModel):
    id: int
    nombre: str
    integrantes: List[IntegrantesEquipoPublic]
    generacion: int


class Error(BaseModel):
    detail: str
