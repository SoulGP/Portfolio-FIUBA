"""poblar la tabla pokemon.

Revision ID: 2fc58a718849
Revises: 9ea1e2785e88
Create Date: 2024-11-07 12:08:02.749385

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa
import csv
from sqlalchemy.sql import table, column
from sqlalchemy import Integer, String


# revision identifiers, used by Alembic.
revision: str = "2fc58a718849"
down_revision: Union[str, None] = "9ea1e2785e88"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None

pokemon_table = table(
    "pokemon",
    column("pokemon_id", Integer),
    column("imagen", String),
    column("identifier", String),
    column("height", Integer),
    column("weight", Integer),
    column("species_id", Integer),
    column("evolution_id", Integer),
    column("imagen_evolucion", String),
    column("generation_id", Integer),
)


def upgrade() -> None:
    conn = op.get_bind()

    def buscar_evolucion_por_pokemon_id(evolution_data, id_pokemon):
        for evolucion in evolution_data:
            if int(evolucion["id"]) == id_pokemon:
                return int(evolucion["evolution_id"])
        return None

    def buscar_generacion_por_pokemon_id(data_generaciones, id_pokemon):
        for especie in data_generaciones:
            if int(especie["id"]) == id_pokemon:
                return int(especie["generation_id"])
        return 0

    with open("data/pokemon.csv", newline="", encoding="utf-8") as pokemon_file, open(
        "data/pokemon_evolutions.csv", encoding="utf-8"
    ) as evolution_file, open(
        "data/pokemon_species.csv", encoding="utf-8"
    ) as species_file:

        pokemon_data = csv.DictReader(pokemon_file)
        evolution_data = list(csv.DictReader(evolution_file))
        species_data = list(csv.DictReader(species_file))

        for pokemon in pokemon_data:
            pokemon_id = int(pokemon["id"])
            evolution_id = buscar_evolucion_por_pokemon_id(evolution_data, pokemon_id)
            generation_id = buscar_generacion_por_pokemon_id(species_data, pokemon_id)

            conn.execute(
                pokemon_table.insert().values(
                    pokemon_id=pokemon_id,
                    imagen=f"https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/{pokemon_id}.png",
                    identifier=pokemon["identifier"],
                    height=int(pokemon["height"]),
                    weight=int(pokemon["weight"]),
                    species_id=int(pokemon["species_id"]),
                    evolution_id=evolution_id,
                    imagen_evolucion=(
                        f"https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/{evolution_id}.png"
                        if evolution_id
                        else None
                    ),
                    generation_id=generation_id,
                )
            )


def downgrade() -> None:
    conn = op.get_bind()
    conn.execute(sa.text("DELETE FROM pokemon"))
