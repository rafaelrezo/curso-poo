from abc import ABC, abstractmethod


class SensorNivel:
    def __init__(self, valor):
        self._valor = valor

    def valor(self):
        return self._valor

    def atualizar(self, valor):
        self._valor = valor


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
        return "%"


class FonteConstante(IFonteLeitura):
    def valor(self):
        return 42.5

    def unidade(self):
        return "%"


def mostrar(fonte):
    print(f"{fonte.valor()} {fonte.unidade()}")


def main():
    sensor = SensorNivel(10)
    real = FonteNivel(sensor)
    simulada = FonteConstante()
    mostrar(real)
    mostrar(simulada)
    sensor.atualizar(20)
    mostrar(real)


if __name__ == "__main__":
    main()
