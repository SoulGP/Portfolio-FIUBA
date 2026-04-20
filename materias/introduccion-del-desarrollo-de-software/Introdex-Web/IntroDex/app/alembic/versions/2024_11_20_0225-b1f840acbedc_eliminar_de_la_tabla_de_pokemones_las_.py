"""Eliminar, de la tabla de pokemones, las columnas con respecto a la evolucion

Revision ID: b1f840acbedc
Revises: fa593d93d697
Create Date: 2024-11-20 02:25:53.152984

"""
from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = 'b1f840acbedc'
down_revision: Union[str, None] = 'fa593d93d697'
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.drop_column('pokemon', 'evolution_id')
    op.drop_column('pokemon', 'imagen_evolucion')


def downgrade() -> None:
    op.add_column('pokemon', sa.Column('evolution_id', sa.Integer, nullable=True))
    op.add_column('pokemon', sa.Column('imagen_evolucion', sa.Text, nullable=True))
