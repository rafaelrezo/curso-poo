from abc import ABC, abstractmethod


class AtuadorMalha(ABC):
    def __init__(self, tag, area):
        self._tag = tag
        self._area = area

    @property
    def tag(self):
        return self._tag

    @property
    def area(self):
        return self._area

    def _limitar_percentual(self, percentual):
        return min(100.0, max(0.0, percentual))

    @abstractmethod
    def aplicar_referencia_percentual(self, percentual):
        pass

    @abstractmethod
    def exibir_resumo(self):
        pass


class InversorBomba(AtuadorMalha):
    def __init__(self, tag, area):
        super().__init__(tag, area)
        self._frequencia_hz = 0.0

    def aplicar_referencia_percentual(self, percentual):
        percentual = self._limitar_percentual(percentual)
        self._frequencia_hz = (percentual / 100.0) * 60.0

    def exibir_resumo(self):
        print(
            f"[InversorBomba] {self.tag} | area: {self.area} "
            f"| frequencia: {self._frequencia_hz} Hz"
        )


class ValvulaProporcional(AtuadorMalha):
    def __init__(self, tag, area):
        super().__init__(tag, area)
        self._abertura_percentual = 0.0

    def aplicar_referencia_percentual(self, percentual):
        self._abertura_percentual = self._limitar_percentual(percentual)

    def exibir_resumo(self):
        print(
            f"[ValvulaProporcional] {self.tag} | area: {self.area} "
            f"| abertura: {self._abertura_percentual}%"
        )


class DosadorQuimico(AtuadorMalha):
    def __init__(self, tag, area):
        super().__init__(tag, area)
        self._vazao_lh = 0.0

    def aplicar_referencia_percentual(self, percentual):
        percentual = self._limitar_percentual(percentual)
        self._vazao_lh = (percentual / 100.0) * 120.0

    def exibir_resumo(self):
        print(
            f"[DosadorQuimico] {self.tag} | area: {self.area} "
            f"| vazao: {self._vazao_lh} L/h"
        )


def main():
    atuadores = [
        InversorBomba("P-201", "Recalque"),
        ValvulaProporcional("FV-310", "Mistura rapida"),
        DosadorQuimico("DP-101", "Cloracao"),
    ]

    referencia_supervisoria = 62.5

    print(
        "Aplicando referencia unica do supervisorio: "
        f"{referencia_supervisoria}%"
    )

    for atuador in atuadores:
        atuador.aplicar_referencia_percentual(referencia_supervisoria)

    print("Resposta individual de cada equipamento:")
    for atuador in atuadores:
        atuador.exibir_resumo()


if __name__ == "__main__":
    main()
