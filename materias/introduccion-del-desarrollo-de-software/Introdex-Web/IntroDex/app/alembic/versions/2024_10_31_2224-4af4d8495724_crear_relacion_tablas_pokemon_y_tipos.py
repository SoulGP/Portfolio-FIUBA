"""Crear relacion entre tablas pokemon y tipos

Revision ID: 4af4d8495724
Revises: 5960d76159d8
Create Date: 2024-10-31 22:24:17.951686

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "4af4d8495724"
down_revision: Union[str, None] = "5960d76159d8"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    with op.batch_alter_table("tipo_pokemon") as batch_op:
        batch_op.add_column(sa.Column("pokemon_id", sa.Integer, nullable=False))
        batch_op.create_foreign_key(
            "fk_pokemon_tipo",
            "pokemon",
            ["pokemon_id"],
            ["pokemon_id"],
        )


def downgrade() -> None:
    with op.batch_alter_table("tipo_pokemon") as batch_op:
        batch_op.drop_constraint("fk_pokemon_tipo", type_="foreignkey")
        batch_op.drop_column("pokemon_id")
