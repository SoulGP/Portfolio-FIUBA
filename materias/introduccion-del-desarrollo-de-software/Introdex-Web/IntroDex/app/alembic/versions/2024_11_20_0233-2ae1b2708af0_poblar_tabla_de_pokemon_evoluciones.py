"""Poblar tabla de pokemon_evoluciones

Revision ID: 2ae1b2708af0
Revises: 58f8c43d2a3c
Create Date: 2024-11-20 02:33:53.854595

"""
from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa
from sqlalchemy.sql import table, column, select
import csv


# revision identifiers, used by Alembic.
revision: str = '2ae1b2708af0'
down_revision: Union[str, None] = '58f8c43d2a3c'
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None

pokemon_evolutiones_table = table(
    "pokemon_evoluciones",
    column("pokemon_id", sa.Integer),
    column("evolution_id", sa.Integer),
    column("imagen_evolucion", sa.Text),
)


def upgrade() -> None:
    conn = op.get_bind()

    with open("data/pokemon_evolutions.csv", newline="", encoding="utf-8") as evolutions_file:
        evolutions_data = csv.DictReader(evolutions_file)

        for evolution in evolutions_data:
            pokemon_id = int(evolution["id"])
            evolution_id = int(evolution["evolution_id"])

            exists = conn.execute(
                select(pokemon_evolutiones_table.c.pokemon_id).where(
                    (pokemon_evolutiones_table.c.pokemon_id == pokemon_id)
                    & (pokemon_evolutiones_table.c.evolution_id == evolution_id)
                )
            ).fetchone()

            if not exists:
                conn.execute(
                    pokemon_evolutiones_table.insert().values(
                        pokemon_id=pokemon_id,
                        evolution_id=evolution_id,
                        imagen_evolucion=f"https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/{evolution_id}.png",
                    )
                )


def downgrade() -> None:
    conn = op.get_bind()

    conn.execute(pokemon_evolutiones_table.delete())
