# Datas, horarios e timestamps em C++ e Python

## Objetivos de aprendizagem

- Diferenciar data/hora de calendário, timestamp Unix e duração medida por relogio monotono.
- Representar instantes em JSON de forma consistente usando UTC e formato ISO 8601.
- Comparar as APIs de tempo em C++ e Python em problemas de telemetria, logs e integração.

**Tempo estimado:** 2h

## Vídeo da aula

![type:video](https://www.youtube.com/embed/oIwRnMT6A6o)

---

## 1. Por que tempo e um problema de software?

Datas e horarios parecem simples ate entrarem em uma aplicação real:

- um sensor registra uma leitura a cada minuto;
- um log precisa mostrar quando uma falha ocorreu;
- um dashboard ordena eventos vindos de maquinas diferentes;
- uma API envia dados para outro sistema em outro fuso horário;
- um teste mede quanto tempo uma operação demorou.

Nesses casos, errar tempo gera erro de análise. Uma leitura pode parecer fora de ordem, um alarme pode ser interpretado no dia errado e uma duração pode ser calculada com o relogio errado.

**Ideia central desta aula:** antes de programar datas, decida se você está representando um instante, uma data de calendário ou uma duração.

---

## 2. Tres ideias que não devem ser misturadas

| Conceito | O que representa | Exemplo | Uso recomendado |
|---|---|---|---|
| Data/hora de calendário | uma representacao humana | `29/04/2026 09:00` | telas, relatorios e agendas |
| Timestamp Unix | segundos desde `1970-01-01T00:00:00Z` | `1777464000` | armazenamento, ordenacao e comparação |
| Duracao | intervalo entre dois eventos | `250 ms` | medir desempenho, timeout e tempo de ciclo |

### Ação recomendada

- Para integração entre sistemas, prefira instantes em UTC.
- Para JSON, use string ISO 8601, como `"2026-04-29T12:00:00Z"`.
- Para medir duração de código, use relogio monotono, não calendário.

---

## 3. Timestamp em JSON

JSON não tem tipo nativo de data. Por isso, datas costumam ser representadas como string.

```json
{
  "timestamp": "2026-04-29T12:00:00Z",
  "temperatura_c": 31.8
}
```

Neste exemplo:

- `2026-04-29` e a data;
- `T` separa data e hora;
- `12:00:00` e o horário;
- `Z` indica UTC.

### Por que UTC?

UTC evita que cada computador interprete o mesmo instante no seu fuso local.

Para o aluno, a regra inicial pode ser:

1. grave no JSON em UTC;
2. processe internamente em UTC;
3. converta para horário local apenas na interface, quando necessário.

---

## 4. C++: `chrono`, `system_clock` e `steady_clock`

Em C++, a biblioteca `<chrono>` separa bem duas necessidades:

- `system_clock`: representa o relogio do sistema, útil para data e hora de calendário;
- `steady_clock`: relogio monotono, útil para medir duracoes sem sofrer ajuste manual do horário do computador.

O exemplo completo está em:

- [exemplo_datas_timestamps.cpp](./exemplo_datas_timestamps.cpp)

### Trecho central

```cpp
auto agora = chrono::system_clock::now();
auto proxima_leitura = agora + chrono::minutes(15);

auto inicio_medicao = chrono::steady_clock::now();
// operacao medida
auto fim_medicao = chrono::steady_clock::now();
```

### Como compilar

```bash
g++ -std=c++17 docs/fundamentos_poo_cpp_python/07_data_timestamp/exemplo_datas_timestamps.cpp -o exemplo_tempo
./exemplo_tempo
```

### Leitura critica

- `system_clock::now()` serve para dizer "quando aconteceu".
- `steady_clock::now()` serve para medir "quanto demorou".
- `duration_cast` converte duracoes para a unidade desejada.
- `put_time` formata o calendário como texto.

---

## 5. Python: `datetime`, `timezone`, `timedelta` e `perf_counter`

Python também separa os conceitos.

O exemplo completo está em:

- [exemplo_datas_timestamps.py](./exemplo_datas_timestamps.py)

### Trecho central

```python
from datetime import datetime, timedelta, timezone

agora = datetime.now(timezone.utc)
proxima_leitura = agora + timedelta(minutes=15)
timestamp_segundos = agora.timestamp()
```

Para medir duração de execução:

```python
from time import perf_counter

inicio = perf_counter()
# operacao medida
fim = perf_counter()
duracao = fim - inicio
```

### Como executar

```bash
python docs/fundamentos_poo_cpp_python/07_data_timestamp/exemplo_datas_timestamps.py
```

---

## 6. Ponte C++ -> Python

| Tarefa | C++ | Python | Cuidado principal |
|---|---|---|---|
| Instante atual | `chrono::system_clock::now()` | `datetime.now(timezone.utc)` | registrar em UTC |
| Somar intervalo | `agora + chrono::minutes(15)` | `agora + timedelta(minutes=15)` | unidade clara |
| Timestamp Unix | `time_since_epoch()` | `.timestamp()` | segundos vs milissegundos |
| Medir duração | `chrono::steady_clock` | `time.perf_counter()` | não usar relogio de calendário |
| Formatar para JSON | `put_time(..., "%Y-%m-%dT%H:%M:%SZ")` | `.isoformat()` com UTC | manter formato consistente |

### Exemplo de contrato entre linguagens

C++ escreve:

```json
{
  "timestamp": "2026-04-29T12:00:00Z",
  "cpu_percent": 51.2
}
```

Python interpreta:

```python
from datetime import datetime

texto = "2026-04-29T12:00:00Z"
instante = datetime.fromisoformat(texto.replace("Z", "+00:00"))
```

O contrato não e "Python entende qualquer data". O contrato e: o campo `timestamp` será uma string ISO 8601 em UTC.

---

## 7. Mini-caso prático: ordenar eventos de uma maquina

Uma maquina gera eventos de supervisão:

- início de ciclo;
- leitura de temperatura;
- alerta de vibracao;
- fim de ciclo.

Cada evento precisa ter um `timestamp`.

Se os eventos forem gravados como texto local, por exemplo `"29/04/2026 09:00"`, outro sistema pode não saber:

- se o formato é dia/mes/ano ou mes/dia/ano;
- qual fuso horário foi usado;
- se houve horário de verao ou ajuste manual do relogio.

Com ISO 8601 em UTC:

```json
{
  "evento": "alerta_vibracao",
  "timestamp": "2026-04-29T12:00:00Z",
  "severidade": "media"
}
```

O sistema Python consegue ordenar, filtrar e calcular intervalos com menor ambiguidade.

---

## 8. Atividade guiada

### Enunciado

Altere o JSON da aula anterior para incluir:

- `timestamp` em cada leitura;
- `duracao_ciclo_ms` como duração numérica;
- `proxima_manutencao` como data planejada em UTC.

### Regras

1. `timestamp` deve usar formato ISO 8601 em UTC.
2. `duracao_ciclo_ms` deve ser número, não string.
3. O app Python deve converter `timestamp` para `datetime`.
4. A tabela final deve aparecer ordenada pelo timestamp.
5. O aluno deve explicar a diferença entre instante e duração.

### Pergunta de projeto

Se a aplicação for executada em computadores com fuso horário diferente, o que ainda funciona corretamente e o que precisa ser convertido apenas na interface?

---

## Perguntas de revisão rápida

1. Qual a diferença entre timestamp Unix e uma data formatada para humanos?
2. Por que `steady_clock` em C++ e `perf_counter()` em Python são melhores para medir duração?
3. Por que JSON precisa representar data como string ou número, e não como um tipo próprio de data?

## Fontes de referência

- https://en.cppreference.com/w/cpp/chrono
- https://en.cppreference.com/w/cpp/chrono/system_clock
- https://en.cppreference.com/w/cpp/chrono/steady_clock
- https://en.cppreference.com/w/cpp/io/manip/put_time
- https://docs.python.org/3/library/datetime.html
- https://docs.python.org/3/library/time.html
- https://docs.python.org/3/library/zoneinfo.html
- https://www.json.org/json-en.html
