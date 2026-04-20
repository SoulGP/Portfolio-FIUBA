"""Crear tabla stats

Revision ID: 7c7d63eefb67
Revises: c6251e0fa387
Create Date: 2024-10-31 17:29:57.474361

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "7c7d63eefb67"
down_revision: Union[str, None] = "c6251e0fa387"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.create_table(
        "stats",
        sa.Column("stat_id", sa.Integer, primary_key=True),
        sa.Column("base_stat", sa.Integer, nullable=False),
    )


def downgrade() -> None:
    op.drop_table("stats")
