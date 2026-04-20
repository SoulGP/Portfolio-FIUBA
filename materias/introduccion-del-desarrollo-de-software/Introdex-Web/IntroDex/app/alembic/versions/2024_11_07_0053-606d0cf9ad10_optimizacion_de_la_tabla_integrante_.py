"""optimizacion de la tabla integrante_equipo.

Revision ID: 606d0cf9ad10
Revises: 1120b1b433d5
Create Date: 2024-11-07 00:53:28.047997

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "606d0cf9ad10"
down_revision: Union[str, None] = "1120b1b433d5"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    with op.batch_alter_table("integrante_equipo") as batch_op:
        batch_op.add_column(sa.Column("equipo_id", sa.Integer, nullable=False))
        batch_op.create_foreign_key(
            "fk_equipo",
            "equipo",
            ["equipo_id"],
            ["id"],
        )


def downgrade() -> None:
    with op.batch_alter_table("integrante_equipo") as batch_op:
        batch_op.drop_constraint("fk_integrante_equipo", type_="foreignkey")
        batch_op.drop_column("equipo_id")
