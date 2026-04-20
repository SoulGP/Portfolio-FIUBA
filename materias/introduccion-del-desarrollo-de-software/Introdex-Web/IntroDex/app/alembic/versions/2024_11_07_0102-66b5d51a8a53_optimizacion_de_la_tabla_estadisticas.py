"""optimizacion de la tabla estadisticas.

Revision ID: 66b5d51a8a53
Revises: 606d0cf9ad10
Create Date: 2024-11-07 01:02:20.509683

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "66b5d51a8a53"
down_revision: Union[str, None] = "606d0cf9ad10"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    with op.batch_alter_table("estadisticas") as batch_op:
        batch_op.alter_column("member_id", primary_key=True)


def downgrade() -> None:
    with op.batch_alter_table("estadisticas") as batch_op:
        batch_op.alter_column("member_id", primary_key=False)
