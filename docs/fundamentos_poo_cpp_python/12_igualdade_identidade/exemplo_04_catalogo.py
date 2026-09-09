from typing import Generic, TypeVar
from dataclasses import dataclass


@dataclass(frozen=True, eq=False)
class IdSensor:
    valor: str

    def __post_init__(self):
        if not self.valor:
            raise ValueError("tag vazia")

    def __eq__(self, outro):
        if not isinstance(outro, IdSensor):
            return NotImplemented
        return self.valor == outro.valor

    def __hash__(self):
        return hash(self.valor)


T = TypeVar("T")


class Catalogo(Generic[T]):
    def __init__(self):
        self._itens: dict[IdSensor, T] = {}

    def inserir(self, id, item):
        if id in self._itens:
            return False
        self._itens[id] = item
        return True

    def buscar(self, id):
        return self._itens.get(id)


def main():
    catalogo = Catalogo[str]()
    primeira = catalogo.inserir(IdSensor("LT-101"), "Bancada A")
    repetida = catalogo.inserir(IdSensor("LT-101"), "Bancada B")
    nome = catalogo.buscar(IdSensor("LT-101"))
    print(primeira, repetida)
    if nome is not None:
        print(nome)


if __name__ == "__main__":
    main()
