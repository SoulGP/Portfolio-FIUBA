"""Relacion 1:N equipos y pokemon

Revision ID: cd263a7894b5
Revises: 977a277e0d31
Create Date: 2024-11-04 15:41:19.327109

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "cd263a7894b5"
down_revision: Union[str, None] = None
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    with op.batch_alter_table("pokemon") as batch_op:
        batch_op.add_column(sa.Column("team_id", sa.Integer))
        batch_op.create_foreign_key("fk_pokemon_equipo", "equipo", ["team_id"], ["id"])


def downgrade() -> None:
    with op.batch_alter_table("pokemon") as batch_op:
        batch_op.drop_constraint("fk_pokemon_equipo", type_="foreignkey")
        batch_op.drop_column("team_id")
