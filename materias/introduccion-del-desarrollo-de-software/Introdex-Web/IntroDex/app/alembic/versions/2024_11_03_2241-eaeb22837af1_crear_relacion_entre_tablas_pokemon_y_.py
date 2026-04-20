"""Crear relacion entre tablas pokemon y movimientos aprendibles

Revision ID: eaeb22837af1
Revises: d0775838aee7
Create Date: 2024-11-03 22:41:42.310206

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "eaeb22837af1"
down_revision: Union[str, None] = "d0775838aee7"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    with op.batch_alter_table("movimientos_aprendibles") as batch_op:
        batch_op.add_column(sa.Column("pokemon_id", sa.Integer, nullable=False))
        batch_op.create_foreign_key(
            "fk_pokemon_movimiento_aprendible",
            "pokemon",
            ["pokemon_id"],
            ["pokemon_id"],
        )


def downgrade() -> None:
    with op.batch_alter_table("movimientos_aprendibles") as batch_op:
        batch_op.drop_column("pokemon_id")
        batch_op.drop_constraint("fk_pokemon_movimiento_aprendible", type_="foreingkey")
