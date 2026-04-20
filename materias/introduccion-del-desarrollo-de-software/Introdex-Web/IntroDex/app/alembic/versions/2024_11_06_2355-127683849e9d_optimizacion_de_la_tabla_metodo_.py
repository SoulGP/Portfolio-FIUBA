"""optimizacion de la tabla metodo_aprender_movimiento.

Revision ID: 127683849e9d
Revises: 72ee0b236030
Create Date: 2024-11-06 23:55:54.450891

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "127683849e9d"
down_revision: Union[str, None] = "72ee0b236030"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.drop_table("metodo_aprender_movimiento")
    op.create_table(
        "metodo_aprender_movimiento",
        sa.Column("pokemon_move_method_id", sa.Integer, primary_key=True),
        sa.Column("identifier", sa.Text, nullable=False),
    )


def downgrade() -> None:
    op.drop_table("metodo_aprender_movimiento")
    op.create_table(
        "metodo_aprender_movimiento",
        sa.Column("move_id", sa.Integer, primary_key=True),
        sa.Column("pokemon_move_method_id", sa.Integer, nullable=False),
        sa.ForeignKeyConstraint(["move_id"], ["movimientos_aprendibles.move_id"]),
    )
