"""Crear tabla integrante de equipo

Revision ID: bf6b6a6a3f1a
Revises: cd263a7894b5
Create Date: 2024-11-04 22:14:47.362131

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "bf6b6a6a3f1a"
down_revision: Union[str, None] = None
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.create_table(
        "integrante_equipo",
        sa.Column("member_id", sa.Integer, primary_key=True),
        sa.Column("pokemon_id", sa.Integer, nullable=False),
        sa.Column("move_id", sa.Integer, nullable=False),
        sa.Column("nature_id", sa.Integer, nullable=False),
        sa.ForeignKeyConstraint(["pokemon_id"], ["pokemon.pokemon_id"]),
        sa.ForeignKeyConstraint(["move_id"], ["movimiento.move_id"]),
        sa.ForeignKeyConstraint(["nature_id"], ["naturaleza.id"]),
    )


def downgrade() -> None:
    op.drop_table("integrante_equipo")
