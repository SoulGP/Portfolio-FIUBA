"""Tabla para equipos

Revision ID: 977a277e0d31
Revises: 73fe80d8d855
Create Date: 2024-11-04 15:10:50.166487

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "977a277e0d31"
down_revision: Union[str, None] = None
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.create_table(
        "equipo",
        sa.Column("id", sa.Integer, primary_key=True),
        sa.Column("identifier", sa.Text, nullable=False),
        sa.Column("generation_id", sa.Integer, nullable=False),
    )


def downgrade() -> None:
    op.drop_table("equipo")
