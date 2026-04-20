"""poblar la tabla naturaleza.

Revision ID: 9ea1e2785e88
Revises: a58fa4ef5f91
Create Date: 2024-11-07 02:25:04.855618

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa
import csv
from sqlalchemy.sql import table, column
from sqlalchemy import Integer, String


# revision identifiers, used by Alembic.
revision: str = "9ea1e2785e88"
down_revision: Union[str, None] = "a58fa4ef5f91"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None

naturaleza_table = table(
    "naturaleza",
    column("id", Integer),
    column("identifier", String),
    column("decreased_stat_id", Integer),
    column("increased_stat_id", Integer),
)


def upgrade() -> None:
    conn = op.get_bind()

    with open("data/natures.csv", newline="") as csvfile:
        nature_data = csv.DictReader(csvfile)
        for naturaleza in nature_data:
            conn.execute(
                naturaleza_table.insert().values(
                    id=int(naturaleza["id"]),
                    identifier=naturaleza["identifier"],
                    decreased_stat_id=int(naturaleza["decreased_stat_id"]),
                    increased_stat_id=int(naturaleza["increased_stat_id"]),
                )
            )


def downgrade() -> None:
    conn = op.get_bind()
    conn.execute(sa.text("DELETE FROM naturaleza"))
