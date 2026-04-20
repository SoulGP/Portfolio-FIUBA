"""Crear relacion entre tablas pokemon y grupo huevo

Revision ID: 3eda3b8c2e24
Revises: 1bc581e21f47
Create Date: 2024-10-31 23:04:49.021207

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "3eda3b8c2e24"
down_revision: Union[str, None] = "1bc581e21f47"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    with op.batch_alter_table("grupo_huevo") as batch_op:
        batch_op.add_column(sa.Column("species_id", sa.Integer, nullable=False)),
        batch_op.create_foreign_key(
            "fk_pokemon_grupo_huevo",
            "pokemon",
            ["species_id"],
            ["species_id"],
        )


def downgrade() -> None:
    with op.batch_alter_table("grupo_huevo") as batch_op:
        batch_op.drop_constraint("fk_pokemon_grupo_huevo", type_="foreignkey")
        batch_op.drop_column("species_id")
