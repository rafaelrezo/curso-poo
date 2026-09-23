from typing import Generic, TypeVar
from dataclasses import dataclass


# Nesta variante, igualdade e hash são escritos explicitamente abaixo.
@dataclass(frozen=True, eq=False)
class IdSensor:
    valor: str

    def __post_init__(self):
        if not self.valor:
            raise ValueError("tag vazia")

    # A igualdade depende da tag, como no catálogo de medições.
    def __eq__(self, outro):
        if not isinstance(outro, IdSensor):
            return NotImplemented
        return self.valor == outro.valor

    def __hash__(self):
        # Tags iguais devem produzir hashes iguais para a busca no dict.
        return hash(self.valor)


# T descreve o tipo dos itens; as anotações não validam tipos na execução.
T = TypeVar("T")


class Catalogo(Generic[T]):
    def __init__(self):
        self._itens: dict[IdSensor, T] = {}

    def inserir(self, id_sensor, item):
        # Recusar antes de atribuir preserva o primeiro cadastro.
        if id_sensor in self._itens:
            return False
        self._itens[id_sensor] = item
        return True

    def buscar(self, id_sensor):
        # None representa uma chave ausente neste contrato.
        return self._itens.get(id_sensor)


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
