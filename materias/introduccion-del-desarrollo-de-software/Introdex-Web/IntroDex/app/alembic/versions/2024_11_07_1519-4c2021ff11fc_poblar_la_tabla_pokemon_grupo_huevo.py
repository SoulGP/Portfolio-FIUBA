"""poblar la tabla pokemon_grupo_huevo.

Revision ID: 4c2021ff11fc
Revises: 27fe9196bba0
Create Date: 2024-11-07 15:19:22.681853

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa
from sqlalchemy import table, column, Integer
import csv


# revision identifiers, used by Alembic.
revision: str = "4c2021ff11fc"
down_revision: Union[str, None] = "27fe9196bba0"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


pokemon_grupo_huevo_table = table(
    "pokemon_grupo_huevo",
    column("species_id", Integer),
    column("egg_group_id", Integer),
)


def upgrade() -> None:
    conn = op.get_bind()

    with open(
        "data/pokemon_egg_groups.csv", newline="", encoding="utf-8"
    ) as egg_groups_file:
        egg_groups_data = csv.DictReader(egg_groups_file)

        for row in egg_groups_data:
            species_id = int(row["species_id"])
            egg_group_id = int(row["egg_group_id"])

            conn.execute(
                pokemon_grupo_huevo_table.insert().values(
                    species_id=species_id,
                    egg_group_id=egg_group_id,
                )
            )


def downgrade() -> None:
    conn = op.get_bind()
    conn.execute(sa.text("DELETE FROM pokemon_grupo_huevo"))
