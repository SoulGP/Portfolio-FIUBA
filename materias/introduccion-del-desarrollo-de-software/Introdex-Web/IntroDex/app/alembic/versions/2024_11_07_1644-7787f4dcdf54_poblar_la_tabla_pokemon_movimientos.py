"""poblar la tabla pokemon_movimientos.

Revision ID: 7787f4dcdf54
Revises: a7054a72add8
Create Date: 2024-11-07 16:44:53.188753

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa
from sqlalchemy.sql import table, column, select
import csv


# revision identifiers, used by Alembic.
revision: str = "7787f4dcdf54"
down_revision: Union[str, None] = "a7054a72add8"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


pokemon_movimientos_table = table(
    "pokemon_movimientos",
    column("move_id", sa.Integer),
    column("pokemon_id", sa.Integer),
    column("pokemon_move_method_id", sa.Integer),
    column("level", sa.Integer),
)


def upgrade() -> None:
    conn = op.get_bind()

    with op.batch_alter_table("pokemon_movimientos", schema=None) as batch_op:
        batch_op.add_column(sa.Column("level", sa.Integer, nullable=True))

    with open("data/pokemon_moves.csv", newline="", encoding="utf-8") as moves_file:
        moves_data = csv.DictReader(moves_file)

        for move in moves_data:
            pokemon_move_method_id = int(move["pokemon_move_method_id"])
            if pokemon_move_method_id not in {1, 2, 4}:
                continue

            move_id = int(move["move_id"])
            pokemon_id = int(move["pokemon_id"])
            level = int(move["level"]) if move["level"].isdigit() else None

            exists = conn.execute(
                select(pokemon_movimientos_table.c.move_id).where(
                    (pokemon_movimientos_table.c.move_id == move_id)
                    & (pokemon_movimientos_table.c.pokemon_id == pokemon_id)
                )
            ).fetchone()

            if not exists:
                conn.execute(
                    pokemon_movimientos_table.insert().values(
                        move_id=move_id,
                        pokemon_id=pokemon_id,
                        pokemon_move_method_id=pokemon_move_method_id,
                        level=level,
                    )
                )


def downgrade() -> None:
    conn = op.get_bind()
    conn.execute(sa.text("DELETE FROM pokemon_movimientos"))

    with op.batch_alter_table("pokemon_movimientos", schema=None) as batch_op:
        batch_op.drop_column("level")
