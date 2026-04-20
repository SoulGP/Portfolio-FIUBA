"""poblar la tabla grupo_huevo.

Revision ID: 27fe9196bba0
Revises: e018c8d76a1a
Create Date: 2024-11-07 15:15:31.683558

"""

from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa
from sqlalchemy import select, table, column, Integer, String
import csv


# revision identifiers, used by Alembic.
revision: str = "27fe9196bba0"
down_revision: Union[str, None] = "e018c8d76a1a"
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


grupo_huevo_table = table(
    "grupo_huevo",
    column("egg_group_id", Integer),
    column("identifier", String),
)


def upgrade() -> None:
    conn = op.get_bind()

    # Función para buscar el nombre del grupo huevo en español (o inglés si no está disponible)
    def buscar_identifier_por_egg_group_id(
        egg_group_data, egg_group_id, local_language_id
    ):
        for egg_group in egg_group_data:
            if (
                int(egg_group["egg_group_id"]) == egg_group_id
                and int(egg_group["local_language_id"]) == local_language_id
            ):
                return egg_group["name"]

        # Fallback a inglés si no está en español
        for egg_group in egg_group_data:
            if (
                int(egg_group["egg_group_id"]) == egg_group_id
                and int(egg_group["local_language_id"]) == 9  # inglés
            ):
                return egg_group["name"]

        # Valor por defecto si no se encuentra en español o inglés
        return "Unknown"

    with open("data/egg_group.csv", newline="", encoding="utf-8") as egg_group_file:
        egg_group_data = list(csv.DictReader(egg_group_file))
        local_language_id = 7  # español

        for egg_group in egg_group_data:
            egg_group_id = int(egg_group["egg_group_id"])
            identifier = buscar_identifier_por_egg_group_id(
                egg_group_data, egg_group_id, local_language_id
            )

            exists = conn.execute(
                select(grupo_huevo_table.c.egg_group_id).where(
                    grupo_huevo_table.c.egg_group_id == egg_group_id
                )
            ).fetchone()

            if not exists:
                conn.execute(
                    grupo_huevo_table.insert().values(
                        egg_group_id=egg_group_id,
                        identifier=identifier,
                    )
                )


def downgrade() -> None:
    conn = op.get_bind()
    conn.execute(sa.text("DELETE FROM grupo_huevo"))
