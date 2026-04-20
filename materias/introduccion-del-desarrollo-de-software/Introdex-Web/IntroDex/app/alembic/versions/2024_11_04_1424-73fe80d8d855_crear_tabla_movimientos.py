"""Crear tabla movimientos

Revision ID: 73fe80d8d855
Revises: 95e3f1435596
Create Date: 2024-11-04 14:24:38.597170

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa

# revision identifiers, used by Alembic.
revision: str = "73fe80d8d855"
down_revision: Union[str, None] = None
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
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
        sa.ForeignKeyConstraint(["pokemon_id"], ["pokemon.pokemon_id"]),
        sa.ForeignKeyConstraint(
            ["pokemon_move_method_id"],
            ["metodo_aprender_movimiento.pokemon_move_method_id"],
        ),
    )


def downgrade() -> None:
    op.drop_table("movimiento")
