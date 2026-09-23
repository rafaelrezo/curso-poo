class IdSensor:
    def __init__(self, valor):
        if not valor:
            raise ValueError("tag vazia")
        self._valor = valor

    # == consulta esta regra de igualdade, baseada no valor da tag.
    def __eq__(self, outro):
        if not isinstance(outro, IdSensor):
            # Deixa o Python tratar a comparação com um tipo não contemplado.
            return NotImplemented
        return self._valor == outro._valor


def main():
    a = IdSensor("LT-101")
    b = IdSensor("LT-101")  # Nova instância, com a mesma tag.
    alias = a  # Outro nome para o mesmo objeto; não há cópia.
    # is verifica identidade; == consulta a igualdade definida pela classe.
    print(f"Alias: {alias is a}")
    print(f"Mesma instancia: {a is b}")
    print(f"Mesmo identificador: {a == b}")


if __name__ == "__main__":
    main()
