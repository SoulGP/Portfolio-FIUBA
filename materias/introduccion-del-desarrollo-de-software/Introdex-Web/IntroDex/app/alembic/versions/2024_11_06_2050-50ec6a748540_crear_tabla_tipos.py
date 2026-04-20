"""crear tabla tipos.

Revision ID: 50ec6a748540
Revises: 2e95b822abae
Create Date: 2024-11-06 20:50:06.175294

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = "50ec6a748540"
down_revision: Union[str, None] = "2e95b822abae"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    op.create_table(
        "tipos",
        sa.Column("type_id", sa.Integer, primary_key=True),
        sa.Column("identifier", sa.String, nullable=False),
    )


def downgrade() -> None:
    op.drop_table("tipos")
