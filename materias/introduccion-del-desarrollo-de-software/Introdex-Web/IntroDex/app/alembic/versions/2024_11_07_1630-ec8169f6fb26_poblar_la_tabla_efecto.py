"""poblar la tabla efecto.

Revision ID: ec8169f6fb26
Revises: 34651d86464b
Create Date: 2024-11-07 16:30:30.242867

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa
from sqlalchemy.sql import table, column, select
import csv


# revision identifiers, used by Alembic.
revision: str = "ec8169f6fb26"
down_revision: Union[str, None] = "34651d86464b"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


efecto_table = table(
    "efecto",
    column("effect_id", sa.Integer),
    column("description", sa.String),
)


def upgrade() -> None:
    conn = op.get_bind()

    with open("data/move_effect.csv", newline="", encoding="utf-8") as effect_file:
        effect_data = list(csv.DictReader(effect_file))

        for effect in effect_data:
            effect_id = int(effect["move_effect_id"])
            description = effect["short_effect"]

            conn.execute(
                efecto_table.insert().values(
                    effect_id=effect_id,
                    description=description,
                )
            )


def downgrade() -> None:
    conn = op.get_bind()
    conn.execute(sa.text("DELETE FROM efecto"))
