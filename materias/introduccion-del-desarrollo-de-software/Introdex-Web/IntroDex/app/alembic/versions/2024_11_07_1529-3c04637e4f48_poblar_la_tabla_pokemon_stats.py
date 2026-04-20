"""poblar la tabla pokemon_stats.

Revision ID: 3c04637e4f48
Revises: fe34cf0dbcd5
Create Date: 2024-11-07 15:29:12.442837

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa
from sqlalchemy import table, column, Integer
import csv


# revision identifiers, used by Alembic.
revision: str = "3c04637e4f48"
down_revision: Union[str, None] = "fe34cf0dbcd5"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


pokemon_stats_table = table(
    "pokemon_stats",
    column("pokemon_id", Integer),
    column("stat_id", Integer),
    column("base_stat", Integer),
)


def upgrade() -> None:
    conn = op.get_bind()

    with open("data/pokemon_stats.csv", newline="", encoding="utf-8") as stats_file:
        stats_data = csv.DictReader(stats_file)

        for row in stats_data:
            pokemon_id = int(row["pokemon_id"])
            stat_id = int(row["stat_id"])
            base_stat = int(row["base_stat"])

            conn.execute(
                pokemon_stats_table.insert().values(
                    pokemon_id=pokemon_id,
                    stat_id=stat_id,
                    base_stat=base_stat,
                )
            )


def downgrade() -> None:
    conn = op.get_bind()
    conn.execute(sa.text("DELETE FROM pokemon_stats"))
