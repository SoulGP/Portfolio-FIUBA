"""optimizacion de la tabla movimientos_aprendibles.

Revision ID: 1120b1b433d5
Revises: 127683849e9d
Create Date: 2024-11-07 00:02:20.932114

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "1120b1b433d5"
down_revision: Union[str, None] = "127683849e9d"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.drop_table("movimientos_aprendibles")
    op.create_table(
        "pokemon_movimientos",
        sa.Column("move_id", sa.Integer, primary_key=True),
        sa.Column("pokemon_id", sa.Integer, primary_key=True),
        sa.ForeignKeyConstraint(["move_id"], ["movimiento.move_id"], name="fk_move"),
        sa.ForeignKeyConstraint(
            ["pokemon_id"], ["pokemon.pokemon_id"], name="fk_pokemon"
        ),
        sa.Column("pokemon_move_method_id", sa.Integer),
        sa.ForeignKeyConstraint(
            ["pokemon_move_method_id"],
            ["metodo_aprender_movimiento.pokemon_move_method_id"],
        ),
    )


def downgrade() -> None:
    op.drop_table("pokemon_movimientos")
    op.create_table(
        "movimientos_aprendibles",
        sa.Column("move_id", sa.Integer, primary_key=True),
    )
    with op.batch_alter_table("movimientos_aprendibles") as batch_op:
        batch_op.add_column(sa.Column("pokemon_id", sa.Integer, nullable=False))
        batch_op.create_foreign_key(
            "fk_pokemon_movimiento_aprendible",
            "pokemon",
            ["pokemon_id"],
            ["pokemon_id"],
        )
