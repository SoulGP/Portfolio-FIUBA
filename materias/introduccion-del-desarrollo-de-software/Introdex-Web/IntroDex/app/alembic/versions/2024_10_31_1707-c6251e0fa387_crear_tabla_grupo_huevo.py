"""Crear tabla grupo_huevo

Revision ID: c6251e0fa387
Revises: 12c7a724fce2
Create Date: 2024-10-31 17:07:19.175250

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "c6251e0fa387"
down_revision: Union[str, None] = "12c7a724fce2"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.create_table(
        "grupo_huevo",
        sa.Column("egg_group_id", sa.Integer, primary_key=True),
    )


def downgrade() -> None:
    op.drop_table("grupo_huevo")
