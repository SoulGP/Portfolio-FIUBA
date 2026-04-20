"""arreglando inconsistencias en integrante_equipo

Revision ID: a58fa4ef5f91
Revises: 66b5d51a8a53
Create Date: 2024-11-07 01:39:27.782046

"""
from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = 'a58fa4ef5f91'
down_revision: Union[str, None] = '66b5d51a8a53'
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None

def upgrade() -> None:
    with op.batch_alter_table('integrante_equipo') as batch_op:
        batch_op.alter_column('member_id', primary_key=False)
        batch_op.drop_constraint('member_id', type_='primary')
        batch_op.create_primary_key('pk_integrante_equipo', ['member_id', 'pokemon_id', 'move_id', 'nature_id'])

def downgrade() -> None:
    with op.batch_alter_table('integrante_equipo') as batch_op:
        batch_op.drop_constraint('pk_integrante_equipo', type_='primary')
        batch_op.alter_column('member_id', primary_key=True)
        batch_op.create_primary_key('member_id', ['member_id'])