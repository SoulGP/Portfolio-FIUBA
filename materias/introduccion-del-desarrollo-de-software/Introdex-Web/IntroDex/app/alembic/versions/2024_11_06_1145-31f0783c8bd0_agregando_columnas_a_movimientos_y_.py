"""Agregando columnas a movimientos y grupo huevo

Revision ID: 31f0783c8bd0
Revises: 6e4f225c16b4
Create Date: 2024-11-06 11:45:55.453848

"""
from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = '31f0783c8bd0'
down_revision: Union[str, None] = '6e4f225c16b4'
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    with op.batch_alter_table('movimiento') as batch_op:
        batch_op.add_column(sa.Column('identifier', sa.Text, nullable=False))
    
    with op.batch_alter_table('grupo_huevo') as batch_op:
        batch_op.add_column(sa.Column('identifier', sa.Text, nullable=False))


def downgrade() -> None:
    with op.batch_alter_table('grupo_huevo') as batch_op:
        batch_op.drop_column('identifier')
    
    with op.batch_alter_table('movimiento') as batch_op:
        batch_op.drop_column('identifier')
