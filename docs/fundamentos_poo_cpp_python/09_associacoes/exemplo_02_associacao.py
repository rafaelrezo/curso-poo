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


class PainelFixo:
    def __init__(self, sensor):
        self._sensor = sensor

    def leitura(self):
        return self._sensor.valor()


def consultar_agora(sensor):
    return sensor.valor()


def observar_paineis(sensor):
    p = PainelFixo(sensor)
    q = PainelFixo(sensor)
    print(f"Antes: {p.leitura()} {q.leitura()}")
    sensor.atualizar(20)
    print(f"Depois: {p.leitura()} {q.leitura()}")


def main():
    sensor = SensorNivel(10)
    observar_paineis(sensor)
    print(f"Sem paineis: {consultar_agora(sensor)}")


if __name__ == "__main__":
    main()
