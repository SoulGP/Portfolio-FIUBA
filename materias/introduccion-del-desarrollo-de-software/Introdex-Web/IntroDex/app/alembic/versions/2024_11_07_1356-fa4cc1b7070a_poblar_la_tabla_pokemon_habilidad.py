"""poblar la tabla pokemon_habilidad.

Revision ID: fa4cc1b7070a
Revises: 6624b1746fce
Create Date: 2024-11-07 13:56:42.844429

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa
from sqlalchemy import table, column, Integer
import csv


# revision identifiers, used by Alembic.
revision: str = "fa4cc1b7070a"
down_revision: Union[str, None] = "6624b1746fce"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


pokemon_habilidad_table = table(
    "pokemon_habilidad",
    column("pokemon_id", Integer),
    column("ability_id", Integer),
)


def upgrade() -> None:
    conn = op.get_bind()

    with open(
        "data/pokemon_abilities.csv", newline="", encoding="utf-8"
    ) as pokemon_abilities_file:
        pokemon_abilities_data = csv.DictReader(pokemon_abilities_file)

        for pokemon_abilities in pokemon_abilities_data:
            pokemon_id = int(pokemon_abilities["pokemon_id"])
            ability_id = int(pokemon_abilities["ability_id"])

            conn.execute(
                pokemon_habilidad_table.insert().values(
                    pokemon_id=pokemon_id,
                    ability_id=ability_id,
                )
            )


def downgrade() -> None:
    conn = op.get_bind()
    conn.execute(sa.text("DELETE FROM pokemon_habilidad"))
