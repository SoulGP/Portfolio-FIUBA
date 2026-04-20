"""poblar la tabla tipos.

Revision ID: d5cee5dab0a5
Revises: 2fc58a718849
Create Date: 2024-11-07 12:56:30.660492

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa
import csv
from sqlalchemy.sql import table, column
from sqlalchemy import Integer, String
from sqlalchemy import select


# revision identifiers, used by Alembic.
revision: str = "d5cee5dab0a5"
down_revision: Union[str, None] = "2fc58a718849"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


tipos_table = table(
    "tipos",
    column("type_id", Integer),
    column("identifier", String),
)


def upgrade() -> None:
    conn = op.get_bind()

    def buscar_identifier_por_type_id(type_names_data, type_id, local_language_id):
        for type_name in type_names_data:
            if (
                int(type_name["type_id"]) == type_id
                and int(type_name["local_language_id"]) == local_language_id
            ):
                return type_name["name"]

        # Fallback a inglés si no está en español
        for type_name in type_names_data:
            if (
                int(type_name["type_id"]) == type_id
                and int(type_name["local_language_id"]) == 9  # inglés
            ):
                return type_name["name"]

        # Valor por defecto si no se encuentra en español o inglés
        return "Unknown"

    with open("data/type_names.csv", newline="", encoding="utf-8") as type_names_file:
        type_names_data = list(csv.DictReader(type_names_file))
        local_language_id = 7  # español

        for type_name in type_names_data:
            type_id = int(type_name["type_id"])
            identifier = buscar_identifier_por_type_id(
                type_names_data, type_id, local_language_id
            )

            exists = conn.execute(
                select(tipos_table.c.type_id).where(tipos_table.c.type_id == type_id)
            ).fetchone()

            if not exists:
                conn.execute(
                    tipos_table.insert().values(
                        type_id=type_id,
                        identifier=identifier,
                    )
                )


def downgrade() -> None:
    conn = op.get_bind()
    conn.execute(sa.text("DELETE FROM tipos"))
