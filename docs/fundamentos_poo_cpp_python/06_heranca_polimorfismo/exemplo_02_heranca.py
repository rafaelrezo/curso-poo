from math import isfinite

class Sensor:
    def __init__(self, tag: str):
        if not tag:
            raise ValueError("tag vazia")
        self._tag = tag
    @property
    def tag(self):
        return self._tag

class SensorNivel(Sensor):
    def __init__(self, tag: str, valor: float = 50):
        super().__init__(tag)
        if not isfinite(valor) or not 0 <= valor <= 100:
            raise ValueError("nivel fora da faixa")
        self._valor = valor
    def valor(self):
        return self._valor
    def unidade(self):
        return "%"
    def atualizar(self, valor):
        if not isfinite(valor) or not 0 <= valor <= 100:
            return False
        self._valor = valor
        return True


def main():
    nivel = SensorNivel("LT-101", 50)
    print(f"{nivel.tag}: {nivel.valor()} {nivel.unidade()}")
    nivel.atualizar(20)
    print(f"{nivel.tag}: {nivel.valor()} {nivel.unidade()}")


if __name__ == "__main__":
    main()
