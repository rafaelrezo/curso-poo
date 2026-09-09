from abc import ABC, abstractmethod


class IFonteLeitura(ABC):
    @abstractmethod
    def valor(self):
        raise NotImplementedError


class FonteTeste(IFonteLeitura):
    def valor(self):
        return 10


class PoliticaAlarme:
    def __init__(self, limite):
        self._limite = limite

    def aciona(self, valor):
        return valor > self._limite


class ControladorConsulta:
    def __init__(self, fonte, politica):
        self._fonte = fonte
        self._politica = politica

    def avaliar(self):
        return self._politica.aciona(self._fonte.valor())


def main():
    fonte = FonteTeste()
    politica = PoliticaAlarme(10)
    controlador = ControladorConsulta(fonte, politica)
    alerta = controlador.avaliar()
    assert not alerta
    print("OK: igualdade ao limite nao alerta")


if __name__ == "__main__":
    main()
