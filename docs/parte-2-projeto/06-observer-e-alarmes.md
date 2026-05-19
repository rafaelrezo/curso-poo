# Aula 6 - Observer, Eventos e Alarmes

## Objetivos de aprendizagem

- Modelar eventos de leitura é alarmes operacionais.
- Aplicar Observer para desacoplar sensores, controle e notificações.
- Criar alarmes automaticos sem espalhar regras de aviso pelo código.

**Tempo estimado:** 1h30.

## Vídeo da aula

![type:video](https://www.youtube.com/embed/wX2gozSqHfU)

---

## 1. Contextualização do problema

O supervisor precisa destacar situacoes importantes:

- nível muito baixo;
- nível muito alto;
- pressão elevada;
- bomba bloqueada;
- sensor com leitura inválida.

Uma solução ruim seria cada sensor chamar diretamente o Streamlit ou gravar mensagens por conta própria. Isso mistura responsabilidades.

Observer permite que um objeto publique eventos e outros objetos sejam notificados sem acoplamento forte.

---

## 2. Explicação teórica

Observer é um padrão em que:

- o sujeito observado mantém uma lista de observadores;
- observadores implementam uma interface comum;
- quando ocorre um evento, todos os observadores são notificados.

### No projeto

| Papel no Observer | Classe possível |
|---|---|
| Sujeito observado | `EstacaoBombeamento` ou `Sensor` |
| Observador | `GerenciadorAlarmes` |
| Evento | `EventoOperacional` |
| Ação | criar alarme, registrar log ou atualizar histórico |

---

## 3. Evento operacional

```cpp
struct EventoOperacional {
    string origem;
    string tipo;
    string mensagem;
    string severidade;
};
```

Exemplos:

```text
origem: PT-201
tipo: PRESSAO_ALTA
mensagem: Pressão acima do limite operacional
severidade: alta
```

---

## 4. Interface de observador

```cpp
class ObservadorEvento {
public:
    virtual ~ObservadorEvento() = default;
    virtual void notificar(const EventoOperacional& evento) = 0;
};
```

### Gerenciador de alarmes

```cpp
#include <vector>

class GerenciadorAlarmes : public ObservadorEvento {
private:
    vector<EventoOperacional> alarmes;

public:
    void notificar(const EventoOperacional& evento) override {
        if (evento.severidade == "alta" || evento.severidade == "media") {
            alarmes.push_back(evento);
        }
    }

    const vector<EventoOperacional>& listarAlarmes() const {
        return alarmes;
    }
};
```

---

## 5. Publicacao de eventos

```cpp
class EstacaoBombeamento {
private:
    vector<ObservadorEvento*> observadores;

public:
    void adicionarObservador(ObservadorEvento* observador) {
        observadores.push_back(observador);
    }

    void publicar(const EventoOperacional& evento) {
        for (auto* observador : observadores) {
            observador->notificar(evento);
        }
    }

    void avaliarPressao(double pressao) {
        if (pressao > 6.0) {
            publicar({"PT-201", "PRESSAO_ALTA", "Pressao acima do limite", "alta"});
        }
    }
};
```

### O que observar

A estação não precisa saber como o alarme será salvo ou exibido. Ela apenas publica um evento.

---

## 6. Tipos de alarmes obrigatórios

Para o projeto final, use pelo menos dois tipos:

| Alarme | Condição | Severidade sugerida |
|---|---|---|
| `NIVEL_CRITICO` | nível menor que `15%` ou maior que `90%` | média |
| `PRESSAO_ALTA` | pressão maior que limite seguro | alta |
| `BOMBA_BLOQUEADA` | comando de bloqueio executado | alta |
| `LEITURA_INVALIDA` | JSON ou valor fora de faixa | média |

---

## 7. Supervisor em Python

```python
import pandas as pd
import streamlit as st

alarmes = [
    {"origem": "PT-201", "tipo": "PRESSAO_ALTA", "severidade": "alta"},
    {"origem": "LT-101", "tipo": "NIVEL_CRITICO", "severidade": "media"},
]

df = pd.DataFrame(alarmes)
st.subheader("Alarmes")
st.dataframe(df)
st.metric("Alarmes ativos", len(df))
```

---

## 8. Aplicação no projeto

O JSON pode passar a incluir alarmes:

```json
{
  "timestamp": "2026-03-25T10:50:00-03:00",
  "alarmes": [
    {
      "origem": "PT-201",
      "tipo": "PRESSAO_ALTA",
      "mensagem": "Pressão acima do limite",
      "severidade": "alta"
    }
  ]
}
```

---

## 9. Exercícios práticos

1. Criar `EventoOperacional`.
2. Criar a interface `ObservadorEvento`.
3. Criar `GerenciadorAlarmes`.
4. Fazer a estação publicar eventos quando houver pressão alta ou nível critico.
5. Mostrar alarmes no supervisor.

---

## 10. Checklist de entrega

- [ ] Existe uma estrutura para eventos.
- [ ] Existe uma interface de observador.
- [ ] Alarmes são criados sem o sensor conhecer o supervisor.
- [ ] Há pelo menos dois tipos de alarme.
- [ ] O JSON exporta os alarmes do ciclo.

---

## 11. Perguntas de revisão rápida

1. Qual acoplamento o Observer reduz neste projeto?
2. Por que um sensor não deveria chamar diretamente o Streamlit?
3. O que diferencia evento de alarme?

---

## 12. Desafios opcionais

- Criar um observador que grava eventos em arquivo.
- Criar severidades `baixa`, `média`, `alta` e `critica`.
- Permitir que o supervisor filtre alarmes por severidade.

---

## Fontes de referência

- [Refactoring Guru - Observer](https://refactoring.guru/design-patterns/observer)
- [C++ Core Guidelines - Interfaces](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Ri-abstract)
- [Streamlit Docs - Data display elements](https://docs.streamlit.io/develop/api-reference/data)
