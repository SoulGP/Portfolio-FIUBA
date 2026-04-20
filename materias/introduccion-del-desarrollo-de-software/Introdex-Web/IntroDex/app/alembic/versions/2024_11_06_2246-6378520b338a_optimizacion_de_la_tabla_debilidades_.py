"""optimizacion de la tabla debilidades_tipo.

Revision ID: 6378520b338a
Revises: 9e9e2157bc32
Create Date: 2024-11-06 22:46:51.487414

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "6378520b338a"
down_revision: Union[str, None] = "9e9e2157bc32"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.drop_table("debilidades_tipo")
    op.create_table(
        "debilidades_tipo",
        sa.Column("damage_type_id", sa.Integer, primary_key=True),
        sa.Column("target_type_id", sa.Integer, primary_key=True),
        sa.ForeignKeyConstraint(
            ["target_type_id"], ["tipo_pokemon.type_id"], name="fk_target"
        ),
        sa.Column("damage_factor", sa.Integer),
    )


def downgrade() -> None:
    op.drop_table("debilidades_tipo")
    op.create_table(
        "debilidades_tipo",
        sa.Column("type_id", sa.Integer, primary_key=True),
        sa.Column("damage_type_id", sa.Integer),
        sa.Column("target_type_id", sa.Integer),
        sa.Column("damage_factor", sa.Integer),
        sa.ForeignKeyConstraint(["type_id"], ["tipo_pokemon.type_id"]),
    )
