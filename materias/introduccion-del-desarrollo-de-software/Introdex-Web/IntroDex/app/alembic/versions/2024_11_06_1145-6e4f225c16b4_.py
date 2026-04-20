"""empty message

Revision ID: 6e4f225c16b4
Revises: eaeb22837af1, 95e3f1435596, 73fe80d8d855, 977a277e0d31, cd263a7894b5, bf6b6a6a3f1a, b46f7011bfec, c3158af8f64a
Create Date: 2024-11-06 11:45:34.448167

"""
from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = '6e4f225c16b4'
down_revision: Union[str, None] = ('eaeb22837af1', '95e3f1435596', '73fe80d8d855', '977a277e0d31', 'cd263a7894b5', 'bf6b6a6a3f1a', 'b46f7011bfec', 'c3158af8f64a')
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    pass


def downgrade() -> None:
    pass
