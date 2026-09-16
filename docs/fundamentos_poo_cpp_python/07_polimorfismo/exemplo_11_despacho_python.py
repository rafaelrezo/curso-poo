from abc import ABC, abstractmethod  # Ferramentas para declarar uma classe abstrata.


class Sensor(ABC):  # Base comum; nao criaremos Sensor diretamente.
    @abstractmethod  # Obriga cada classe concreta a implementar unidade().
    def unidade(self):
        raise NotImplementedError  # Corpo de reserva; as derivadas abaixo o substituem.


class SensorNivel(Sensor):
    def unidade(self):
        return "%"


class SensorTemperatura(Sensor):
    def unidade(self):
        return "C"


def imprimir(sensor: Sensor):  # O cliente conhece o contrato, nao a classe concreta.
    print(sensor.unidade())  # A implementacao vem do objeto recebido.


def main():
    nivel = SensorNivel()
    temperatura = SensorTemperatura()
    imprimir(nivel)  # Chama SensorNivel.unidade().
    imprimir(temperatura)  # Chama SensorTemperatura.unidade().


if __name__ == "__main__":
    main()
