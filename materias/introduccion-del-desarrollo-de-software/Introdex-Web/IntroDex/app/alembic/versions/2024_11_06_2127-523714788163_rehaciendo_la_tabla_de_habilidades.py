"""Rehaciendo la tabla de habilidades

Revision ID: 523714788163
Revises: 8369ac03b64b
Create Date: 2024-11-06 21:27:13.781681

"""
from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = '523714788163'
down_revision: Union[str, None] = '8369ac03b64b'
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.drop_table('habilidades')
    op.create_table(
        'habilidades',
        sa.Column('ability_id', sa.Integer(), nullable=False),
        sa.Column('identifier', sa.String(length=50), nullable=False),
        sa.Column('is_hidden', sa.Boolean(), nullable=False),
        sa.PrimaryKeyConstraint('ability_id')
    )
        


def downgrade() -> None:
    op.drop_table('habilidades')
    op.create_table(
        "habilidades",
        sa.Column("ability_id", sa.Integer, primary_key=True),
        sa.Column("is_hiden", sa.Integer, nullable=False),
        sa.Column("pokemon_id", sa.Integer, sa.ForeignKey("pokemon.pokemon_id"), nullable=False)
    )
