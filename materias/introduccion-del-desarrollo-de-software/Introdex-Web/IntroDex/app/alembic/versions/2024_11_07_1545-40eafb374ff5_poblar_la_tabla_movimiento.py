"""poblar la tabla movimiento.

Revision ID: 40eafb374ff5
Revises: 8a4abb5ab4cd
Create Date: 2024-11-07 15:45:24.138649

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa
from sqlalchemy import table, column, Integer, String
import csv


# revision identifiers, used by Alembic.
revision: str = "40eafb374ff5"
down_revision: Union[str, None] = "8a4abb5ab4cd"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


movimiento_table = table(
    "movimiento",
    column("move_id", Integer),
    column("identifier", String),
    column("type_id", Integer),
    column("damage_class_id", Integer),
    column("power", Integer),
    column("accuracy", Integer),
    column("pp", Integer),
    column("generation_id", Integer),
    column("effect_id", Integer),
)


def buscar_identifier_por_move_id(move_names_data, move_id, local_language_id):
    for move_name in move_names_data:
        if (
            int(move_name["move_id"]) == move_id
            and int(move_name["local_language_id"]) == local_language_id
        ):
            return move_name["name"]

    for move_name in move_names_data:
        if (
            int(move_name["move_id"]) == move_id
            and int(move_name["local_language_id"]) == 9  # ingles
        ):
            return move_name["name"]

    return "Unknown"


def upgrade() -> None:
    conn = op.get_bind()

    with op.batch_alter_table("movimiento") as batch_op:
        batch_op.alter_column("power", existing_type=Integer, nullable=True)
        batch_op.alter_column("accuracy", existing_type=Integer, nullable=True)
        batch_op.alter_column("pp", existing_type=Integer, nullable=True)

    with open("data/moves.csv", newline="", encoding="utf-8") as moves_file, open(
        "data/move_names.csv", newline="", encoding="utf-8"
    ) as move_names_file:

        moves_data = csv.DictReader(moves_file)
        move_names_data = list(csv.DictReader(move_names_file))
        local_language_id = 7  # español

        for move in moves_data:
            move_id = int(move["id"])
            identifier = buscar_identifier_por_move_id(
                move_names_data, move_id, local_language_id
            )
            type_id = int(move["type_id"])
            damage_class_id = int(move["damage_class_id"])
            power = int(move["power"]) if move["power"] else None
            accuracy = int(move["accuracy"]) if move["accuracy"] else None
            pp = int(move["pp"]) if move["pp"] else None
            generation_id = int(move["generation_id"])
            effect_id = int(move["effect_id"])

            conn.execute(
                movimiento_table.insert().values(
                    move_id=move_id,
                    identifier=identifier,
                    type_id=type_id,
                    damage_class_id=damage_class_id,
                    power=power,
                    accuracy=accuracy,
                    pp=pp,
                    generation_id=generation_id,
                    effect_id=effect_id,
                )
            )


def downgrade() -> None:
    conn = op.get_bind()
    conn.execute(sa.text("DELETE FROM movimiento"))

    with op.batch_alter_table("movimiento") as batch_op:
        batch_op.alter_column("power", existing_type=Integer, nullable=False)
        batch_op.alter_column("accuracy", existing_type=Integer, nullable=False)
        batch_op.alter_column("pp", existing_type=Integer, nullable=False)
