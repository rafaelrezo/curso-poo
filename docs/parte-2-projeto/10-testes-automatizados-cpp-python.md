# Aula 10 - Testes Automatizados em C++ e Python

## Objetivos de aprendizagem

- Escrever testes executáveis para regras, comandos, contrato JSON, alarmes e persistência.
- Rodar testes localmente com comandos reproduzíveis no terminal.
- Organizar o projeto para que a lógica seja testável sem depender da interface.

**Tempo estimado:** 1h30.

## Vídeo da aula

![type:video](https://www.youtube.com/embed/H2jDPK3HVzw)

---

## 1. Estrutura recomendada

O projeto deve separar código de produção, testes e dados controlados.

```text
estacao-bombeamento/
  dispositivo_cpp/
    include/
    src/
    tests/
    build/
  supervisor_python/
    app.py
    modelos.py
    repositorios.py
    validacao.py
    tests/
  dados/
    leituras_exemplo.jsonl
  README.md
```

Regra prática: teste não deve depender de arquivo gerado manualmente durante a aula, a menos que esse arquivo faça parte explícita do teste de integração.

!!! tip "Repositório de referência"

    A estrutura abaixo pode ser comparada com o exemplo público [`testes_ci_estacao`](https://github.com/rafaelrezo/curso-poo-exemplos/tree/main/testes_ci_estacao). Use como visualização opcional para baixar, rodar e observar os testes, não como projeto final pronto.

---

## 2. Teste unitário de Strategy em C++

Para começar, `assert` é suficiente. Ele permite mostrar o conceito sem instalar framework externo.

Arquivo `dispositivo_cpp/tests/test_controle.cpp`:

```cpp
#include <cassert>

#include "estrategia_controle.hpp"
#include "tipo_comando.hpp"

void deve_ligar_bomba_quando_nivel_baixo() {
    ControleNormal regra;

    TipoComando comando = regra.decidir(25.0, 2.0);

    assert(comando == TipoComando::LigarBomba);
}

void deve_desligar_bomba_quando_nivel_alto() {
    ControleNormal regra;

    TipoComando comando = regra.decidir(85.0, 2.0);

    assert(comando == TipoComando::DesligarBomba);
}

void deve_bloquear_partida_quando_pressao_alta() {
    ControleNormal regra;

    TipoComando comando = regra.decidir(25.0, 7.0);

    assert(comando == TipoComando::BloquearPartida);
}

int main() {
    deve_ligar_bomba_quando_nivel_baixo();
    deve_desligar_bomba_quando_nivel_alto();
    deve_bloquear_partida_quando_pressao_alta();
    return 0;
}
```

Execução local:

```bash
cd dispositivo_cpp
mkdir -p build
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude tests/test_controle.cpp -o build/test_controle
./build/test_controle
```

Se o programa termina sem mensagem, os testes passaram. Se algum `assert` falhar, a execução é interrompida no ponto do erro.

No repositório de referência, veja o arquivo [`dispositivo_cpp/tests/test_controlador.cpp`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/testes_ci_estacao/dispositivo_cpp/tests/test_controlador.cpp) para uma versão completa com Strategy, Command e telemetria JSON.

---

## 3. Teste unitário de Command em C++

Command deve ser testado pelo efeito produzido.

```cpp
#include <cassert>

#include "bomba.hpp"
#include "comando.hpp"

void deve_bloquear_bomba() {
    Bomba bomba;
    BloquearPartida comando;

    comando.executar(bomba);

    assert(bomba.estaBloqueada());
    assert(!bomba.estaLigada());
}

void deve_ligar_bomba_quando_nao_estiver_bloqueada() {
    Bomba bomba;
    LigarBomba comando;

    comando.executar(bomba);

    assert(bomba.estaLigada());
}
```

Os métodos `estaLigada()` e `estaBloqueada()` não quebram encapsulamento. Eles expõem consulta segura de estado, não acesso direto aos atributos internos.

---

## 4. Quando usar GoogleTest

`assert` é bom para começar. Em projetos maiores, frameworks como GoogleTest melhoram relatório de falhas, organização de casos e integração com CMake.

| Técnica/Padrão | Melhor uso | Esforço | Entregável | Limitação |
|---|---|---|---|---|
| `assert` puro | primeiros testes e exemplos pequenos | baixo | executável simples | relatório limitado |
| GoogleTest | muitos testes C++ com boa saída | médio | suíte com `TEST(...)` | exige dependência e build |
| CTest/CMake | execução padronizada de várias suítes | médio | `ctest` no terminal e no CI | exige organização inicial |

Recomendação para o curso: entregar com `assert` é aceitável quando os testes são claros e reproduzíveis. GoogleTest é um desafio opcional para equipes que já dominam o básico.

---

## 5. Teste de contrato JSON em Python

Contrato JSON é o combinado entre controlador e supervisor. Validar contrato não é apenas verificar se o texto é JSON válido.

JSON sintaticamente válido, mas inválido para o projeto:

```json
{"tag": "LT-101", "valor": "alto"}
```

Função de validação:

```python
def validar_leitura(leitura: dict) -> None:
    obrigatorios = {"tag", "valor", "unidade", "timestamp", "status"}
    faltantes = obrigatorios - set(leitura)

    if faltantes:
        raise ValueError(f"Campos ausentes: {sorted(faltantes)}")

    if not isinstance(leitura["tag"], str):
        raise TypeError("tag deve ser texto")

    if not isinstance(leitura["unidade"], str):
        raise TypeError("unidade deve ser texto")

    if not isinstance(leitura["timestamp"], str):
        raise TypeError("timestamp deve ser texto")

    if not isinstance(leitura["status"], str):
        raise TypeError("status deve ser texto")

    if leitura["status"] != "falha" and not isinstance(leitura["valor"], int | float):
        raise TypeError("valor deve ser numerico quando status nao e falha")
```

Testes com `pytest`:

```python
import pytest


def test_leitura_valida_deve_passar():
    leitura = {
        "tag": "LT-101",
        "valor": 58.4,
        "unidade": "%",
        "timestamp": "2026-06-09T10:30:00-03:00",
        "status": "operando",
    }

    validar_leitura(leitura)


def test_leitura_sem_tag_deve_falhar():
    leitura = {
        "valor": 58.4,
        "unidade": "%",
        "timestamp": "2026-06-09T10:30:00-03:00",
        "status": "operando",
    }

    with pytest.raises(ValueError):
        validar_leitura(leitura)
```

Execução local:

```bash
cd supervisor_python
python3 -m pytest -q
```

---

## 6. Parametrização de casos

Quando a mesma regra precisa ser testada com várias entradas, `pytest.mark.parametrize` reduz repetição.

```python
import pytest


@pytest.mark.parametrize(
    ("tag", "valor", "alarme_esperado"),
    [
        ("PT-201", 7.2, "PRESSAO_ALTA"),
        ("LT-101", 92.0, "NIVEL_CRITICO"),
    ],
)
def test_condicoes_criticas_geram_alarmes(tag, valor, alarme_esperado):
    leitura = {
        "tag": tag,
        "valor": valor,
        "unidade": "%",
        "timestamp": "2026-06-09T10:30:00-03:00",
        "status": "alerta",
    }

    alarmes = gerar_alarmes(leitura)

    assert alarmes[0]["alarme"] == alarme_esperado
```

Impacto prático: a equipe consegue mostrar mais cenários sem criar cinco funções quase iguais.

---

## 7. Teste de Repository em memória

Antes de testar banco, teste o contrato esperado do repositório.

```python
def test_repositorio_memoria_lista_todas():
    repositorio = RepositorioLeiturasMemoria()

    repositorio.salvar(Leitura(
        timestamp="2026-06-09T10:00:00-03:00",
        tag="LT-101",
        valor=40.0,
        unidade="%",
        status="operando",
    ))

    leituras = repositorio.listar_todas()

    assert len(leituras) == 1
    assert leituras[0].tag == "LT-101"
    assert leituras[0].valor == 40.0
```

Esse teste é rápido e não depende de arquivo.

---

## 8. Teste de SQLite com banco temporário

Para testar o repositório real sem sujar o banco do projeto, use arquivo temporário.

```python
def test_repositorio_sqlite_salva_e_lista(tmp_path):
    caminho_banco = tmp_path / "teste.db"
    repositorio = RepositorioLeiturasSqlite(str(caminho_banco))
    repositorio.inicializar()

    repositorio.salvar(Leitura(
        timestamp="2026-06-09T10:00:00-03:00",
        tag="PT-201",
        valor=2.7,
        unidade="bar",
        status="operando",
    ))

    leituras = repositorio.listar_todas()

    assert len(leituras) == 1
    assert leituras[0].tag == "PT-201"
```

Esse já é teste de integração, porque envolve a camada Python e o SQLite.

---

## 9. Teste simples de integração C++ -> Python

Fluxo mínimo:

```text
controlador C++ -> arquivo JSON Lines -> validador Python
```

Arquivo `dados/leituras_exemplo.jsonl`:

```json
{"tag":"LT-101","valor":42.0,"unidade":"%","timestamp":"2026-06-09T10:00:00-03:00","status":"operando"}
{"tag":"PT-201","valor":2.7,"unidade":"bar","timestamp":"2026-06-09T10:00:01-03:00","status":"operando"}
```

Teste:

```python
import json
from pathlib import Path


def test_jsonl_do_controlador_respeita_contrato():
    caminho = Path("../dados/leituras_exemplo.jsonl")
    linhas = caminho.read_text(encoding="utf-8").splitlines()

    assert linhas

    for linha in linhas:
        leitura = json.loads(linha)
        validar_leitura(leitura)
```

Esse teste não prova a interface inteira, mas protege o contrato entre C++ e Python.

No repositório de referência, veja [`dados/leituras_exemplo.jsonl`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/testes_ci_estacao/dados/leituras_exemplo.jsonl) e [`supervisor_python/tests/test_ingestao.py`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/testes_ci_estacao/supervisor_python/tests/test_ingestao.py).

---

## 10. Teste de fumaça do Streamlit

Quando a lógica está separada da tela, a maior parte dos testes fica fora do Streamlit. Ainda assim, um teste de fumaça pode verificar se o app carrega.

```python
from streamlit.testing.v1 import AppTest


def test_app_streamlit_inicia_sem_excecao():
    app = AppTest.from_file("app.py")
    app.run()

    assert not app.exception
```

Esse teste é útil antes da entrega, mas não deve ser a única evidência de qualidade.

---

## 11. README mínimo de testes

Cada equipe deve documentar como reproduzir os testes.

````markdown
## Como rodar os testes

### C++

```bash
cd dispositivo_cpp
mkdir -p build
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude tests/test_controle.cpp -o build/test_controle
./build/test_controle
```

### Python

```bash
cd supervisor_python
python3 -m pip install -r requirements.txt
python3 -m pytest -q
```

### Validação manual

- abrir o Streamlit;
- carregar dados de exemplo;
- conferir tabela, gráfico, alarmes e comandos;
- reiniciar o app e verificar persistência.
````

---

## 12. Mini-caso prático

A equipe adicionou o sensor `FT-301` de vazão. A regra é:

```text
vazão menor que 12 m3/h por dois ciclos seguidos deve gerar falha.
```

Plano de testes:

| Item testado | Tipo | Evidência |
|---|---|---|
| uma leitura baixa | unitário | ainda não bloqueia |
| duas leituras baixas seguidas | unitário | gera bloqueio |
| JSON do alarme | contrato | campos obrigatórios existem |
| gravação do alarme | integração | aparece no SQLite |
| tela de alarmes | fumaça/aceitação | alarme aparece na interface |

---

## 13. Exercícios práticos

1. Criar três testes para a estratégia de controle.
2. Criar dois testes para comandos de atuação.
3. Criar três testes de contrato JSON.
4. Criar um teste de alarme.
5. Criar um teste de Repository em memória.
6. Criar um teste de SQLite com banco temporário.
7. Atualizar o README com comandos de execução.

---

## 14. Perguntas de revisão rápida

1. Por que `tmp_path` ajuda a testar SQLite?
2. Qual é a vantagem de parametrizar testes?
3. Por que separar lógica da tela facilita testar o Streamlit?

---

## Fontes de referência

- [cppreference - `assert`](https://en.cppreference.com/w/cpp/error/assert)
- [GoogleTest - Quickstart CMake](https://google.github.io/googletest/quickstart-cmake.html)
- [pytest Docs - How to invoke pytest](https://docs.pytest.org/en/stable/how-to/usage.html)
- [pytest Docs - Temporary directories and files](https://docs.pytest.org/en/stable/how-to/tmp_path.html)
- [Streamlit Docs - Get started with app testing](https://docs.streamlit.io/develop/concepts/app-testing/get-started)
