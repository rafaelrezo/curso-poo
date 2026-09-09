class FonteConstante:
    def valor(self):
        return 42.5


class FalhaLeitura(Exception):
    pass


class Sessao:
    def __init__(self):
        self.abertas = 0

    def abrir(self):
        self.abertas += 1

    def fechar(self):
        self.abertas -= 1


def adquirir(fonte, disponivel, sessao):
    sessao.abrir()
    try:
        if not disponivel:
            raise FalhaLeitura("fonte indisponivel")
        return fonte.valor()
    finally:
        sessao.fechar()


def ler_servico(fonte, disponivel, sessao):
    return adquirir(fonte, disponivel, sessao)


def executar_ciclo(fonte, disponivel, sessao):
    try:
        return True, ler_servico(fonte, disponivel, sessao)
    except FalhaLeitura:
        return False, 0


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
