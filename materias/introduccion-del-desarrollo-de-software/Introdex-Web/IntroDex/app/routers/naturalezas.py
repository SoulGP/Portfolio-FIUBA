from fastapi import APIRouter, HTTPException, status
from app.modelos import Naturaleza
from sqlmodel import select
from app.database import SessionDep

router = APIRouter()


@router.get("/", response_model=list[Naturaleza])
def obtener_naturalezas(session: SessionDep):
    query = select(Naturaleza).order_by(Naturaleza.id)
    naturalezas = session.exec(query).all()
    if not naturalezas:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND, detail="Naturalezas no encontradas"
        )
    return naturalezas
