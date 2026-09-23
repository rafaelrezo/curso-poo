from dataclasses import dataclass


# Gera igualdade e hash pela tag; frozen impede sua reatribuição usual.
@dataclass(frozen=True)
class IdSensor:
    valor: str

    # Executado após o __init__ gerado pelo decorador.
    def __post_init__(self):
        if not self.valor:
            raise ValueError("tag vazia")


@dataclass(frozen=True)
class Medicao:
    valor: float
    unidade: str


class Catalogo:
    def __init__(self):
        # Chaves: IdSensor. Valores: objetos Medicao.
        self._itens = {}

    def inserir(self, id_sensor, item):
        # in consulta as chaves usando hash e igualdade.
        if id_sensor in self._itens:
            return False  # Preserva a medição cadastrada anteriormente.
        self._itens[id_sensor] = item
        return True

    def buscar(self, id_sensor):
        # get comunica ausência com None, sem criar uma entrada.
        return self._itens.get(id_sensor)

    def quantidade(self):
        return len(self._itens)


def main():
    catalogo = Catalogo()
    print(f"Primeira: {catalogo.inserir(IdSensor('LT-101'), Medicao(12, '%'))}")
    # Outra leitura não torna a mesma tag uma nova chave.
    print(f"Duplicada: {catalogo.inserir(IdSensor('LT-101'), Medicao(99, '%'))}")
    # A busca aceita outra instância que represente a mesma tag.
    item = catalogo.buscar(IdSensor("LT-101"))
    # Confirme a presença antes de acessar os campos da medição.
    if item is not None:
        print(f"Preservada: {item.valor} {item.unidade}")
    print(f"Ausente: {catalogo.buscar(IdSensor('LT-999')) is None}")
    print(f"Quantidade: {catalogo.quantidade()}")


if __name__ == "__main__":
    main()
