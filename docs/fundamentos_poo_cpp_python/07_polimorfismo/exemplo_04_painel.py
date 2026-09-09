from abc import ABC, abstractmethod
from math import isfinite


class Sensor(ABC):
    def __init__(self, tag: str):
        self._tag = tag

    @property
    def tag(self):
        return self._tag

    @abstractmethod
    def valor(self):
        raise NotImplementedError

    @abstractmethod
    def unidade(self):
        raise NotImplementedError

    @abstractmethod
    def atualizar(self, leitura):
        raise NotImplementedError

    @abstractmethod
    def em_alerta(self):
        raise NotImplementedError


class SensorNivel(Sensor):
    def __init__(self, tag):
        super().__init__(tag)
        self._valor = 50.0

    def valor(self):
        return self._valor

    def unidade(self):
        return "%"

    def atualizar(self, leitura):
        if not isfinite(leitura) or leitura < 0 or leitura > 100:
            return False
        self._valor = leitura
        return True

    def em_alerta(self):
        return self.valor() < 20.0


class SensorTemperatura(Sensor):
    def __init__(self, tag):
        super().__init__(tag)
        self._valor = 25.0

    def valor(self):
        return self._valor

    def unidade(self):
        return "C"

    def atualizar(self, leitura):
        if not isfinite(leitura) or leitura < -40 or leitura > 125:
            return False
        self._valor = leitura
        return True

    def em_alerta(self):
        return self.valor() > 45.0


def linha_painel(sensor: Sensor) -> str:
    estado = "ALERTA" if sensor.em_alerta() else "OK"
    return f"{sensor.tag}: {sensor.valor():.1f} {sensor.unidade()} | {estado}"


def main():
    nivel = SensorNivel("LT-101")
    temperatura = SensorTemperatura("TT-201")
    nivel.atualizar(15)
    temperatura.atualizar(15)
    print(linha_painel(nivel))
    print(linha_painel(temperatura))


if __name__ == "__main__":
    main()
