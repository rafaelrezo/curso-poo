# Contrato de telemetria: arquivos, JSONL e primeira integração

## Objetivos de aprendizagem

- Separar serialização de dados, transporte e apresentação.
- Produzir em C++ e consumir em Python um contrato JSONL mínimo e reproduzível.
- Acrescentar timestamp UTC somente quando surge a necessidade de ordenar eventos.

**Tempo estimado:** parte do marco 2 (4h compartilhadas com a integração resiliente).

## Vídeo da aula

![type:video](https://www.youtube.com/embed/iiADhChRriM)

---

## 1. Qual problema apareceu?

Partimos da arquitetura aprovada no marco 1 e das coleções da seção 11. No fork concluído, crie `projeto/02-integracao`; execute `make test-projeto` antes de alterar o sistema.

O controlador possui sensores em memória. Quando o processo termina, as leituras desaparecem e o Python não consegue acessá-las. Precisamos de uma fronteira explícita entre os programas.

```text
Controlador C++ -> dados/leituras.jsonl -> Supervisor Python
```

JSON define a representação. O arquivo é o transporte didático desta etapa. Na aula de comunicação TCP desta parte, o mesmo contrato poderá passar por TCP sem que JSON “vire uma rede”.

---

## 2. Comece pelo contrato

Cada linha será um objeto JSON completo:

```json
{"tag":"LT-101","tipo":"nivel","valor":42.5,"unidade":"%","status":"operando","timestamp":"2026-08-25T13:30:00Z"}
```

| Campo | Tipo | Regra |
|---|---|---|
| `tag` | string | identificador estável e não vazio |
| `tipo` | string | categoria conhecida pelo domínio |
| `valor` | number ou null | número finito para `operando`/`alerta`; `null` obrigatório para `falha` |
| `unidade` | string | acompanha o significado físico |
| `status` | string | `operando`, `alerta` ou `falha` |
| `timestamp` | string | instante UTC no formato `AAAA-MM-DDTHH:MM:SSZ` |

Um JSON bem formado ainda pode violar o contrato: `{"valor":"alto"}` é JSON válido, mas não é uma telemetria válida.

---

## 3. Produtor mínimo em C++

```cpp
#include <fstream>
#include <cmath>
#include <locale>
#include <iomanip>
#include <limits>
#include <regex>
#include <stdexcept>
#include <string>

void gravarLeitura(
    const std::string& caminho,
    const std::string& tag,
    double valor,
    const std::string& timestamp
) {
    // Primeiro incremento: domínio restrito e explícito, sem texto livre.
    if (!std::regex_match(tag, std::regex("[A-Za-z0-9_-]+")) ||
        !std::isfinite(valor) || valor < 0 || valor > 100 ||
        !std::regex_match(timestamp,
            std::regex("[0-9]{4}-[0-9]{2}-[0-9]{2}T[0-9]{2}:[0-9]{2}:[0-9]{2}Z"))) {
        throw std::invalid_argument("registro fora do contrato do produtor inicial");
    }
    // O chamador fornece um instante real; a validação calendárica ocorre no consumidor.
    std::ofstream arquivo(caminho, std::ios::app);
    arquivo.imbue(std::locale::classic());
    arquivo << std::setprecision(std::numeric_limits<double>::max_digits10);
    if (!arquivo) {
        throw std::runtime_error("não foi possível abrir " + caminho);
    }

    arquivo.exceptions(std::ios::badbit | std::ios::failbit);
    arquivo << "{\"tag\":\"" << tag
            << "\",\"tipo\":\"nivel\",\"valor\":" << valor
            << ",\"unidade\":\"%\",\"status\":\"operando\""
            << ",\"timestamp\":\"" << timestamp << "\"}\n";
    arquivo.close();
}
```

O produtor inicial só aceita tags alfanuméricas com hífen ou sublinhado, valores finitos de nível e timestamps no formato fixado. Essa restrição impede a entrada de aspas e controles nos campos interpolados. Ao aceitar texto livre ou novos tipos de registro, substitua a montagem manual por uma biblioteca JSON e teste escapes. O consumidor da próxima atividade valida também o calendário.

---

## 4. Consumidor CLI em Python

Comece no terminal; a interface gráfica só entra depois que o pipeline de dados estiver verde.

```python
import json
from pathlib import Path


def carregar_leituras(caminho: Path) -> list[dict]:
    leituras = []
    for linha in caminho.read_text(encoding="utf-8").splitlines():
        if linha.strip():
            leituras.append(json.loads(linha))
    return leituras


for leitura in carregar_leituras(Path("dados/leituras.jsonl")):
    print(
        f"{leitura['timestamp']} | {leitura['tag']} | "
        f"{leitura['valor']} {leitura['unidade']} | {leitura['status']}"
    )
```

### Saída esperada

```text
2026-08-25T13:30:00Z | LT-101 | 42.5 % | operando
```

---

## 5. Por que o timestamp entra agora?

Com várias leituras, o supervisor precisa saber quando cada evento ocorreu e em que ordem apresentá-los.

- use `system_clock` em C++ ou `datetime.now(timezone.utc)` em Python para instantes civis;
- use `steady_clock` ou `perf_counter` para medir duração;
- serialize instantes em UTC, terminando em `Z`;
- não use o relógio do sistema para medir tempo decorrido.

O detalhamento sintático de `chrono` e `datetime` fica como consulta; o objetivo da aula é preservar o contrato entre os programas.

---

## 6. Primeira interface supervisória

Somente depois que o consumidor CLI funcionar, substitua a apresentação:

```python
import streamlit as st

leituras = carregar_leituras(Path("dados/leituras.jsonl"))
st.title("Supervisório didático")
st.dataframe(leituras)
```

Crie `requirements.txt` com a dependência `streamlit`; inclua também o import de `Path` e a função `carregar_leituras` no módulo `supervisor.py`. Execute:

```bash
python3 -m pip install -r requirements.txt
streamlit run supervisor.py
```

Streamlit apresenta os dados; ele não deve redefinir o contrato nem decidir regras de controle.

---

## 7. Teste de contrato e fluxo profissional

O CI deve executar, no mínimo:

1. build do produtor C++ com avisos habilitados;
2. execução do produtor em diretório temporário;
3. parsing de todas as linhas com `json.loads`;
4. validação dos seis campos e tipos;
5. execução do consumidor Python.

Continue na branch `projeto/02-integracao`. Implemente os testes acima e acrescente sua execução ao alvo `make test-projeto`, mantendo as regressões da etapa 14. A CI executa exatamente esse alvo após o push. Use diretório temporário para que execuções repetidas não acumulem registros de testes anteriores.

Faça commit e push para `origin`; abra PR para a main do próprio fork, com saída local, link da CI e registro do uso de IA. Integre depois de concluir a próxima atividade de resiliência.

No PR, registre uma decisão: por que JSONL foi escolhido em vez de um array JSON único? Resposta esperada: cada evento pode ser acrescentado e processado linha a linha.

---

## 8. Mini-caso prático

O controlador produz cinco leituras e o supervisor as ordena pelo timestamp. Introduza então uma linha truncada. O consumidor atual encerra com erro: esse resultado observável prepara a aplicação das exceções da seção 10 à fronteira de integração.

---

## Perguntas de revisão rápida

1. Qual é a diferença entre formato de serialização e meio de transporte?
2. Por que JSON válido ainda pode violar o contrato de telemetria?
3. Por que o supervisor CLI deve funcionar antes do Streamlit?

## Fontes de referência

- [JSON — RFC 8259](https://www.rfc-editor.org/rfc/rfc8259)
- [Python Docs — `json`](https://docs.python.org/3/library/json.html)
- [cppreference — file streams](https://en.cppreference.com/w/cpp/io/basic_ofstream)
- [cppreference — `chrono`](https://en.cppreference.com/w/cpp/chrono)
- [Python Docs — `datetime`](https://docs.python.org/3/library/datetime.html)
- [Streamlit Docs](https://docs.streamlit.io/)
