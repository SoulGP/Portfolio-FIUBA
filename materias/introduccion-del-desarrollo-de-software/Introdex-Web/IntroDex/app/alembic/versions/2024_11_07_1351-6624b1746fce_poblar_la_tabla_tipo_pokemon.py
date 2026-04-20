"""poblar la tabla tipo_pokemon.

Revision ID: 6624b1746fce
Revises: d5cee5dab0a5
Create Date: 2024-11-07 13:51:40.616793

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa
from sqlalchemy import table, column, Integer
import csv


# revision identifiers, used by Alembic.
revision: str = "6624b1746fce"
down_revision: Union[str, None] = "d5cee5dab0a5"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


tipo_pokemon_table = table(
    "tipo_pokemon",
    column("type_id", Integer),
    column("pokemon_id", Integer),
)


def upgrade() -> None:
    conn = op.get_bind()

    with open(
        "data/pokemon_types.csv", newline="", encoding="utf-8"
    ) as pokemon_types_file:
        pokemon_types_data = csv.DictReader(pokemon_types_file)

        for pokemon_type in pokemon_types_data:
            type_id = int(pokemon_type["type_id"])
            pokemon_id = int(pokemon_type["pokemon_id"])

            conn.execute(
                tipo_pokemon_table.insert().values(
                    type_id=type_id,
                    pokemon_id=pokemon_id,
                )
            )


def downgrade() -> None:
    conn = op.get_bind()
    conn.execute(sa.text("DELETE FROM tipo_pokemon"))
