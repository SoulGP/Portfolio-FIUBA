"""Crear tabla metodo para aprender movimiento

Revision ID: a2e5f9517df0
Revises: b82a4085972c
Create Date: 2024-10-31 17:30:44.446417

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "a2e5f9517df0"
down_revision: Union[str, None] = "b82a4085972c"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.create_table(
        "movimientos_aprendibles",
        sa.Column("move_id", sa.Integer, primary_key=True),
    )


def downgrade() -> None:
    op.drop_table("movimientos_aprendibles")
