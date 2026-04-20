"""Crear tabla pokemons

Revision ID: 5c3e54bb0b25
Revises: 
Create Date: 2024-10-30 16:36:31.075985

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "5c3e54bb0b25"
down_revision: Union[str, None] = None
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.create_table(
        "pokemon",
        sa.Column("pokemon_id", sa.Integer, primary_key=True),
        sa.Column("imagen", sa.Text, nullable=False),
        sa.Column("identifier", sa.Text, nullable=False),
        sa.Column("height", sa.Integer, nullable=False),
        sa.Column("weight", sa.Integer, nullable=False),
        sa.Column("species_id", sa.Integer, nullable=False),
        sa.Column("evolution_id", sa.Integer, nullable=False),
        sa.Column("imagen_evolucion", sa.Text, nullable=False),
        sa.Column("generation_id", sa.Integer, nullable=False),
    )


def downgrade() -> None:
    op.drop_table("pokemon")
