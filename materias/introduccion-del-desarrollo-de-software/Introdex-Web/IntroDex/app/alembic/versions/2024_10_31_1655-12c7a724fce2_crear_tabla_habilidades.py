"""Crear tabla habilidades

Revision ID: 12c7a724fce2
Revises: 4008effab822
Create Date: 2024-10-31 16:55:26.202181

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "12c7a724fce2"
down_revision: Union[str, None] = "4008effab822"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.create_table(
        "habilidades",
        sa.Column("ability_id", sa.Integer, primary_key=True),
        sa.Column("is_hiden", sa.Integer, nullable=False),
    )


def downgrade() -> None:
    op.drop_table("habilidades")
