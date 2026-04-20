"""crear tabla de relacion de tabla movimiento con la tabla categoria.

Revision ID: 156fc754068a
Revises: 28c64ebab55e
Create Date: 2024-11-06 23:42:14.205208

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "156fc754068a"
down_revision: Union[str, None] = "28c64ebab55e"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.create_table(
        "categoria",
        sa.Column("move_damage_class_id", sa.Integer, primary_key=True),
        sa.Column("identifier", sa.Text, nullable=False),
    )


def downgrade() -> None:
    op.drop_table("categoria")
