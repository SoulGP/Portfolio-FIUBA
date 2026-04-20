"""poblar la tabla debilidades_tipo.

Revision ID: 8a4abb5ab4cd
Revises: 3c04637e4f48
Create Date: 2024-11-07 15:31:37.693001

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa
from sqlalchemy import table, column, Integer
import csv


# revision identifiers, used by Alembic.
revision: str = "8a4abb5ab4cd"
down_revision: Union[str, None] = "3c04637e4f48"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


debilidades_tipo_table = table(
    "debilidades_tipo",
    column("damage_type_id", Integer),
    column("target_type_id", Integer),
    column("damage_factor", Integer),
)


def upgrade() -> None:
    conn = op.get_bind()

    with open("data/type_efficacy.csv", newline="", encoding="utf-8") as efficacy_file:
        efficacy_data = csv.DictReader(efficacy_file)

        for efficacy in efficacy_data:
            damage_type_id = int(efficacy["damage_type_id"])
            target_type_id = int(efficacy["target_type_id"])
            damage_factor = int(efficacy["damage_factor"])

            conn.execute(
                debilidades_tipo_table.insert().values(
                    damage_type_id=damage_type_id,
                    target_type_id=target_type_id,
                    damage_factor=damage_factor,
                )
            )


def downgrade() -> None:
    conn = op.get_bind()
    conn.execute(sa.text("DELETE FROM debilidades_tipo"))
