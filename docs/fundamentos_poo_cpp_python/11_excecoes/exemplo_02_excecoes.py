class FonteConstante:
    def valor(self):
        return 42.5


class FalhaLeitura(Exception):  # Tipo proprio para uma falha prevista de aquisicao.
    pass  # A classe herda o comportamento de Exception; nao precisa de metodo novo.


class Sessao:
    def __init__(self):
        self.abertas = 0

    def abrir(self):
        self.abertas += 1

    def fechar(self):
        self.abertas -= 1


def adquirir(fonte, disponivel, sessao):
    sessao.abrir()  # A partir daqui ha uma sessao ativa.
    print(f"Durante aquisicao | sessoes: {sessao.abertas}")
    try:
        if not disponivel:
            raise FalhaLeitura("fonte indisponivel")  # Interrompe a aquisicao.
        return fonte.valor()
    finally:
        sessao.fechar()  # Executa no retorno normal e antes de propagar a falha.


def ler_servico(fonte, disponivel, sessao):
    return adquirir(fonte, disponivel, sessao)  # Nao captura: a falha sobe ao cliente.


def executar_ciclo(fonte, disponivel, sessao):
    try:
        return True, ler_servico(fonte, disponivel, sessao)
    except FalhaLeitura:  # So a falha prevista vira "sem leitura".
        return False, 0  # O False marca ausencia; zero nao e uma medicao.


def mostrar(resultado, sessao):
    sucesso, valor = resultado
    mensagem = f"Leitura: {valor}" if sucesso else "Sem leitura"
    print(f"{mensagem} | sessoes: {sessao.abertas}")


def main():
    fonte = FonteConstante()
    sessao = Sessao()
    primeiro = executar_ciclo(fonte, True, sessao)
    mostrar(primeiro, sessao)
    segundo = executar_ciclo(fonte, False, sessao)
    mostrar(segundo, sessao)
    terceiro = executar_ciclo(fonte, True, sessao)
    mostrar(terceiro, sessao)


if __name__ == "__main__":
    main()
