from math import isfinite


class SensorNivel:
    def __init__(self, tag: str, valor: float = 50.0):
        if not tag or not isfinite(valor) or not 0 <= valor <= 100:
            raise ValueError("tag vazia ou nivel fora de 0..100")
        self._tag = tag
        self._valor = valor

    @property
    def tag(self):
        return self._tag

    @property
    def valor(self):
        return self._valor


def main():
    usual = SensorNivel("LT-101")
    informado = SensorNivel("LT-102", 42.5)
    zero = SensorNivel("LT-104", 0.0)
    print(f"{usual.tag}: {usual.valor:g} %")
    print(f"{informado.tag}: {informado.valor:g} %")
    print(f"{zero.tag}: {zero.valor:g} %")


if __name__ == "__main__":
    main()
