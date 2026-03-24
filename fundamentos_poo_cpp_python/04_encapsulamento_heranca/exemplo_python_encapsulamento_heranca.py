class AtivoIndustrial:
    def __init__(self, tag, area):
        self._tag = tag
        self._area = area
        self._bloqueado = False

    @property
    def tag(self):
        return self._tag

    @property
    def area(self):
        return self._area

    def esta_bloqueado(self):
        return self._bloqueado

    def bloquear(self):
        self._bloqueado = True

    def liberar(self):
        self._bloqueado = False

    def _redefinir_area(self, nova_area):
        self._area = nova_area


class SensorPressao(AtivoIndustrial):
    def __init__(self, tag, area, limite_alarme):
        super().__init__(tag, area)
        self._valor_atual = 0.0
        self._limite_alarme = limite_alarme
        self._offset_calibracao = 0.0

    def atualizar_valor(self, novo_valor):
        if self.esta_bloqueado():
            return

        self._valor_atual = max(0.0, novo_valor)

    def aplicar_offset_calibracao(self, novo_offset):
        self._offset_calibracao = novo_offset

    def mover_para_area(self, nova_area):
        self._redefinir_area(nova_area)

    @property
    def valor_atual(self):
        return self._valor_atual

    def em_alarme(self):
        return self._valor_atual > self._limite_alarme

    def exibir_resumo(self):
        print(
            f"[SensorPressao] {self.tag} | area: {self.area} "
            f"| bloqueado: {'sim' if self.esta_bloqueado() else 'nao'} "
            f"| valor: {self._valor_atual} bar "
            f"| limite: {self._limite_alarme} bar "
            f"| alarme: {'sim' if self.em_alarme() else 'nao'}"
        )


class BombaRecalque(AtivoIndustrial):
    def __init__(self, tag, area):
        super().__init__(tag, area)
        self._frequencia_hz = 0.0

    def ajustar_frequencia(self, nova_frequencia):
        if self.esta_bloqueado():
            return

        self._frequencia_hz = min(60.0, max(0.0, nova_frequencia))

    def exibir_resumo(self):
        print(
            f"[BombaRecalque] {self.tag} | area: {self.area} "
            f"| bloqueado: {'sim' if self.esta_bloqueado() else 'nao'} "
            f"| frequencia: {self._frequencia_hz} Hz"
        )


def emitir_relatorio_calibracao(sensor):
    print(
        f"[Relatorio de calibracao] {sensor.tag} "
        f"| valor exposto: {sensor.valor_atual} "
        f"| use uma API publica clara em vez de um equivalente a friend"
    )


def main():
    sensor_recalque = SensorPressao("PT-301", "Campo", 6.0)
    bomba_principal = BombaRecalque("P-301", "Casa de bombas")

    sensor_recalque.atualizar_valor(6.8)
    sensor_recalque.aplicar_offset_calibracao(-0.15)
    sensor_recalque.mover_para_area("Utilidades")

    bomba_principal.ajustar_frequencia(48.0)
    bomba_principal.bloquear()
    bomba_principal.ajustar_frequencia(55.0)

    sensor_recalque.exibir_resumo()
    bomba_principal.exibir_resumo()
    emitir_relatorio_calibracao(sensor_recalque)


if __name__ == "__main__":
    main()
