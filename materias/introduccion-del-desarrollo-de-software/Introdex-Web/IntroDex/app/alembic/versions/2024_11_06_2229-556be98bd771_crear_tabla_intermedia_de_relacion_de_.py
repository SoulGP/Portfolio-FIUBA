"""crear tabla intermedia de relacion de pokemon con grupo_huevo.

Revision ID: 556be98bd771
Revises: 73fa3b8f7679
Create Date: 2024-11-06 22:29:44.464148

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "556be98bd771"
down_revision: Union[str, None] = "73fa3b8f7679"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.create_table(
        "pokemon_grupo_huevo",
        sa.Column("species_id", sa.Integer, primary_key=True),
        sa.Column("egg_group_id", sa.Integer, primary_key=True),
        sa.ForeignKeyConstraint(
            ["species_id"], ["pokemon.species_id"], name="fk_species"
        ),
        sa.ForeignKeyConstraint(
            ["egg_group_id"], ["grupo_huevo.egg_group_id"], name="fk_egg_group"
        ),
    )


def downgrade() -> None:
    op.drop_table("pokemon_grupo_huevo")
