"""Modificacion de una columna de la tabla habilidades

Revision ID: 7490f94c2bb9
Revises: 914e13f16ac8
Create Date: 2024-11-06 21:55:48.735054

"""
from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = '7490f94c2bb9'
down_revision: Union[str, None] = '914e13f16ac8'
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None

def upgrade() -> None:
    with op.batch_alter_table("habilidades") as batch_op:
        batch_op.alter_column("identifier",
                              existing_type=sa.String(length=50),
                              type_=sa.Text)


def downgrade() -> None:
    with op.batch_alter_table("habilidades") as batch_op:
        batch_op.alter_column("identifier",
                              existing_type=sa.Text,
                              type_=sa.String(length=50))
