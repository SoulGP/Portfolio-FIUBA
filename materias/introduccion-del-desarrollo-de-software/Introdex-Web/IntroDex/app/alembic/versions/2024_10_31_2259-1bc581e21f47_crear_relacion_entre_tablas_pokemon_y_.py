"""Crear relacion entre tablas pokemon y habilidades

Revision ID: 1bc581e21f47
Revises: 4af4d8495724
Create Date: 2024-10-31 22:59:10.681713

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "1bc581e21f47"
down_revision: Union[str, None] = "4af4d8495724"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    with op.batch_alter_table("habilidades") as batch_op:
        batch_op.add_column(sa.Column("pokemon_id", sa.Integer, nullable=False))
        batch_op.create_foreign_key(
            "fk_pokemon_habilidad",
            "pokemon",
            ["pokemon_id"],
            ["pokemon_id"],
        )


def downgrade() -> None:
    with op.batch_alter_table("habilidades") as batch_op:
        batch_op.drop_constraint("fk_pokemon_habilidad", type_="foreignkey"),
        batch_op.drop_column("pokemon_id")
