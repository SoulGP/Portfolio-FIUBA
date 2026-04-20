"""poblar la tabla stats.

Revision ID: fe34cf0dbcd5
Revises: 4c2021ff11fc
Create Date: 2024-11-07 15:23:59.343846

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa
from sqlalchemy import table, column, Integer, String
import csv


# revision identifiers, used by Alembic.
revision: str = "fe34cf0dbcd5"
down_revision: Union[str, None] = "4c2021ff11fc"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


stats_table = table(
    "stats",
    column("stat_id", Integer),
    column("identifier", String),
)


def upgrade() -> None:
    conn = op.get_bind()

    with open("data/stats.csv", newline="", encoding="utf-8") as stats_file:
        stats_data = csv.DictReader(stats_file)

        for row in stats_data:
            stat_id = int(row["id"])
            identifier = row["identifier"]

            conn.execute(
                stats_table.insert().values(
                    stat_id=stat_id,
                    identifier=identifier,
                )
            )


def downgrade() -> None:
    conn = op.get_bind()
    conn.execute(sa.text("DELETE FROM stats"))
