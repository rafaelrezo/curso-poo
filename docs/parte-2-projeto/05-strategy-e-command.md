# Aula 5 - Strategy e Command Pattern na Estação

## Objetivos de aprendizagem

- Entender Strategy como forma de trocar regras sem alterar o controlador principal.
- Entender Command como representacao de ações de atuação.
- Implementar comandos `LIGAR_BOMBA`, `DESLIGAR_BOMBA` e `BLOQUEAR_PARTIDA`.

**Tempo estimado:** 1h30.

## Vídeo da aula

![type:video](https://www.youtube.com/embed/wX2gozSqHfU)

---

## 1. Contextualização do problema

Na aula anterior, as regras ficaram dentro de `ControladorEstacao`. Isso funciona para um projeto pequeno, mas com o tempo aparecem variações:

- regra conservadora para testes;
- regra mais agressiva para simulação;
- regra de emergencia;
- regra usada apenas durante manutenção.

Se cada nova regra exigir reescrever o controlador, o código ficará frágil. Strategy e Command ajudam a organizar essa evolução.

---

## 2. Strategy Pattern

Strategy representa uma família de algoritmos intercambiáveis.

No projeto:

- o algoritmo é a regra de controle;
- a entrada são as leituras da estação;
- a saída é um comando;
- o controlador usa a estratégia sem conhecer seus detalhes.

```cpp
enum class TipoComando {
    Nenhum,
    LigarBomba,
    DesligarBomba,
    BloquearPartida
};

class EstrategiaControle {
public:
    virtual ~EstrategiaControle() = default;
    virtual TipoComando decidir(double nivel, double pressao) const = 0;
};
```

### Estratégia concreta

```cpp
class ControlePorFaixa : public EstrategiaControle {
public:
    TipoComando decidir(double nivel, double pressao) const override {
        if (pressao > 6.0) {
            return TipoComando::BloquearPartida;
        }
        if (nivel < 30.0) {
            return TipoComando::LigarBomba;
        }
        if (nivel > 80.0) {
            return TipoComando::DesligarBomba;
        }
        return TipoComando::Nenhum;
    }
};
```

---

## 3. Command Pattern

Command transforma uma ação em objeto. Isso permite registrar, enfileirar, testar e executar comandos de forma padronizada.

```cpp
class Comando {
public:
    virtual ~Comando() = default;
    virtual void executar(Bomba& bomba) const = 0;
    virtual string nome() const = 0;
};

class LigarBomba : public Comando {
public:
    void executar(Bomba& bomba) const override {
        bomba.ligar();
    }

    string nome() const override {
        return "LIGAR_BOMBA";
    }
};

class BloquearPartida : public Comando {
public:
    void executar(Bomba& bomba) const override {
        bomba.bloquearPartida();
    }

    string nome() const override {
        return "BLOQUEAR_PARTIDA";
    }
};
```

---

## 4. Comparação dos padrões

| Técnica/Padrão | Melhor uso | Esforço | Entregável | Limitação |
|---|---|---|---|---|
| `if` direto | regra pequena e fixa | baixo | código rápido | cresce mal com variações |
| Strategy | trocar algoritmo de decisão | médio | classes de regra | exige montagem dos objetos |
| Command | representar ação como objeto | médio | histórico e execução padronizada | pode parecer excesso em projetos muito pequenos |
| State | comportamento depende do estado do ativo | médio | transições controladas | será mais útil quando houver muitos estados |

### Recomendação prática por cenário

- Use `if` direto durante prototipacao curta.
- Use Strategy quando houver mais de uma politica de controle.
- Use Command quando comandos precisarem ser registrados, testados ou enviados.

---

## 5. Aplicação no projeto

O fluxo fica assim:

```text
leituras -> EstrategiaControle -> TipoComando -> Comando -> Bomba
```

O controlador passa a coordenar objetos, em vez de concentrar todas as regras.

```cpp
class ControladorEstacao {
private:
    const EstrategiaControle& estrategia;

public:
    explicit ControladorEstacao(const EstrategiaControle& estrategia)
        : estrategia(estrategia) {}

    TipoComando avaliar(double nivel, double pressao) const {
        return estrategia.decidir(nivel, pressao);
    }
};
```

---

## 6. Registro do comando no JSON

```json
{
  "timestamp": "2026-03-25T10:45:00-03:00",
  "comando": "LIGAR_BOMBA",
  "origem": "ControlePorFaixa",
  "executado": true
}
```

Esse registro será usado na aula de persistência.

---

## 7. Ponte C++ -> Python

No supervisor, os comandos podem aparecer em uma tabela:

```python
import pandas as pd
import streamlit as st

comandos = [
    {"timestamp": "2026-03-25T10:45:00-03:00", "comando": "LIGAR_BOMBA", "executado": True},
    {"timestamp": "2026-03-25T10:46:00-03:00", "comando": "DESLIGAR_BOMBA", "executado": True},
]

st.dataframe(pd.DataFrame(comandos))
```

---

## 8. Exercícios práticos

1. Criar a interface `EstrategiaControle`.
2. Criar a classe `ControlePorFaixa`.
3. Criar a interface `Comando`.
4. Implementar `LigarBomba`, `DesligarBomba` e `BloquearPartida`.
5. Registrar o nome do comando executado no JSON.

---

## 9. Checklist de entrega

- [ ] A regra de controle está representada como Strategy.
- [ ] Os comandos estão representados como objetos.
- [ ] Existem comandos para ligar, desligar e bloquear partida.
- [ ] O JSON registra o comando gerado.
- [ ] O README explica onde Strategy e Command aparecem no projeto.

---

## 10. Perguntas de revisão rápida

1. Qual problema o Strategy resolve neste projeto?
2. Por que transformar comando em objeto pode ajudar nos testes?
3. Em que ponto Command seria exagero para um projeto muito pequeno?

---

## 11. Desafios opcionais

- Criar `ControleManualAssistido`, que nunca liga a bomba automaticamente.
- Criar uma fila de comandos pendentes.
- Adicionar um comando `RESETAR_BLOQUEIO`, exigindo justificativa no README.

---

## Fontes de referência

- [Refactoring Guru - Strategy](https://refactoring.guru/design-patterns/strategy)
- [Refactoring Guru - Command](https://refactoring.guru/design-patterns/command)
- [C++ Core Guidelines - Interfaces](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Ri-abstract)
