"""crear tabla intermedia de relacion de pokemon con stats.

Revision ID: 9e9e2157bc32
Revises: add47043580e
Create Date: 2024-11-06 22:40:34.658923

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "9e9e2157bc32"
down_revision: Union[str, None] = "add47043580e"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.create_table(
        "pokemon_stats",
        sa.Column("pokemon_id", sa.Integer, primary_key=True),
        sa.Column("stat_id", sa.Integer, primary_key=True),
        sa.Column("base_stat", sa.Integer),
        sa.ForeignKeyConstraint(
            ["pokemon_id"], ["pokemon.pokemon_id"], name="fk_pokemon"
        ),
        sa.ForeignKeyConstraint(["stat_id"], ["stats.stat_id"], name="fk_stats"),
    )


def downgrade() -> None:
    op.drop_table("pokemon_stats")
