from collections.abc import Generator
import logging
from typing import Annotated

from fastapi import Depends
from sqlmodel import create_engine, Session


SQLITE_FILE_PATH = "app/database.db"

engine = create_engine(f"sqlite:///{SQLITE_FILE_PATH}")


def get_session() -> Generator[Session, None, None]:
    with Session(engine) as session:
        yield session


SessionDep = Annotated[Session, Depends(get_session)]

logger = logging.getLogger(__name__)
