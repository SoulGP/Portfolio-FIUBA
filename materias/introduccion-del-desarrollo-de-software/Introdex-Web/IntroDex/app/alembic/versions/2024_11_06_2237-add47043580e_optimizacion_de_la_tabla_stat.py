"""optimizacion de la tabla stat.

Revision ID: add47043580e
Revises: 556be98bd771
Create Date: 2024-11-06 22:37:07.799147

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "add47043580e"
down_revision: Union[str, None] = "556be98bd771"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.drop_table("stats")
    op.create_table(
        "stats",
        sa.Column("stat_id", sa.Integer, primary_key=True),
        sa.Column("identifier", sa.Text, nullable=False),
    )


def downgrade() -> None:
    op.drop_table("stats")
    op.create_table(
        "stats",
        sa.Column("stat_id", sa.Integer, primary_key=True),
        sa.Column("base_stat", sa.Integer),
        sa.Column("pokemon_id", sa.Integer),
        sa.ForeignKeyConstraint(
            ["pokemon_id"], ["pokemon.pokemon_id"], name="fk_pokemon_stat"
        ),
    )
