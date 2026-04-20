"""Nueva tabla de pokemon_evoluciones

Revision ID: 58f8c43d2a3c
Revises: b1f840acbedc
Create Date: 2024-11-20 02:29:20.889491

"""
from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = '58f8c43d2a3c'
down_revision: Union[str, None] = 'b1f840acbedc'
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.create_table(
        "pokemon_evoluciones",
        sa.Column("pokemon_id", sa.Integer, primary_key=True),
        sa.Column("evolution_id", sa.Integer, primary_key=True),
        sa.Column("imagen_evolucion", sa.Text, nullable=False),
    )


def downgrade() -> None:
    op.drop_table("pokemon_evoluciones")
