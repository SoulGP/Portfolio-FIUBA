"""poblar la tabla categoria.

Revision ID: 34651d86464b
Revises: 40eafb374ff5
Create Date: 2024-11-07 16:19:08.124967

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa
from sqlalchemy.sql import table, column, select
import csv


# revision identifiers, used by Alembic.
revision: str = "34651d86464b"
down_revision: Union[str, None] = "40eafb374ff5"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


categoria_table = table(
    "categoria",
    column("move_damage_class_id", sa.Integer),
    column("identifier", sa.String),
)


def buscar_identifier_por_damage_class_id(
    damage_class_data, move_damage_class_id, local_language_id
):
    for damage_class in damage_class_data:
        if (
            int(damage_class["move_damage_class_id"]) == move_damage_class_id
            and int(damage_class["local_language_id"]) == local_language_id
        ):
            return damage_class["name"]

    for damage_class in damage_class_data:
        if (
            int(damage_class["move_damage_class_id"]) == move_damage_class_id
            and int(damage_class["local_language_id"]) == 9  # ingles
        ):
            return damage_class["name"]

    return "Unknown"


def upgrade():
    conn = op.get_bind()

    with open(
        "data/move_damage_class.csv", newline="", encoding="utf-8"
    ) as damage_class_file:
        damage_class_data = list(csv.DictReader(damage_class_file))
        local_language_id = 7  # español

        for damage_class in damage_class_data:
            move_damage_class_id = int(damage_class["move_damage_class_id"])
            identifier = buscar_identifier_por_damage_class_id(
                damage_class_data, move_damage_class_id, local_language_id
            )

            exists = conn.execute(
                select(categoria_table.c.move_damage_class_id).where(
                    categoria_table.c.move_damage_class_id == move_damage_class_id
                )
            ).fetchone()

            if not exists:
                conn.execute(
                    categoria_table.insert().values(
                        move_damage_class_id=move_damage_class_id,
                        identifier=identifier,
                    )
                )


def downgrade():
    conn = op.get_bind()
    conn.execute(sa.text("DELETE FROM categoria"))
