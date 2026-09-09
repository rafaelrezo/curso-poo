from abc import ABC, abstractmethod


class Sensor(ABC):
    @abstractmethod
    def unidade(self):
        raise NotImplementedError


class SensorNivel(Sensor):
    def unidade(self):
        return "%"


class SensorTemperatura(Sensor):
    def unidade(self):
        return "C"


def imprimir(sensor: Sensor):
    print(sensor.unidade())


def main():
    nivel = SensorNivel()
    temperatura = SensorTemperatura()
    imprimir(nivel)
    imprimir(temperatura)


if __name__ == "__main__":
    main()
