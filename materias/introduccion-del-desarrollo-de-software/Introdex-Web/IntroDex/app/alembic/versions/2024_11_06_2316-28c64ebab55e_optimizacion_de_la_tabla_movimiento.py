"""optimizacion de la tabla movimiento.

Revision ID: 28c64ebab55e
Revises: 6378520b338a
Create Date: 2024-11-06 23:16:24.870912

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "28c64ebab55e"
down_revision: Union[str, None] = "6378520b338a"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.drop_table("movimiento")
    op.create_table(
        "movimiento",
        sa.Column("move_id", sa.Integer, primary_key=True),
        sa.Column("identifier", sa.Text, nullable=False),
        sa.Column("type_id", sa.Integer, nullable=False),
        sa.Column("damage_class_id", sa.Integer, nullable=False),
        sa.Column("power", sa.Integer, nullable=False),
        sa.Column("accuracy", sa.Integer, nullable=False),
        sa.Column("pp", sa.Integer, nullable=False),
        sa.Column("generation_id", sa.Integer, nullable=False),
        sa.Column("effect_id", sa.Integer, nullable=False),
        sa.ForeignKeyConstraint(["type_id"], ["tipos.type_id"]),
    )


def downgrade() -> None:
    op.drop_table("movimiento")
    op.create_table(
        "movimiento",
        sa.Column("move_id", sa.Integer, primary_key=True),
        sa.Column("pokemon_id", sa.Integer, nullable=False),
        sa.Column("pokemon_move_method_id", sa.Integer, nullable=False),
        sa.Column("type_id", sa.Integer, nullable=False),
        sa.Column("damage_class_id", sa.Integer, nullable=False),
        sa.Column("power", sa.Integer, nullable=False),
        sa.Column("accuracy", sa.Integer, nullable=False),
        sa.Column("pp", sa.Integer, nullable=False),
        sa.Column("generation_id", sa.Integer, nullable=False),
        sa.Column("effect_id", sa.Integer, nullable=False),
        sa.Column("identifier", sa.Text, nullable=False),
        sa.ForeignKeyConstraint(["pokemon_id"], ["pokemon.pokemon_id"]),
        sa.ForeignKeyConstraint(
            ["pokemon_move_method_id"],
            ["metodo_aprender_movimiento.pokemon_move_method_id"],
        ),
    )
