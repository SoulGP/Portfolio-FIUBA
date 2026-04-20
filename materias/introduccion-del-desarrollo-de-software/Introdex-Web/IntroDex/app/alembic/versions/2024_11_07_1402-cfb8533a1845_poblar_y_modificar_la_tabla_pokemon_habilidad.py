"""poblar y modificar la tabla pokemon_habilidad.

Revision ID: cfb8533a1845
Revises: fa4cc1b7070a
Create Date: 2024-11-07 14:02:52.287304

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa
from sqlalchemy import table, column, Integer, Boolean
import csv


# revision identifiers, used by Alembic.
revision: str = "cfb8533a1845"
down_revision: Union[str, None] = "fa4cc1b7070a"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


pokemon_habilidad_table = table(
    "pokemon_habilidad",
    column("pokemon_id", Integer),
    column("ability_id", Integer),
    column("is_hidden", Boolean),
)


def upgrade() -> None:
    with op.batch_alter_table("pokemon_habilidad") as batch_op:
        batch_op.add_column(
            sa.Column(
                "is_hidden", sa.Boolean, nullable=False, server_default=sa.false()
            )
        )

    conn = op.get_bind()
    conn.execute(sa.text("DELETE FROM pokemon_habilidad"))

    with open(
        "data/pokemon_abilities.csv", newline="", encoding="utf-8"
    ) as pokemon_abilities_file:
        pokemon_abilities_data = csv.DictReader(pokemon_abilities_file)

        for pokemon_abilities in pokemon_abilities_data:
            pokemon_id = int(pokemon_abilities["pokemon_id"])
            ability_id = int(pokemon_abilities["ability_id"])
            is_hidden = bool(int(pokemon_abilities["is_hidden"]))

            conn.execute(
                pokemon_habilidad_table.insert().values(
                    pokemon_id=pokemon_id, ability_id=ability_id, is_hidden=is_hidden
                )
            )


def downgrade() -> None:
    with op.batch_alter_table("pokemon_habilidad") as batch_op:
        batch_op.drop_column("is_hidden")
