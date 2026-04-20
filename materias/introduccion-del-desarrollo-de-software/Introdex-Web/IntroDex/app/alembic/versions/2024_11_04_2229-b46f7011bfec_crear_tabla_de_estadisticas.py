"""Crear tabla de estadisticas

Revision ID: b46f7011bfec
Revises: bf6b6a6a3f1a
Create Date: 2024-11-04 22:29:30.025019

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "b46f7011bfec"
down_revision: Union[str, None] = None
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.create_table(
        "estadisticas",
        sa.Column("hp", sa.Integer, nullable=False),
        sa.Column("attack", sa.Integer, nullable=False),
        sa.Column("defense", sa.Integer, nullable=False),
        sa.Column("special-attack", sa.Integer, nullable=False),
        sa.Column("special-defense", sa.Integer, nullable=False),
        sa.Column("speed", sa.Integer, nullable=False),
    )


def downgrade() -> None:
    op.drop_table("estadisticas")
