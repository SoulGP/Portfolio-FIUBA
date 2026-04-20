"""crear tabla intermedia de relacion para pokemon y tipos.

Revision ID: 8369ac03b64b
Revises: 50ec6a748540
Create Date: 2024-11-06 20:53:09.669338

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "8369ac03b64b"
down_revision: Union[str, None] = "50ec6a748540"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade():
    op.drop_table("tipo_pokemon")
    op.create_table(
        "tipo_pokemon",
        sa.Column("type_id", sa.Integer, primary_key=True),
        sa.Column("pokemon_id", sa.Integer, primary_key=True),
        sa.ForeignKeyConstraint(["type_id"], ["tipos.type_id"], name="fk_tipo"),
        sa.ForeignKeyConstraint(
            ["pokemon_id"], ["pokemon.pokemon_id"], name="fk_pokemon"
        ),
    )


def downgrade():
    op.drop_table("tipo_pokemon")
    op.create_table(
        "tipo_pokemon",
        sa.Column("type_id", sa.Integer, primary_key=True),
        sa.Column(
            "pokemon_id",
            sa.Integer,
            sa.ForeignKey("pokemon.pokemon_id"),
            nullable=False,
        ),
    )
