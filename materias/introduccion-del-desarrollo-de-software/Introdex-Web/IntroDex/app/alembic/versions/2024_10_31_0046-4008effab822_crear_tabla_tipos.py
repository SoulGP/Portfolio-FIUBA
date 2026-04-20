"""Crear tabla tipos

Revision ID: 4008effab822
Revises: 5c3e54bb0b25
Create Date: 2024-10-31 00:46:18.591037

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "4008effab822"
down_revision: Union[str, None] = "5c3e54bb0b25"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.create_table(
        "tipo_pokemon",
        sa.Column("type_id", sa.Integer, primary_key=True),
    )


def downgrade() -> None:
    op.drop_table("tipo_pokemon")
