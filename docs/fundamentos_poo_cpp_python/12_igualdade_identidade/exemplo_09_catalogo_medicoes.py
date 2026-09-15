from dataclasses import dataclass


@dataclass(frozen=True)
class IdSensor:
    valor: str

    def __post_init__(self):
        if not self.valor:
            raise ValueError("tag vazia")


@dataclass(frozen=True)
class Medicao:
    valor: float
    unidade: str


class Catalogo:
    def __init__(self):
        self._itens = {}

    def inserir(self, id, item):
        if id in self._itens:
            return False
        self._itens[id] = item
        return True

    def buscar(self, id):
        return self._itens.get(id)

    def quantidade(self):
        return len(self._itens)


def main():
    catalogo = Catalogo()
    print(f"Primeira: {catalogo.inserir(IdSensor('LT-101'), Medicao(12, '%'))}")
    print(f"Duplicada: {catalogo.inserir(IdSensor('LT-101'), Medicao(99, '%'))}")
    item = catalogo.buscar(IdSensor("LT-101"))
    if item is not None:
        print(f"Preservada: {item.valor} {item.unidade}")
    print(f"Ausente: {catalogo.buscar(IdSensor('LT-999')) is None}")
    print(f"Quantidade: {catalogo.quantidade()}")


if __name__ == "__main__":
    main()
