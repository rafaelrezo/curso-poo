from dataclasses import dataclass


@dataclass(frozen=True, eq=False)
class IdSensor:
    valor: str

    def __post_init__(self):
        if not self.valor:
            raise ValueError("tag vazia")

    def __eq__(self, outro):
        if not isinstance(outro, IdSensor):
            return NotImplemented
        return self.valor == outro.valor

    def __hash__(self):
        return hash(self.valor)


def main():
    a = IdSensor("LT-101")
    b = IdSensor("LT-101")
    alias = a
    print(f"Alias: {alias is a}")
    print(f"Mesma instancia: {a is b}")
    print(f"Mesmo valor: {a == b}")
    print(f"Mesmo hash: {hash(a) == hash(b)}")


if __name__ == "__main__":
    main()
