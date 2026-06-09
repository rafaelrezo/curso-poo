# Aula 9 - Testes e Validação da Arquitetura

## Objetivos de aprendizagem

- Criar testes pequenos para regras, comandos, contrato JSON e persistência.
- Diferenciar teste unitário, teste de contrato e teste de integração simples.
- Usar testes para preparar a entrega e a defesa técnica do projeto final.

**Tempo estimado:** 2h

## Vídeo da aula

![type:video](https://www.youtube.com/embed/H2jDPK3HVzw)

---

## 1. Por que testar antes do projeto final?

Quando o sistema ainda é pequeno, parece suficiente executar o programa e olhar a saída. No projeto final, isso deixa de ser confiável.

Uma alteração pequena pode quebrar:

- a regra de ligar a bomba;
- o bloqueio por pressão alta;
- a geração de comando;
- o formato do JSON;
- a separação entre leituras válidas e inválidas;
- a gravação no banco;
- a lista de alarmes exibida no supervisor.

Teste é uma forma de transformar uma expectativa em verificação automática.

Exemplo de expectativa:

```text
Se o nível estiver abaixo de 30% e a pressão estiver normal,
a regra deve gerar LIGAR_BOMBA.
```

Essa frase pode virar teste.

---

## 2. O que testar neste projeto

O projeto possui duas partes principais:

- controlador/dispositivo em `C++`;
- supervisor em `Python/Streamlit`.

Os testes não precisam cobrir tudo com a mesma profundidade. O foco é proteger os pontos que definem o comportamento do sistema.

| Camada | O que testar | Exemplo |
|---|---|---|
| Regra de controle | entrada operacional gera decisão correta | nível baixo gera `LIGAR_BOMBA` |
| Comando | ação altera estado esperado | `BloquearPartida` bloqueia bomba |
| Contrato JSON | campos obrigatórios e tipos | `tag`, `valor`, `unidade`, `timestamp`, `status` |
| Alarmes | condição crítica gera alarme | pressão alta gera `PRESSAO_ALTA` |
| Persistência | dado salvo pode ser consultado | leitura salva aparece em `listar_por_tag` |
| Integração simples | saída de uma parte entra na outra | JSON do C++ é aceito pelo Python |

---

## 3. Tipos de teste

| Tipo | Pergunta principal | Exemplo | Ferramenta simples |
|---|---|---|---|
| Unitário | uma função ou classe isolada funciona? | `ControlePorFaixa` decide corretamente | `assert` em C++ ou `pytest` em Python |
| Contrato | o formato combinado está sendo respeitado? | JSON contém campos obrigatórios | `pytest` |
| Integração simples | duas partes conversam corretamente? | C++ gera JSON que Python valida | `pytest` + arquivo de exemplo |
| Regressão | um erro corrigido continua corrigido? | leitura inválida não derruba importação | teste específico |
| Validação manual | a tela ajuda a entender o sistema? | gráfico, tabela e alarmes aparecem | checklist de demonstração |

Teste manual ainda é útil, principalmente para Streamlit. Mas ele não substitui testes automáticos das regras principais.

---

## 4. Teste unitário de Strategy em C++

Para começar com baixa complexidade, use `assert`.

```cpp
#include <cassert>

void deve_ligar_bomba_quando_nivel_baixo() {
    ControlePorFaixa regra;

    TipoComando comando = regra.decidir(25.0, 2.0);

    assert(comando == TipoComando::LigarBomba);
}

void deve_desligar_bomba_quando_nivel_alto() {
    ControlePorFaixa regra;

    TipoComando comando = regra.decidir(85.0, 2.0);

    assert(comando == TipoComando::DesligarBomba);
}

void deve_bloquear_partida_quando_pressao_alta() {
    ControlePorFaixa regra;

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

Compilação sugerida:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic tests/test_controle.cpp -o test_controle
./test_controle
```

Se o programa termina sem mensagem, os `asserts` passaram. Se algum `assert` falhar, o programa encerra indicando o ponto do erro.

---

## 5. Teste unitário de Command em C++

Command deve ser testado pelo efeito que causa.

```cpp
#include <cassert>

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

Para isso, a classe `Bomba` precisa expor métodos de consulta seguros, por exemplo:

```cpp
bool estaLigada() const {
    return ligada;
}

bool estaBloqueada() const {
    return bloqueada;
}
```

Esses métodos não quebram encapsulamento porque apenas consultam estado. Eles não permitem alterar atributos diretamente.

---

## 6. Teste de contrato JSON em Python

Contrato JSON é o combinado entre controlador e supervisor. Validar contrato não é apenas verificar se o JSON abre.

Um JSON pode ser sintaticamente válido e mesmo assim não servir para o projeto.

```json
{"tag": "LT-101", "valor": "alto"}
```

Esse JSON abre, mas está incompleto e usa tipo errado para `valor`.

### Função de validação

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

### Testes com `pytest`

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


def test_valor_textual_deve_falhar_quando_status_operando():
    leitura = {
        "tag": "LT-101",
        "valor": "alto",
        "unidade": "%",
        "timestamp": "2026-06-09T10:30:00-03:00",
        "status": "operando",
    }

    with pytest.raises(TypeError):
        validar_leitura(leitura)
```

Execução:

```bash
python3 -m pytest -q
```

---

## 7. Teste de alarmes

Alarmes devem ser testados por condição.

```python
def gerar_alarmes(leitura: dict) -> list[dict]:
    alarmes = []

    if leitura["tag"].startswith("PT") and leitura["valor"] > 6.0:
        alarmes.append({
            "origem": leitura["tag"],
            "alarme": "PRESSAO_ALTA",
            "severidade": "alta",
        })

    if leitura["tag"].startswith("LT") and leitura["valor"] > 90.0:
        alarmes.append({
            "origem": leitura["tag"],
            "alarme": "NIVEL_CRITICO",
            "severidade": "alta",
        })

    return alarmes
```

Teste:

```python
def test_pressao_alta_deve_gerar_alarme():
    leitura = {
        "tag": "PT-201",
        "valor": 7.2,
        "unidade": "bar",
        "timestamp": "2026-06-09T10:30:00-03:00",
        "status": "alerta",
    }

    alarmes = gerar_alarmes(leitura)

    assert len(alarmes) == 1
    assert alarmes[0]["alarme"] == "PRESSAO_ALTA"
    assert alarmes[0]["severidade"] == "alta"
```

Esse teste protege uma regra importante do projeto final.

---

## 8. Teste de Repository sem depender do SQLite

Antes de testar banco, é útil testar a lógica usando um repositório em memória.

```python
def test_repositorio_memoria_lista_por_tag():
    repositorio = RepositorioLeiturasMemoria()

    repositorio.salvar(Leitura(
        timestamp="2026-06-09T10:00:00-03:00",
        tag="LT-101",
        valor=40.0,
        unidade="%",
        status="operando",
    ))

    repositorio.salvar(Leitura(
        timestamp="2026-06-09T10:01:00-03:00",
        tag="PT-201",
        valor=2.7,
        unidade="bar",
        status="operando",
    ))

    leituras = repositorio.listar_por_tag("LT-101")

    assert len(leituras) == 1
    assert leituras[0].valor == 40.0
```

Esse teste é rápido e não depende de arquivo. Depois, a equipe pode criar um teste específico para SQLite usando um banco temporário.

---

## 9. Teste simples de integração

Integração simples verifica se uma parte consegue alimentar a outra.

Fluxo mínimo:

```text
controlador C++ -> arquivo JSON Lines -> validador Python
```

Arquivo `dados/leituras_jsonl_exemplo.txt`:

```json
{"tag":"LT-101","valor":42.0,"unidade":"%","timestamp":"2026-06-09T10:00:00-03:00","status":"operando"}
{"tag":"PT-201","valor":2.7,"unidade":"bar","timestamp":"2026-06-09T10:00:01-03:00","status":"operando"}
```

Teste:

```python
import json
from pathlib import Path


def test_arquivo_jsonl_do_controlador_respeita_contrato():
    caminho = Path("dados/leituras_jsonl_exemplo.txt")
    linhas = caminho.read_text(encoding="utf-8").splitlines()

    assert linhas

    for linha in linhas:
        leitura = json.loads(linha)
        validar_leitura(leitura)
```

Esse teste não garante que todo o sistema está perfeito, mas protege o contrato entre as duas partes.

---

## 10. Validação manual do Streamlit

Algumas coisas são melhor verificadas olhando a aplicação.

Checklist mínimo:

- [ ] tabela de leituras aparece;
- [ ] gráfico histórico mostra pelo menos duas variáveis;
- [ ] resumo de bombas ou atuadores aparece;
- [ ] lista de comandos aparece;
- [ ] lista de alarmes aparece;
- [ ] leitura inválida não derruba a aplicação;
- [ ] dados persistidos continuam disponíveis após reiniciar o supervisor.

Durante a apresentação, essa validação manual deve ser rápida e objetiva.

---

## 11. Matriz mínima de testes para o projeto final

Cada equipe deve ter uma matriz simples no README.

| Item testado | Tipo de teste | Evidência |
|---|---|---|
| regra de nível baixo | unitário | `test_controle` ou `pytest` |
| regra de pressão alta | unitário | `test_controle` ou `pytest` |
| comando de bloqueio | unitário | teste de `Command` |
| contrato JSON | contrato | `pytest` |
| alarme crítico | unitário | `pytest` |
| persistência | integração simples | salvar e consultar leitura |
| fluxo C++ -> Python | integração simples | JSON Lines validado |
| tela Streamlit | manual | checklist na apresentação |

Essa tabela ajuda a organizar a defesa técnica. O aluno deve conseguir explicar o que cada teste prova e o que ele não prova.

---

## 12. Erros comuns

| Erro | Consequência | Ação recomendada |
|---|---|---|
| testar apenas a tela | falhas internas passam despercebidas | testar regras e contrato separadamente |
| aceitar qualquer JSON | supervisor quebra depois | validar campos e tipos |
| teste depender de dado manual | resultado instável | criar dados de teste controlados |
| colocar SQL dentro da tela toda | difícil testar persistência | usar `Repository` |
| testar só caso feliz | falhas reais não aparecem | incluir leitura inválida e pressão alta |
| não documentar como rodar | professor não consegue reproduzir | colocar comandos no README |

---

## 13. Mini-caso prático

A dupla definiu uma regra própria:

```text
Se a vazão ficar abaixo de 12 m3/h por dois ciclos seguidos,
gerar FALHA_BAIXA_VAZAO e bloquear a partida.
```

Testes mínimos para essa regra:

1. uma leitura isolada de vazão baixa ainda não bloqueia;
2. duas leituras seguidas de vazão baixa bloqueiam;
3. vazão normal depois da falha registra recuperação ou mantém histórico;
4. o JSON do alarme possui `origem`, `alarme`, `severidade` e `timestamp`;
5. o supervisor exibe o alarme no histórico.

Esse mini-caso mostra como transformar uma regra específica em evidência técnica.

---

## 14. Exercícios práticos

1. Criar três testes para a estratégia de controle.
2. Criar dois testes para comandos de atuação.
3. Criar três testes de contrato JSON.
4. Criar um teste de alarme.
5. Criar um teste simples para repositório.
6. Criar uma matriz de testes no README.
7. Executar os testes antes de abrir o pull request final.

---

## 15. Checklist de entrega

- [ ] Há testes das regras principais.
- [ ] Há teste para pelo menos um comando.
- [ ] Há validação do contrato JSON.
- [ ] Há teste de alarme.
- [ ] Há teste simples de persistência ou repositório.
- [ ] Há teste ou evidência do fluxo C++ -> Python.
- [ ] O README explica como rodar os testes.
- [ ] A matriz de testes está documentada.

---

## 16. Perguntas de revisão rápida

1. Qual é a diferença entre validar JSON e testar contrato JSON?
2. Por que um repositório em memória ajuda nos testes?
3. O que um teste de integração simples prova no fluxo C++ -> Python?

---

## Fontes de referência

- [cppreference - assert](https://en.cppreference.com/w/cpp/error/assert)
- [Python Docs - unittest](https://docs.python.org/3/library/unittest.html)
- [pytest Docs](https://docs.pytest.org/)
- [Python Docs - json](https://docs.python.org/3/library/json.html)
- [GitHub Docs - GitHub Actions](https://docs.github.com/en/actions)
