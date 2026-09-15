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


class SensorNivel:
    def __init__(self, valor):
        self._valor = valor

    def valor(self):
        return self._valor


def main():
    sensor = SensorNivel(12)
    catalogo = Catalogo()
    catalogo.inserir(IdSensor("LT-101"), Medicao(sensor.valor(), "%"))
    print(f"Registros: {catalogo.quantidade()}")
    print(f"Sensor: {sensor.valor()} %")


if __name__ == "__main__":
    main()
