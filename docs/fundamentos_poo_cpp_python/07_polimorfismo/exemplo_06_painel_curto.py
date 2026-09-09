from abc import ABC, abstractmethod


class Sensor(ABC):
    @abstractmethod
    def valor(self):
        raise NotImplementedError

    @abstractmethod
    def unidade(self):
        raise NotImplementedError

    @abstractmethod
    def em_alerta(self):
        raise NotImplementedError


class SensorNivel(Sensor):
    def valor(self):
        return 15

    def unidade(self):
        return "%"

    def em_alerta(self):
        return self.valor() < 20


class SensorTemperatura(Sensor):
    def valor(self):
        return 15

    def unidade(self):
        return "C"

    def em_alerta(self):
        return self.valor() > 45


def mostrar(sensor: Sensor):
    estado = "ALERTA" if sensor.em_alerta() else "OK"
    print(f"{sensor.valor()} {sensor.unidade()} | {estado}")


def main():
    nivel = SensorNivel()
    temperatura = SensorTemperatura()
    mostrar(nivel)
    mostrar(temperatura)


if __name__ == "__main__":
    main()
