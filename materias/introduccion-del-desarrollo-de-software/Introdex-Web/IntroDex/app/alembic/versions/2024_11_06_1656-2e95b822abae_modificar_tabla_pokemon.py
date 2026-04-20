"""modificar tabla pokemon.

Revision ID: 2e95b822abae
Revises: 31f0783c8bd0
Create Date: 2024-11-06 16:56:06.544706

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "2e95b822abae"
down_revision: Union[str, None] = "31f0783c8bd0"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    with op.batch_alter_table("pokemon") as batch_op:
        batch_op.drop_constraint("fk_pokemon_equipo", type_="foreignkey")
        batch_op.drop_column("team_id")
        batch_op.alter_column("evolution_id", nullable=True)
        batch_op.alter_column("imagen_evolucion", nullable=True)


def downgrade() -> None:
    with op.batch_alter_table("estadisticas") as batch_op:
        batch_op.alter_column("evolution_id", nullable=False)
        batch_op.alter_column("imagen_evolucion", nullable=False)
        batch_op.add_column("team_id")
        batch_op.ForeignKeyConstraint("fk_pokemon_equipo", type_="foreignkey")
