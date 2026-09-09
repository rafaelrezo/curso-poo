# Integração resiliente: aplicar exceções ao contrato

## Objetivos de aprendizagem

- Aplicar as exceções da Parte 1 nas fronteiras de aquisição e integração.
- Distinguir evento operacional válido de documento inválido.
- Verificar recuperação por registro e preservar as regressões dos objetos.

**Tempo estimado:** parte do marco 2 (4h compartilhadas com JSON e integração).

## Vídeo da aula

![type:video](https://www.youtube.com/embed/He7Z1Mx8Hno)

---

## 1. De onde partimos e qual problema apareceu?

O produtor e o consumidor da atividade anterior trocam leituras. Uma linha truncada ainda encerra o consumidor. Vamos aplicar [exceções e recursos](../../fundamentos_poo_cpp_python/11_excecoes/index.md) para isolar registros inválidos, mantendo visíveis falhas que impedem processar o arquivo inteiro.

| Situação | Significado | Resposta |
|---|---|---|
| `SensorNivel.atualizar(-10)` | proposta rejeitada pelo objeto | retorna falso, preservando o valor anterior |
| aquisição indisponível | falha operacional | tratar `FalhaLeitura` e produzir evento com `status="falha"` e `valor=null` |
| JSON truncado ou campos inválidos | entrada fora do contrato | registrar linha e motivo, continuar o lote |
| arquivo inacessível | lote não pode ser aberto | propagar até a fronteira da aplicação, registrar e encerrar |
| defeito de programação | comportamento não previsto | deixar evidente; não ocultar em captura genérica |

## 2. Ponte C++ → Python: reutilizar a política de falhas

Em C++, reutilize `adquirir` e `FalhaLeitura` do starter. A função de aquisição recebe disponibilidade e calibração; não existe obrigação de acrescentar um método `adquirir` a `Sensor`. Capture por referência constante no ponto que decide a resposta operacional. RAII libera recursos ao sair do escopo.

Em Python, a mesma decisão usa `except FalhaLeitura`; `with` ou `finally` garantem liberação. Nenhuma dessas escolhas exige mudar o retorno booleano de `atualizar`. Um evento de falha tem tag, tipo, unidade, status e timestamp válidos, com valor nulo para não apresentar uma leitura antiga como atual.

Primeiro teste aquisição bem-sucedida, indisponibilidade e calibração inválida; depois conecte o resultado ao serializador. Estenda o produtor da atividade anterior para serializar o evento de falha, preservando as regras de escape e formato.

## 3. Aplicar agora: validar antes de apresentar

Salve em `supervisor_validacao.py` no fork. A validação aceita exatamente o formato temporal escolhido para o projeto e associa unidade ao tipo.

```python
import json
import math
from datetime import datetime


def validar(leitura):
    if not isinstance(leitura, dict):
        raise ValueError("registro deve ser objeto JSON")
    campos = {"tag", "tipo", "valor", "unidade", "status", "timestamp"}
    if not campos <= leitura.keys():
        raise ValueError("faltam campos obrigatórios")
    for campo in campos - {"valor"}:
        if not isinstance(leitura[campo], str) or not leitura[campo].strip():
            raise ValueError(f"{campo} deve ser texto não vazio")
    unidades = {"nivel": "%", "temperatura": "C"}
    if leitura["tipo"] not in unidades:
        raise ValueError("tipo desconhecido")
    if leitura["unidade"] != unidades[leitura["tipo"]]:
        raise ValueError("unidade incompatível com tipo")
    if leitura["status"] not in {"operando", "alerta", "falha"}:
        raise ValueError("status desconhecido")
    valor = leitura["valor"]
    if leitura["status"] == "falha":
        if valor is not None:
            raise ValueError("falha exige valor null")
    elif type(valor) not in (int, float) or not math.isfinite(valor):
        raise ValueError("leitura exige número finito; booleano não é medição")
    instante = leitura["timestamp"]
    data = datetime.strptime(instante, "%Y-%m-%dT%H:%M:%SZ")
    if data.strftime("%Y-%m-%dT%H:%M:%SZ") != instante:
        raise ValueError("timestamp deve usar AAAA-MM-DDTHH:MM:SSZ")
    return leitura


def processar_linhas(linhas):
    validas, invalidas = [], []
    for numero, linha in enumerate(linhas, start=1):
        try:
            validas.append(validar(json.loads(linha)))
        except (ValueError, TypeError, OverflowError) as erro:
            invalidas.append({"linha": numero, "erro": str(erro)})
    return validas, invalidas
```

`JSONDecodeError` deriva de `ValueError`. A verificação de objeto ocorre antes de consultar campos; listas e `null` não provocam acesso indevido a atributos. `bool` deriva de `int` em Python, por isso a checagem numérica o exclui explicitamente. Números grandes demais para a conversão de `math.isfinite` também são rejeitados.

O validador protege o contrato de transporte. Limites físicos pertencem aos objetos do domínio e aos seus testes; um alerta não é automaticamente uma leitura fora da faixa física.

## 4. Mini-caso prático e resultado observável

Monte um lote com uma leitura válida, uma linha truncada e um evento válido de falha:

```python
registro = {"tag": "LT-101", "tipo": "nivel", "valor": 42.5,
            "unidade": "%", "status": "operando",
            "timestamp": "2026-09-09T13:30:00Z"}
falha = dict(registro, valor=None, status="falha")
validas, invalidas = processar_linhas(
    [json.dumps(registro), '{"tag":', json.dumps(falha)]
)
print(len(validas), len(invalidas), invalidas[0]["linha"])
```

Saída esperada: `2 1 2`. O evento de falha entra nas leituras válidas; a linha truncada entra no relatório de entradas inválidas.

## 5. Extensão autônoma e confirmação

Continue em `projeto/02-integracao`. Escreva testes para lista JSON, `null`, campo ausente, booleano, NaN, infinito, data impossível, unidade incompatível, status desconhecido e falha acompanhada de valor numérico. Verifique também lote vazio e uma linha válida após uma inválida.

Acrescente esses testes ao alvo `make test-projeto`, junto ao teste que executa o produtor C++ e valida sua saída com Python. Execute o mesmo comando localmente; faça commit e push para `origin`. A CI já executa esse alvo para branches `projeto/**`. Abra PR para a main do próprio fork com resultados, link da execução remota e rastreabilidade de IA. Testes de contrato do transporte complementam os testes dos objetos; mantenha ambos.

Se uma falha operacional desaparecer, confira a distinção entre `status="falha"` e documento inválido. Se um defeito for silenciosamente ignorado, remova capturas amplas e identifique a fronteira responsável por decidir a reação.

## 6. O que isso prepara?

O caminho C++ → JSONL → Python funciona com falhas isoladas. O marco seguinte aplica padrões de projeto para variar regras e notificar interessados sem concentrar tudo no controlador. Atualize o diagrama quando introduzir responsabilidades; reutilize este contrato ao acrescentar histórico e comunicação TCP.

## Perguntas de revisão rápida

1. Por que `status="falha"` pode representar uma telemetria válida?
2. Por que a captura de erro de parsing fica dentro do laço?
3. Qual teste impede que uma leitura antiga seja apresentada como nova após falha?

## Fontes de referência

- [Python — exceções](https://docs.python.org/3/tutorial/errors.html)
- [Python — JSON](https://docs.python.org/3/library/json.html)
- [Python — datetime](https://docs.python.org/3/library/datetime.html)
- [C++ Core Guidelines — erros e recursos](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-errors)
