"""crear tabla de relacion de tabla movimiento con la tabla efecto.

Revision ID: 72ee0b236030
Revises: 156fc754068a
Create Date: 2024-11-06 23:45:58.312884

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "72ee0b236030"
down_revision: Union[str, None] = "156fc754068a"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.create_table(
        "efecto",
        sa.Column("effect_id", sa.Integer, primary_key=True),
        sa.Column("description", sa.Text, nullable=False),
    )


def downgrade() -> None:
    op.drop_table("efecto")
