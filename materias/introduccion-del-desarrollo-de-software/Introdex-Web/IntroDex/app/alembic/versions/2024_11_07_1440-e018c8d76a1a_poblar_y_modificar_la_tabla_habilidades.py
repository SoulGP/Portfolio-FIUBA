"""poblar y modificar la tabla habilidades.

Revision ID: e018c8d76a1a
Revises: cfb8533a1845
Create Date: 2024-11-07 14:40:23.355460

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa
from sqlalchemy.sql import table, column, select
from sqlalchemy import Integer, String
import csv


# revision identifiers, used by Alembic.
revision: str = "e018c8d76a1a"
down_revision: Union[str, None] = "cfb8533a1845"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


habilidades_table = table(
    "habilidades",
    column("ability_id", Integer),
    column("identifier", String),
)


def upgrade() -> None:
    conn = op.get_bind()

    with op.batch_alter_table("habilidades", schema=None) as batch_op:
        batch_op.drop_column("is_hidden")

    def buscar_identifier_por_ability_id(ability_data, ability_id, local_language_id):
        for ability in ability_data:
            if (
                int(ability["ability_id"]) == ability_id
                and int(ability["local_language_id"]) == local_language_id
            ):
                return ability["name"]

        # Fallback a inglés si no está en español
        for ability in ability_data:
            if (
                int(ability["ability_id"]) == ability_id
                and int(ability["local_language_id"]) == 9  # inglés
            ):
                return ability["name"]

        # Valor por defecto si no se encuentra en español o inglés
        return "Unknown"

    with open(
        "data/ability_names.csv", newline="", encoding="utf-8"
    ) as ability_names_file:
        ability_data = list(csv.DictReader(ability_names_file))
        local_language_id = 7  # español

        for ability in ability_data:
            ability_id = int(ability["ability_id"])
            identifier = buscar_identifier_por_ability_id(
                ability_data, ability_id, local_language_id
            )

            exists = conn.execute(
                select(habilidades_table.c.ability_id).where(
                    habilidades_table.c.ability_id == ability_id
                )
            ).fetchone()

            if not exists:
                conn.execute(
                    habilidades_table.insert().values(
                        ability_id=ability_id,
                        identifier=identifier,
                    )
                )


def downgrade() -> None:
    conn = op.get_bind()
    conn.execute(sa.text("DELETE FROM habilidades"))

    with op.batch_alter_table("habilidades", schema=None) as batch_op:
        batch_op.add_column(sa.Column("is_hidden", sa.Boolean, nullable=False))
