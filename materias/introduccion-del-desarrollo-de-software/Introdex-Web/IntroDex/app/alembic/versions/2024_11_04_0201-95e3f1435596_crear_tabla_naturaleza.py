"""Crear tabla naturaleza

Revision ID: 95e3f1435596
Revises: eaeb22837af1
Create Date: 2024-11-04 02:01:59.396941

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "95e3f1435596"
down_revision: Union[str, None] = None
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.create_table(
        "naturaleza",
        sa.Column("id", sa.Integer, primary_key=True),
        sa.Column("identifier", sa.Text, nullable=False),
        sa.Column("decreased_stat_id", sa.Integer, nullable=False),
        sa.Column("increased_stat_id", sa.Integer, nullable=False),
    )


def downgrade() -> None:
    op.drop_table("naturaleza")
