class SensorAnalogico:
    def __init__(self, tag, unidade="desconhecida", minimo=0.0, maximo=100.0, valor=0.0):
        self.tag = tag
        self.unidade = unidade
        self.minimo = minimo
        self.maximo = maximo
        self.valor = valor

    @classmethod
    def descoberto_na_rede(cls, tag):
        return cls(tag)

    @classmethod
    def com_leitura_atual(cls, tag, unidade, minimo, maximo, valor):
        return cls(tag, unidade, minimo, maximo, valor)

    @classmethod
    def parametrizado(cls, tag, unidade, minimo, maximo):
        return cls(tag, unidade, minimo, maximo, minimo)

    def exibir_resumo(self):
        print(
            f"[Sensor] {self.tag} | faixa: {self.minimo} a {self.maximo} "
            f"{self.unidade} | valor atual: {self.valor} {self.unidade}"
        )


def main():
    sensor_descoberto = SensorAnalogico.descoberto_na_rede("LT-101")
    sensor_parametrizado = SensorAnalogico.parametrizado("LT-101", "%", 0.0, 100.0)
    sensor_com_leitura = SensorAnalogico.com_leitura_atual("TT-201", "C", 0.0, 150.0, 72.5)

    sensor_descoberto.exibir_resumo()
    sensor_parametrizado.exibir_resumo()
    sensor_com_leitura.exibir_resumo()


if __name__ == "__main__":
    main()
