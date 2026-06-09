# Aula 8 - Padrões de Projeto II: Observer, Eventos e Alarmes

## Objetivos de aprendizagem

- Entender `Observer` como organização de arquivos e responsabilidades.
- Diferenciar leitura, evento e alarme no projeto da estação.
- Usar um repositório de apoio para estudar como eventos, observadores e estação se conectam.

**Tempo estimado:** 2h

## Vídeo da aula

![type:video](https://www.youtube.com/embed/iMV1aHaijhQ)

---

## 1. Retomada: padrão também organiza o projeto

Na aula anterior, `Strategy`, `Command` e `Repository` foram estudados a partir de arquivos separados.

Nesta aula, a mesma ideia será aplicada ao `Observer`.

O problema agora é:

```text
Como avisar várias partes do sistema quando algo importante acontece?
```

Exemplos:

- pressão alta;
- nível crítico;
- bomba bloqueada;
- leitura inválida;
- falha simulada da dupla.

Essas situações podem gerar diferentes reações:

- mostrar alarme no supervisor;
- gravar histórico;
- imprimir log;
- atualizar estatísticas;
- alimentar testes.

Se todas essas reações ficarem dentro da classe `EstacaoBombeamento`, a classe fica grande e difícil de manter.

---

## 2. Leitura, evento e alarme

Antes do padrão, separe três conceitos:

| Conceito | Pergunta que responde | Exemplo |
|---|---|---|
| Leitura | Qual valor foi medido? | `PT-201 = 7.2 bar` |
| Evento | O que aconteceu? | pressão passou do limite |
| Alarme | Esse evento exige atenção? | `PRESSAO_ALTA`, severidade `alta` |

Uma leitura é um dado medido.

```text
PT-201 mediu 7.2 bar
```

Um evento é uma ocorrência interpretada.

```text
Pressão acima do limite seguro
```

Um alarme é um evento que deve chamar atenção.

```text
PRESSAO_ALTA no sensor PT-201
```

Nem todo evento precisa virar alarme. `BOMBA_LIGADA` pode ser apenas histórico. `PRESSAO_ALTA` deve virar alarme.

---

## 3. Solução direta sem Observer

Uma solução inicial é colocar tudo dentro da estação:

```cpp
class EstacaoBombeamento {
private:
    vector<Alarme> alarmes;

public:
    void avaliarPressao(double pressao) {
        if (pressao > 6.0) {
            alarmes.push_back({"PT-201", "PRESSAO_ALTA", "alta"});
            cout << "ALARME: PRESSAO_ALTA" << endl;
            // salvarNoBanco(...)
            // atualizarSupervisor(...)
            // registrarLog(...)
        }
    }
};
```

Essa solução funciona no começo, mas mistura responsabilidades:

- a estação detecta o problema;
- a estação cria o alarme;
- a estação imprime log;
- a estação pode salvar banco;
- a estação pode conhecer detalhes do supervisor.

Sinal de alerta:

```text
a estação deixa de cuidar da lógica da estação e começa a cuidar de tudo.
```

Contraexemplo no repositório de apoio:

- [C++ sem `Observer`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/padroes_ii_observer/sem_observer/dispositivo_cpp/main.cpp)

Use esse arquivo como referência de comparação. Ele executa, mas a estação concentra ações que depois serão separadas em observadores.

---

## 4. Ideia do Observer

`Observer` separa quem publica de quem reage.

| Papel | Responsabilidade |
|---|---|
| sujeito observado | publica eventos |
| observador | recebe eventos e executa uma reação |

No projeto:

| Papel no Observer | Arquivo no exemplo |
|---|---|
| evento | `evento_operacional.hpp` |
| interface do observador | `observador_evento.hpp` |
| sujeito observado | `estacao_bombeamento.hpp` |
| observador de alarmes | `gerenciador_alarmes.hpp` |
| observador de log | `logger_console.hpp` |
| observador de histórico | `registrador_eventos.hpp` |

Ideia central:

```text
A estação publica o evento.
Os observadores decidem o que fazer com ele.
```

---

## 5. Estrutura do exemplo de apoio

Abra o exemplo:

- [Repositório `curso-poo-exemplos`](https://github.com/rafaelrezo/curso-poo-exemplos)
- [Exemplo `padroes_ii_observer`](https://github.com/rafaelrezo/curso-poo-exemplos/tree/main/padroes_ii_observer)

Estrutura principal:

```text
padroes_ii_observer/
  sem_observer/
    dispositivo_cpp/
      main.cpp
  dispositivo_cpp/
    include/
      evento_operacional.hpp
      observador_evento.hpp
      gerenciador_alarmes.hpp
      logger_console.hpp
      registrador_eventos.hpp
      estacao_bombeamento.hpp
    src/
      main.cpp
```

Essa estrutura mostra a organização do padrão:

| Arquivo | Responsabilidade |
|---|---|
| `sem_observer/` | contraexemplo com reações concentradas na estação |
| `evento_operacional.hpp` | define a informação publicada |
| `observador_evento.hpp` | define o contrato dos observadores |
| `gerenciador_alarmes.hpp` | guarda alarmes ativos |
| `logger_console.hpp` | imprime eventos |
| `registrador_eventos.hpp` | simula gravação de histórico |
| `estacao_bombeamento.hpp` | detecta condições e publica eventos |
| `main.cpp` | monta a estação e registra observadores |

Leitura recomendada:

1. Comece por `sem_observer/dispositivo_cpp/main.cpp`.
2. Observe que a estação cria alarme, imprime log e salva histórico.
3. Depois leia `evento_operacional.hpp` e `observador_evento.hpp`.
4. Compare como as reações foram movidas para observadores separados.

---

## 6. Evento: dado publicado pela estação

O evento está em:

- [Evento - `evento_operacional.hpp`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/padroes_ii_observer/dispositivo_cpp/include/evento_operacional.hpp)

Trecho:

```cpp
struct EventoOperacional {
    std::string origem;
    std::string tipo;
    std::string mensagem;
    std::string severidade;
};
```

Leitura:

| Campo | Papel |
|---|---|
| `origem` | quem gerou o evento |
| `tipo` | nome padronizado da ocorrência |
| `mensagem` | texto explicativo |
| `severidade` | impacto operacional |

O evento não sabe quem vai usar esses dados. Ele apenas carrega a informação.

---

## 7. Interface: contrato dos observadores

A interface está em:

- [Interface - `observador_evento.hpp`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/padroes_ii_observer/dispositivo_cpp/include/observador_evento.hpp)

Trecho:

```cpp
class ObservadorEvento {
public:
    virtual ~ObservadorEvento() = default;

    virtual void notificar(const EventoOperacional& evento) = 0;
};
```

Leitura:

- todo observador precisa implementar `notificar`;
- a estação não precisa conhecer a classe concreta;
- qualquer novo observador pode entrar se respeitar essa interface.

---

## 8. Observadores: reações separadas

No exemplo existem três observadores.

### Gerenciador de alarmes

- [Alarmes - `gerenciador_alarmes.hpp`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/padroes_ii_observer/dispositivo_cpp/include/gerenciador_alarmes.hpp)

Responsabilidade:

```text
receber eventos e guardar os que devem aparecer como alarmes.
```

### Logger de console

- [Logger - `logger_console.hpp`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/padroes_ii_observer/dispositivo_cpp/include/logger_console.hpp)

Responsabilidade:

```text
receber eventos e imprimir mensagens de diagnóstico.
```

### Registrador de eventos

- [Histórico - `registrador_eventos.hpp`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/padroes_ii_observer/dispositivo_cpp/include/registrador_eventos.hpp)

Responsabilidade:

```text
receber eventos e simular gravação de histórico.
```

Ganho:

```text
se uma nova reação for necessária, crie um novo observador.
Não reescreva a estação.
```

---

## 9. Estação: sujeito observado

A estação está em:

- [Estação - `estacao_bombeamento.hpp`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/padroes_ii_observer/dispositivo_cpp/include/estacao_bombeamento.hpp)

Trecho central:

```cpp
class EstacaoBombeamento {
private:
    std::vector<ObservadorEvento*> observadores;

public:
    void adicionarObservador(ObservadorEvento* observador) {
        observadores.push_back(observador);
    }

    void publicar(const EventoOperacional& evento) {
        for (auto* observador : observadores) {
            observador->notificar(evento);
        }
    }
};
```

Leitura:

- a estação guarda uma lista de observadores;
- quando algo acontece, ela chama `publicar`;
- `publicar` entrega o evento para todos os observadores;
- a estação não sabe se o evento será log, alarme ou histórico.

---

## 10. Fluxo completo

O fluxo completo está em:

- [C++ principal - `main.cpp`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/padroes_ii_observer/dispositivo_cpp/src/main.cpp)

Trecho:

```cpp
EstacaoBombeamento estacao;

GerenciadorAlarmes gerenciadorAlarmes;
LoggerConsole logger;
RegistradorEventos registrador;

estacao.adicionarObservador(&gerenciadorAlarmes);
estacao.adicionarObservador(&logger);
estacao.adicionarObservador(&registrador);

estacao.avaliarPressao(7.2);
estacao.avaliarNivel(92.0);
```

Leitura do fluxo:

```text
EstacaoBombeamento detecta pressão alta
        |
        v
publica EventoOperacional
        |
        +--> GerenciadorAlarmes guarda alarme
        |
        +--> LoggerConsole imprime log
        |
        +--> RegistradorEventos salva histórico
```

Esse fluxo mostra por que a organização em arquivos ajuda: cada reação está em um arquivo próprio.

---

## 11. Como executar o exemplo

Clone o repositório de exemplos:

```bash
git clone https://github.com/rafaelrezo/curso-poo-exemplos.git
cd curso-poo-exemplos/padroes_ii_observer
```

Compile e execute:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic \
  -I dispositivo_cpp/include \
  dispositivo_cpp/src/main.cpp \
  -o /tmp/padroes_ii_observer_cpp

/tmp/padroes_ii_observer_cpp
```

---

## 12. Ponte C++ -> Python: alarmes no supervisor

No supervisor, os alarmes podem ser representados como dados.

```python
from dataclasses import dataclass


@dataclass
class Alarme:
    origem: str
    tipo: str
    mensagem: str
    severidade: str
```

A regra principal continua a mesma:

```text
Streamlit exibe alarmes.
Streamlit não deve concentrar a regra que cria alarmes.
```

No projeto final, o supervisor pode carregar alarmes de JSON, CSV ou SQLite e apenas exibi-los em tabela, gráfico ou resumo.

---

## 13. Alarmes mínimos para o projeto final

O projeto final deve possuir pelo menos três tipos de alarme.

Uma seleção viável:

| Alarme | Condição sugerida | Severidade sugerida |
|---|---|---|
| `NIVEL_CRITICO` | nível `< 15%` ou `> 90%` | alta |
| `PRESSAO_ALTA` | pressão acima do limite seguro | alta |
| `BOMBA_BLOQUEADA` | comando de bloqueio executado | media |
| `LEITURA_INVALIDA` | valor fora da faixa ou JSON inválido | media |
| `FALHA_SIMULADA` | falha específica da dupla | definida pela equipe |

Recomendação prática:

- use pelo menos dois alarmes baseados em sensores;
- use pelo menos um alarme baseado em atuação ou falha;
- documente no README a condição de disparo de cada alarme;
- mostre no código qual observador trata cada consequência.

---

## 14. JSON para eventos e alarmes

Forma simples: um alarme por linha.

```json
{
  "timestamp": "2026-06-09T10:50:00-03:00",
  "tipo": "alarme",
  "origem": "PT-201",
  "alarme": "PRESSAO_ALTA",
  "mensagem": "Pressao acima do limite seguro",
  "severidade": "alta"
}
```

Forma agrupada: leituras, comandos e alarmes no mesmo ciclo.

```json
{
  "timestamp": "2026-06-09T10:50:00-03:00",
  "tipo": "ciclo",
  "leituras": [
    {"tag": "PT-201", "valor": 7.2, "unidade": "bar", "status": "alerta"}
  ],
  "comandos": [
    {"comando": "BLOQUEAR_PARTIDA", "alvo": "BMB-101", "executado": true}
  ],
  "alarmes": [
    {"origem": "PT-201", "alarme": "PRESSAO_ALTA", "severidade": "alta"}
  ]
}
```

Use a forma simples se a equipe ainda estiver consolidando o fluxo. Use a forma agrupada quando a equipe já estiver confortável com listas e validação de JSON.

---

## 15. Comparação final

| Técnica/Padrão | Melhor uso | Esforço | Entregável | Limitação |
|---|---|---|---|---|
| Lista simples de alarmes | poucos alarmes e fluxo pequeno | baixo | vetor/lista no controlador | cresce mal quando há várias reações |
| `Observer` | vários destinos para o mesmo evento | médio | sujeito + observadores | exige organizar a lista de observadores |
| `Repository` de eventos | histórico consultável | médio | tabela ou arquivo de eventos | depende de contrato estável |
| Streamlit direto | visualização | baixo | tabela de alarmes | não deve concentrar regra de alarme |

Recomendação prática:

- use lista simples se houver apenas uma reação;
- use `Observer` se o evento precisa gerar alarme, log e persistência;
- use `Repository` para salvar eventos ou alarmes;
- mantenha o Streamlit como camada de exibição.

---

## 16. Mini-caso prático

Durante um ciclo, `PT-201` mede `7.2 bar`. O limite seguro é `6.0 bar`.

Sem `Observer`, a estação poderia:

1. criar o alarme;
2. salvar no banco;
3. imprimir mensagem;
4. atualizar a lista do supervisor.

Com `Observer`, a estação apenas publica:

```text
PRESSAO_ALTA em PT-201
```

Os observadores reagem:

- `GerenciadorAlarmes` guarda o alarme;
- `LoggerConsole` imprime a ocorrência;
- `RegistradorEventos` salva o histórico.

---

## 17. Exercícios práticos

1. Abrir o exemplo `padroes_ii_observer`.
2. Executar primeiro o contraexemplo em `sem_observer/`.
3. Identificar o arquivo que define o evento.
4. Identificar o arquivo que define a interface dos observadores.
5. Explicar por que `EstacaoBombeamento` não precisa conhecer `LoggerConsole` em detalhes.
6. Criar um novo observador chamado `ContadorEventos`.
7. Documentar no README do projeto final quais arquivos representam o `Observer`.

---

## 18. Checklist de entrega

- [ ] O README diferencia leitura, evento e alarme.
- [ ] Existe estrutura para evento operacional.
- [ ] Existe pelo menos um observador.
- [ ] A estação publica eventos sem conhecer a tela.
- [ ] Existem pelo menos três tipos de alarme.
- [ ] O supervisor mostra alarmes de forma tabular.
- [ ] A equipe consegue explicar o ganho do `Observer`.

---

## 19. Perguntas de revisão rápida

1. Qual é a diferença entre leitura, evento e alarme?
2. Por que a estação não deve salvar banco, imprimir log e atualizar tela ao mesmo tempo?
3. Como a organização em arquivos ajuda a explicar o `Observer` na defesa técnica?

---

## Fontes de referência

- [Repositório de exemplos do curso](https://github.com/rafaelrezo/curso-poo-exemplos)
- [Refactoring.Guru - Padrões de Projeto em português](https://refactoring.guru/pt-br/design-patterns)
- [C++ Core Guidelines - Interfaces](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Ri-abstract)
- [Python Docs - dataclasses](https://docs.python.org/3/library/dataclasses.html)
- [Streamlit Docs - Data display elements](https://docs.streamlit.io/develop/api-reference/data)
- [Refactoring Guru - Observer](https://refactoring.guru/design-patterns/observer)
