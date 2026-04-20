"""optimizacion de la tabla grupo_huevo.

Revision ID: 73fa3b8f7679
Revises: 7490f94c2bb9
Create Date: 2024-11-06 22:21:33.154744

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "73fa3b8f7679"
down_revision: Union[str, None] = "7490f94c2bb9"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.drop_table("grupo_huevo")
    op.create_table(
        "grupo_huevo",
        sa.Column("egg_group_id", sa.Integer, primary_key=True),
        sa.Column("identifier", sa.Text, nullable=False),
    )


def downgrade() -> None:
    op.drop_table("grupo_huevo")
    op.create_table(
        "grupo_huevo",
        sa.Column("egg_group_id", sa.Integer, primary_key=True),
        sa.Column("species_id", sa.Integer),
        sa.Column("identifier", sa.Text, nullable=False),
        sa.ForeignKeyConstraint(
            ["species_id"], ["pokemon.species_id"], name="fk_pokemon_grupo_huevo"
        ),
    )
