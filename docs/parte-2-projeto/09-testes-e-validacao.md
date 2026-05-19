# Aula 9 - Testes e Validação da Arquitetura

## Objetivos de aprendizagem

- Criar testes unitários para regras de negócio.
- Validar o contrato JSON consumido pelo supervisor.
- Executar testes simples de integração entre dispositivo e supervisor.

**Tempo estimado:** 1h30.

## Vídeo da aula

![type:video](https://www.youtube.com/embed/ZFDSPSU6JMw)

---

## 1. Contextualização do problema

Quando o projeto cresce, testar apenas "olhando a tela" deixa de ser suficiente. Uma mudança pequena pode quebrar:

- a regra de ligar bomba;
- a regra de bloqueio por pressão;
- o formato do JSON;
- a persistência;
- a leitura do supervisor.

Testes são uma forma de proteger o comportamento esperado.

---

## 2. Tipos de teste nesta disciplina

| Tipo | O que verifica | Exemplo |
|---|---|---|
| Unitário | uma classe ou função isolada | `ControlePorFaixa` retorna `LIGAR_BOMBA` |
| Contrato | formato é campos do JSON | `tag`, `valor`, `unidade`, `timestamp`, `status` existem |
| Integração simples | duas partes trabalhando juntas | C++ gera JSON e Python consegue ler |
| Validação manual | comportamento no Streamlit | tabela, gráfico e resumo aparecem |

---

## 3. Teste unitário em C++ sem framework

Para manter a barreira baixa, a equipe pode começar com `assert`.

```cpp
#include <cassert>

void deveLigarBombaQuandoNivelBaixo() {
    ControlePorFaixa regra;
    TipoComando comando = regra.decidir(25.0, 2.0);
    assert(comando == TipoComando::LigarBomba);
}

void deveBloquearQuandoPressaoAlta() {
    ControlePorFaixa regra;
    TipoComando comando = regra.decidir(25.0, 7.0);
    assert(comando == TipoComando::BloquearPartida);
}

int main() {
    deveLigarBombaQuandoNivelBaixo();
    deveBloquearQuandoPressaoAlta();
    return 0;
}
```

Depois, equipes mais avançadas podem usar Catch2 ou GoogleTest.

---

## 4. Teste de contrato JSON em Python

```python
def validar_leitura(leitura: dict) -> None:
    obrigatorios = {"tag", "valor", "unidade", "timestamp", "status"}
    faltantes = obrigatorios - set(leitura)

    if faltantes:
        raise ValueError(f"Campos ausentes: {sorted(faltantes)}")

    if not isinstance(leitura["tag"], str):
        raise TypeError("tag deve ser texto")

    if not isinstance(leitura["valor"], int | float):
        raise TypeError("valor deve ser numérico")
```

### Teste com `pytest`

```python
import pytest


def test_leitura_valida():
    leitura = {
        "tag": "LT-101",
        "valor": 58.4,
        "unidade": "%",
        "timestamp": "2026-03-25T10:30:00-03:00",
        "status": "operando",
    }

    validar_leitura(leitura)


def test_leitura_sem_tag_deve_falhar():
    leitura = {"valor": 58.4, "unidade": "%", "timestamp": "x", "status": "operando"}

    with pytest.raises(ValueError):
        validar_leitura(leitura)
```

---

## 5. Teste simples de integração

Um teste de integração simples pode seguir estes passos:

1. executar o dispositivo C++;
2. gerar `dados/leitura.json` ou enviar JSON por TCP;
3. executar uma função Python que carrega e valida os dados;
4. verificar se a tabela do supervisor receberia as colunas esperadas.

```python
import json
from pathlib import Path


def test_json_gerado_pelo_dispositivo_tem_contrato_minimo():
    leitura = json.loads(Path("dados/leitura.json").read_text(encoding="utf-8"))
    validar_leitura(leitura)
```

---

## 6. Validação da arquitetura

Antes da entrega final, a equipe deve revisar se o projeto ainda respeita a separação planejada.

| Pergunta | Sinal de problema |
|---|---|
| O Streamlit decide ligar bomba? | regra de controle vazou para a interface |
| Sensor altera diretamente a bomba? | acoplamento forte entre leitura e atuação |
| JSON tem campos diferentes em cada arquivo? | contrato instável |
| Comando é apenas string espalhada? | Command Pattern não ficou explícito |
| Alarme depende da tela para existir? | Observer não está desacoplado |

---

## 7. Exercícios práticos

1. Criar pelo menos três testes para `ControlePorFaixa`.
2. Criar testes de contrato JSON em Python.
3. Criar um JSON inválido e confirmar que o teste falha.
4. Criar um teste para persistência em CSV ou SQLite.
5. Atualizar o README com comandos para executar os testes.

---

## 8. Checklist de entrega

- [ ] Há testes das regras de negócio.
- [ ] Há validação do contrato JSON.
- [ ] Há pelo menos um teste de integração simples.
- [ ] O README explica como rodar os testes.
- [ ] A equipe corrigiu pelo menos um problema encontrado pelos testes.

---

## 9. Perguntas de revisão rápida

1. Qual é a diferença entre teste unitário e teste de integração?
2. Por que validar contrato JSON é diferente de apenas carregar JSON?
3. Que problema arquitetural aparece quando o Streamlit passa a controlar a bomba?

---

## 10. Desafios opcionais

- Usar GitHub Actions para rodar testes automaticamente.
- Criar teste de regressão para um bug encontrado pela equipe.
- Criar uma pequena matriz de testes no README.

---

## Fontes de referência

- [Python Docs - unittest](https://docs.python.org/3/library/unittest.html)
- [pytest Docs](https://docs.pytest.org/)
- [cppreference - assert](https://en.cppreference.com/w/cpp/error/assert)
- [GitHub Docs - GitHub Actions](https://docs.github.com/en/actions)
