"""Crear tabla tipo de debilidades

Revision ID: 5960d76159d8
Revises: a2e5f9517df0
Create Date: 2024-10-31 17:31:02.667739

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "5960d76159d8"
down_revision: Union[str, None] = "a2e5f9517df0"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.create_table(
        "debilidades_tipo",
        sa.Column("type_id", sa.Integer, primary_key=True),
        sa.Column("damage_type_id", sa.Integer, nullable=False),
        sa.Column("target_type_id", sa.Integer, nullable=False),
        sa.Column("damage_factor", sa.Integer, nullable=False),
        sa.ForeignKeyConstraint(["type_id"], ["tipo_pokemon.type_id"]),
    )


def downgrade() -> None:
    op.drop_table("debilidades_tipo")
