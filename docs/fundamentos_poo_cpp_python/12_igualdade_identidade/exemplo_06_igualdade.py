class IdSensor:
    def __init__(self, valor):
        if not valor:
            raise ValueError("tag vazia")
        self._valor = valor

    def __eq__(self, outro):
        if not isinstance(outro, IdSensor):
            return NotImplemented
        return self._valor == outro._valor


def main():
    a = IdSensor("LT-101")
    b = IdSensor("LT-101")
    alias = a
    print(f"Alias: {alias is a}")
    print(f"Mesma instancia: {a is b}")
    print(f"Mesmo identificador: {a == b}")


if __name__ == "__main__":
    main()
