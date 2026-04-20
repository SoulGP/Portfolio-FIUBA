"""Crear relacion entre tablas pokemon y stats

Revision ID: d0775838aee7
Revises: 3eda3b8c2e24
Create Date: 2024-11-03 22:33:44.600203

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "d0775838aee7"
down_revision: Union[str, None] = "3eda3b8c2e24"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    with op.batch_alter_table("stats") as batch_op:
        batch_op.add_column(sa.Column("pokemon_id", sa.Integer, nullable=False))
        batch_op.create_foreign_key(
            "fk_pokemon_stat",
            "pokemon",
            ["pokemon_id"],
            ["pokemon_id"],
        )


def downgrade() -> None:
    with op.batch_alter_table("stats") as batch_op:
        batch_op.drop_column("pokemon_id")
        batch_op.drop_constraint("fk_pokemon_stat", type_="foreingkey")
