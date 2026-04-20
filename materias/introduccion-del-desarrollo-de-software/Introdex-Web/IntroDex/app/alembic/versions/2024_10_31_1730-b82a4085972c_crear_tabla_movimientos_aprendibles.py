"""Crear tabla metodo movimientos aprendibles

Revision ID: b82a4085972c
Revises: 7c7d63eefb67
Create Date: 2024-10-31 17:30:14.811272

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "b82a4085972c"
down_revision: Union[str, None] = "7c7d63eefb67"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.create_table(
        "metodo_aprender_movimiento",
        sa.Column("move_id", sa.Integer, primary_key=True),
        sa.Column("pokemon_move_method_id", sa.Integer, nullable=False),
        sa.ForeignKeyConstraint(["move_id"], ["movimientos_aprendibles.move_id"]),
    )


def downgrade() -> None:
    op.drop_table("metodo_aprender_movimiento")
