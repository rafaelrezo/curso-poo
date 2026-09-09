from abc import ABC, abstractmethod
from math import isfinite


class SensorNivel:
    def __init__(self, valor):
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


class IFonteLeitura(ABC):
    @abstractmethod
    def valor(self):
        raise NotImplementedError

    @abstractmethod
    def unidade(self):
        raise NotImplementedError


class FonteNivel(IFonteLeitura):
    def __init__(self, sensor):
        self._sensor = sensor

    def valor(self):
        return self._sensor.valor()

    def unidade(self):
        return self._sensor.unidade()


def ler_fonte(fonte: IFonteLeitura):
    return fonte.valor()


def main():
    sensor = SensorNivel(10)
    fonte = FonteNivel(sensor)
    print(f"{ler_fonte(fonte)} {fonte.unidade()}")
    sensor.atualizar(20)
    print(f"{ler_fonte(fonte)} {fonte.unidade()}")
    # IFonteLeitura()  # experimento: tente instanciar o contrato


if __name__ == "__main__":
    main()
