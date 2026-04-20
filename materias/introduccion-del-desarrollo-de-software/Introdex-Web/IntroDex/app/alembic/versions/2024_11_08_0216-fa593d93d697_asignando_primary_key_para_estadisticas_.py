"""Asignando primary key para estadisticas con la columna member_id

Revision ID: fa593d93d697
Revises: 7787f4dcdf54
Create Date: 2024-11-08 02:16:07.187745

"""
from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = 'fa593d93d697'
down_revision: Union[str, None] = '7787f4dcdf54'
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    with op.batch_alter_table("estadisticas") as batch_op:
        batch_op.alter_column("member_id", primary_key=True)


def downgrade() -> None:
    with op.batch_alter_table("estadisticas") as batch_op:
        batch_op.alter_column("member_id", primary_key=False)
        