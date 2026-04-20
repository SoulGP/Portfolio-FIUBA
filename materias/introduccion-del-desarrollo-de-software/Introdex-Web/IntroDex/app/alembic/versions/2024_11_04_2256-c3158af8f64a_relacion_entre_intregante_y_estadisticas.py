"""Relacion entre intregante y estadisticas

Revision ID: c3158af8f64a
Revises: b46f7011bfec
Create Date: 2024-11-04 22:56:05.257377

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "c3158af8f64a"
down_revision: Union[str, None] = None
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    with op.batch_alter_table("estadisticas") as batch_op:
        batch_op.add_column(sa.Column("member_id", sa.Integer))
        batch_op.create_foreign_key(
            "fk_integrante_estadisticas",
            "integrante_equipo",
            ["member_id"],
            ["member_id"],
        )


def downgrade() -> None:
    with op.batch_alter_table("estadisticas") as batch_op:
        batch_op.drop_column("member_id")
        batch_op.drop_constraint("fk_integrante_estadisticas", type_="foreingkey")
