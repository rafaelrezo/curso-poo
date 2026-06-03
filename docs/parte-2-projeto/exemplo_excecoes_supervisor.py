import json


STATUS_VALIDOS = {"operando", "alerta", "falha", "manutencao"}
CAMPOS_OBRIGATORIOS = {"tag", "tipo", "valor", "unidade", "status"}


def validar_leitura(leitura: dict) -> None:
    faltantes = CAMPOS_OBRIGATORIOS - set(leitura)

    if faltantes:
        raise ValueError(f"campos ausentes: {sorted(faltantes)}")

    if not isinstance(leitura["tag"], str) or not leitura["tag"]:
        raise TypeError("tag deve ser texto nao vazio")

    if not isinstance(leitura["tipo"], str) or not leitura["tipo"]:
        raise TypeError("tipo deve ser texto nao vazio")

    if not isinstance(leitura["unidade"], str) or not leitura["unidade"]:
        raise TypeError("unidade deve ser texto nao vazio")

    if leitura["status"] not in STATUS_VALIDOS:
        raise ValueError(f"status invalido: {leitura['status']}")

    if leitura["status"] != "falha" and not isinstance(leitura["valor"], int | float):
        raise TypeError("valor deve ser numerico quando a leitura nao esta em falha")


class LeituraSupervisor:
    def __init__(self, dados: dict) -> None:
        validar_leitura(dados)
        self.tag = dados["tag"]
        self.tipo = dados["tipo"]
        self.valor = dados["valor"]
        self.unidade = dados["unidade"]
        self.status = dados["status"]

    def como_linha(self) -> dict:
        return {
            "tag": self.tag,
            "tipo": self.tipo,
            "valor": self.valor,
            "unidade": self.unidade,
            "status": self.status,
        }


def importar_linhas_json(linhas: list[str]) -> tuple[list[LeituraSupervisor], list[dict]]:
    leituras_validas = []
    leituras_invalidas = []

    for numero, linha in enumerate(linhas, start=1):
        try:
            dados = json.loads(linha)
            leitura = LeituraSupervisor(dados)
            leituras_validas.append(leitura)
        except json.JSONDecodeError as erro:
            leituras_invalidas.append({
                "linha": numero,
                "erro": f"JSON invalido: {erro.msg}",
            })
        except (TypeError, ValueError) as erro:
            leituras_invalidas.append({
                "linha": numero,
                "erro": str(erro),
            })

    return leituras_validas, leituras_invalidas


if __name__ == "__main__":
    linhas_recebidas = [
        '{"tag":"LT-101","tipo":"nivel","valor":42.0,"unidade":"%","status":"operando"}',
        '{"tag":"PT-201","tipo":"pressao","valor":2.7,"unidade":"bar","status":"operando"}',
        '{"tag":"LT-999","tipo":"nivel","valor":null,"unidade":"%","status":"falha","erro":"leitura fora da faixa"}',
        '{"tag":"FT-301","tipo":"vazao","valor":"erro","unidade":"m3/h","status":"operando"}',
        '{"tag":"LT-102","tipo":"nivel","valor":51.0,"unidade":"%","status":"operando"',
    ]

    validas, invalidas = importar_linhas_json(linhas_recebidas)

    print("Leituras validas:")
    for leitura in validas:
        print(leitura.como_linha())

    print("\nLeituras invalidas:")
    for item in invalidas:
        print(item)
