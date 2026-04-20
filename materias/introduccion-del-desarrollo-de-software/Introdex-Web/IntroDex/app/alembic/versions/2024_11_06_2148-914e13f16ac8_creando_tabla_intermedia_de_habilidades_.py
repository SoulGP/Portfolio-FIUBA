"""Creando tabla intermedia de habilidades y pokemon

Revision ID: 914e13f16ac8
Revises: 523714788163
Create Date: 2024-11-06 21:48:18.153637

"""
from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = '914e13f16ac8'
down_revision: Union[str, None] = '523714788163'
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.create_table(
        "pokemon_habilidad",
        sa.Column("pokemon_id", sa.Integer, primary_key=True),
        sa.Column("ability_id", sa.Integer, primary_key=True),
        sa.ForeignKeyConstraint(["pokemon_id"], ["pokemon.pokemon_id"], name="fk_pokemon"),
        sa.ForeignKeyConstraint(["ability_id"], ["habilidades.ability_id"], name="fk_ability"),
        )



def downgrade() -> None:
    op.drop_table("pokemon_habilidad")
