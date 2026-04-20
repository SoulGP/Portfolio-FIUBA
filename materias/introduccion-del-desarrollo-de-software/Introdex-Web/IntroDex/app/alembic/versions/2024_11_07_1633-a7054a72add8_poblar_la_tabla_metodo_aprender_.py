"""poblar la tabla metodo_aprender_movimiento.

Revision ID: a7054a72add8
Revises: ec8169f6fb26
Create Date: 2024-11-07 16:33:55.553491

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa
from sqlalchemy.sql import table, column, select
import csv


# revision identifiers, used by Alembic.
revision: str = "a7054a72add8"
down_revision: Union[str, None] = "ec8169f6fb26"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


metodo_aprender_movimiento_table = table(
    "metodo_aprender_movimiento",
    column("pokemon_move_method_id", sa.Integer),
    column("identifier", sa.String),
)


def upgrade() -> None:
    conn = op.get_bind()

    with open(
        "data/pokemon_move_methods.csv", newline="", encoding="utf-8"
    ) as move_methods_file:
        move_methods_data = list(csv.DictReader(move_methods_file))

        for move_method in move_methods_data:
            pokemon_move_method_id = int(move_method["pokemon_move_method_id"])
            identifier = move_method["name"]
            local_language_id = int(move_method["local_language_id"])

            if (
                pokemon_move_method_id in {1, 2, 4} and local_language_id == 7
            ):  # español
                exists = conn.execute(
                    select(
                        metodo_aprender_movimiento_table.c.pokemon_move_method_id
                    ).where(
                        metodo_aprender_movimiento_table.c.pokemon_move_method_id
                        == pokemon_move_method_id
                    )
                ).fetchone()

                if not exists:
                    conn.execute(
                        metodo_aprender_movimiento_table.insert().values(
                            pokemon_move_method_id=pokemon_move_method_id,
                            identifier=identifier,
                        )
                    )


def downgrade() -> None:
    conn = op.get_bind()
    conn.execute(sa.text("DELETE FROM metodo_aprender_movimiento"))
