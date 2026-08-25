def exibir_mensagem(mensagem: str) -> None:
    print(mensagem)


def main() -> None:
    linguagem = "Python"
    mensagem = f"Hello, World! Executando {linguagem}."
    exibir_mensagem(mensagem)


if __name__ == "__main__":
    main()
