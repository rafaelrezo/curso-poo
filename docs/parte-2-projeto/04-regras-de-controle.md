# Aula Integrada - Atividade Prática com Lista de Objetos

## Objetivos de aprendizagem

- Praticar uma lista dinâmica de objetos em C++ e em Python.
- Usar encapsulamento e validação apenas como apoio para manter os objetos consistentes.
- Montar uma saída em formato de dados que possa ser exibida por um supervisor.

**Tempo estimado:** parte da mesma aula integrada.

## Vídeo da aula

![type:video](https://www.youtube.com/embed/ZFDSPSU6JMw)

---

## 1. Síntese da aula

Até aqui, a aula trabalhou uma sequência única:

1. criar uma classe-base;
2. criar classes derivadas;
3. guardar objetos diferentes em uma lista dinâmica;
4. percorrer a lista em C++;
5. repetir a ideia em Python;
6. validar dados básicos;
7. transformar objetos em dados para exibição.

Agora a prática usa o contexto do projeto: uma estação com múltiplos sensores.

---

## 2. Exemplo de apoio: sala técnica

O exemplo usado nas explicações foi uma sala técnica:

| Classe | Tipo | Unidade | Faixa sugerida |
|---|---|---:|---:|
| `SensorTemperatura` | `temperatura` | `C` | `-10` a `80` |
| `SensorUmidade` | `umidade` | `%` | `0` a `100` |
| `SensorCorrente` | `corrente` | `A` | `0` a `50` |

Esse exemplo serve como guia de estrutura:

- classe-base com interface comum;
- classes derivadas com comportamento próprio;
- lista dinâmica;
- laço para percorrer todos os objetos;
- conversão para dados que o supervisor consegue exibir.

---

## 3. Atividade única da aula

Crie uma versão simplificada para uma **estação de bombeamento**.

### Requisitos em C++

Implemente uma hierarquia de sensores:

| Classe | Tipo | Unidade | Faixa mínima | Faixa máxima |
|---|---|---:|---:|---:|
| `SensorNivel` | `nivel` | `%` | `0` | `100` |
| `SensorPressao` | `pressao` | `bar` | `0` | `10` |
| `SensorVazao` | `vazao` | `m3/h` | `0` | `100` |

O programa deve:

1. declarar uma classe-base `Sensor`;
2. criar as três classes derivadas;
3. guardar os sensores em `vector<unique_ptr<Sensor>>`;
4. adicionar sensores usando `push_back(make_unique<...>())`;
5. percorrer a lista usando `for (const auto& sensor : sensores)`;
6. chamar `simularLeitura()` em cada sensor;
7. imprimir `tag`, `tipo`, `valor`, `unidade` e `status`.

Use uma saída simples com separador `|` para facilitar leitura, comparação e testes:

```text
LT-101|nivel|42|%|operando
PT-201|pressao|2.7|bar|operando
FT-301|vazao|18|m3/h|operando
```

### Requisitos em Python

Crie uma estrutura equivalente para representar leituras recebidas pelo supervisor.

O programa deve:

1. receber uma lista de dicionários com leituras;
2. validar campos obrigatórios;
3. criar objetos `LeituraSupervisor`;
4. guardar esses objetos em uma lista;
5. separar leituras válidas e leituras inválidas;
6. imprimir uma tabela simples no terminal ou preparar um `DataFrame`.

No Python, organize o supervisor com esta API mínima:

```python
def validar_leitura(leitura: dict) -> None:
    ...


class LeituraSupervisor:
    def como_linha(self) -> dict:
        ...


def criar_leituras_supervisor(dados: list[dict]) -> tuple[list[LeituraSupervisor], list[dict]]:
    ...


def avaliar_operacao(leituras: list[LeituraSupervisor]) -> str:
    ...
```

---

## 4. Formato de dados esperado

O C++ pode imprimir linhas ou gerar um JSON. Para a atividade, basta produzir dados com esta estrutura conceitual:

```json
[
  {
    "tag": "LT-101",
    "tipo": "nivel",
    "valor": 42.0,
    "unidade": "%",
    "status": "operando"
  },
  {
    "tag": "PT-201",
    "tipo": "pressao",
    "valor": 2.7,
    "unidade": "bar",
    "status": "operando"
  },
  {
    "tag": "FT-301",
    "tipo": "vazao",
    "valor": 18.0,
    "unidade": "m3/h",
    "status": "operando"
  }
]
```

O importante é que a lista tenha itens uniformes, mesmo que os sensores tenham classes diferentes no C++.

---

## 5. Pequena regra para usar os dados

Depois de montar a lista, aplique uma regra simples apenas para praticar o uso dos objetos:

| Condição | Mensagem esperada |
|---|---|
| nível menor que `30%` | `nivel_baixo` |
| nível maior que `80%` | `nivel_alto` |
| pressão maior que `6 bar` | `pressao_alta` |
| nenhuma condição crítica | `operacao_normal` |

Essa regra pode aparecer como uma função separada. Ela não deve ficar dentro do código de tela.

Use a seguinte prioridade quando mais de uma condição aparecer ao mesmo tempo:

1. `nivel_baixo`;
2. `nivel_alto`;
3. `pressao_alta`;
4. `operacao_normal`.

Exemplo de saída conceitual:

```json
{
  "leituras": [
    {"tag": "LT-101", "tipo": "nivel", "valor": 42.0, "unidade": "%", "status": "operando"},
    {"tag": "PT-201", "tipo": "pressao", "valor": 2.7, "unidade": "bar", "status": "operando"}
  ],
  "mensagem": "operacao_normal"
}
```

---

## 6. Critérios de implementação

### C++

- A classe-base possui pelo menos um método virtual puro.
- As classes derivadas usam `override`.
- A lista usa `vector<unique_ptr<Sensor>>`.
- O código inclui `<vector>` e `<memory>`.
- O laço usa `const auto&`.
- Não há escrita direta em atributos internos fora da classe.

### Python

- A lista de entrada pode ter quantidade variável de leituras.
- Cada leitura é validada antes de ser exibida.
- Leituras válidas e inválidas são separadas.
- A classe `LeituraSupervisor` organiza os dados recebidos.
- O supervisor não altera a lógica interna do dispositivo C++.

---

## 7. Roteiro sugerido de desenvolvimento

1. Comece pelo C++ com apenas `SensorNivel`.
2. Troque a variável única por `vector<unique_ptr<Sensor>>`.
3. Adicione `SensorPressao` e `SensorVazao`.
4. Percorra todos os sensores com `const auto&`.
5. Acrescente validação de faixa.
6. Gere uma lista de leituras.
7. No Python, leia uma lista de dicionários equivalente.
8. Valide e transforme os dicionários em objetos.
9. Mostre os dados em tabela.
10. Aplique a regra simples de mensagem operacional.

---

## 8. Perguntas de revisão rápida

1. Onde aparece a lista dinâmica no C++?
2. Onde aparece a lista dinâmica no Python?
3. Por que `unique_ptr` aparece no C++ e não aparece no Python?
4. Como o laço sobre a lista usa polimorfismo?
5. Por que a regra simples deve ser separada da tela?

---

## 9. Entrega da atividade

A entrega deve conter:

- código C++ com a hierarquia de sensores;
- código Python com a lista de leituras do supervisor;
- exemplo de saída com pelo menos três sensores;
- uma leitura inválida tratada sem encerrar o programa;
- resposta curta no README explicando `vector`, `unique_ptr`, `push_back`, `const auto&`, `list` e `append`.

## Fontes de referência

- [cppreference - std::vector](https://en.cppreference.com/w/cpp/container/vector)
- [cppreference - std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr)
- [cppreference - virtual functions](https://en.cppreference.com/w/cpp/language/virtual)
- [Python Docs - Data Structures](https://docs.python.org/3/tutorial/datastructures.html)
- [Python Docs - Classes](https://docs.python.org/3/tutorial/classes.html)
