# Parte 2 — Projeto Integrador: controle e supervisão

## Objetivos de aprendizagem

- Integrar os building blocks da Parte 1 em uma arquitetura evolutiva de controle e supervisão.
- Aplicar padrões de projeto, persistência, comunicação e estratégias de qualidade a problemas observáveis.
- Entregar incrementos por branch e pull request, com testes locais, CI e justificativa técnica.

**Tempo estimado:** 21h, com encontros de 2h e um checkpoint final.

## Vídeo de contexto

![type:video](https://www.youtube.com/embed/rDidOn6KN9k)

---

## 1. De onde partimos?

A Parte 2 não começa do zero. Ao concluir a Parte 1, a equipe já deve possuir:

- classes de sensores e atuador com estado válido;
- controlador composto por objetos com responsabilidades distintas;
- contrato comum para diferentes sensores;
- coleção dinâmica com posse segura em C++ e lista em Python;
- produtor C++ e consumidor Python ligados por JSONL;
- timestamp UTC e distinção entre estado de falha e entrada inválida;
- tratamento básico de exceções nas fronteiras;
- testes cumulativos e workflow de CI.

Se esse baseline não estiver verde, a primeira ação é recuperar o checkpoint do capítulo 09. A Parte 2 acrescentará complexidade arquitetural, não lacunas de fundamentos.

---

## 2. Visão do sistema

```text
Controlador virtual C++
  sensores -> regras de controle -> atuadores
       |                  ^
       +---- eventos -----+
              |
          contrato JSON
              |
Supervisório Python
  alarmes -> persistência -> visualização -> comandos
```

O sistema continua sendo didático. Ele simula responsabilidades de controle e supervisão, mas não possui garantias de tempo real, segurança funcional, I/O industrial ou protocolos de um CLP/SCADA real.

---

## 3. Sequência reorganizada

| Etapa | Problema novo | Conceito/tecnologia | Evidência principal |
|---:|---|---|---|
| 1 | componentes funcionam isolados | arquitetura, responsabilidades e contrato versionado | diagrama e baseline integrado |
| 2 | regras de controle variam | `Strategy` | algoritmo substituível sem alterar o controlador |
| 3 | comandos precisam ser representados | `Command` | comando testável e rastreável |
| 4 | vários interessados reagem a eventos | `Observer`, eventos e alarmes | alarme emitido sem acoplamento direto |
| 5 | histórico desaparece ao encerrar | SQLite e `Repository` | leituras persistidas e consultadas |
| 6 | arquivo não representa integração contínua | comunicação TCP e contrato | produtor e consumidor em processos distintos |
| 7 | falhas atravessam várias camadas | recuperação, logging e timeout | falha observável sem corrupção do estado |
| 8 | mudanças podem quebrar contratos | testes unitários, contrato e integração | matriz de testes verde |
| 9 | validação manual não escala | análise estática, cobertura e CI | pipeline com feedback por commit |
| 10 | decisões precisam ser defendidas | projeto final e revisão técnica | PR final, evidências e defesa oral |

Listas dinâmicas e introdução a exceções deixaram de ser capítulos desta parte: agora são pré-requisitos produzidos na Parte 1. Exceções reaparecem aqui como decisão arquitetural de resiliência.

---

## 4. Estrutura que emerge gradualmente

```text
projeto-integrador/
├── controller_cpp/
│   ├── include/
│   ├── src/
│   └── tests/
├── supervisor_py/
│   ├── app/
│   └── tests/
├── contracts/
├── data/
├── docs/diagramas/
├── .github/workflows/ci.yml
├── README.md
└── AI_LOG.md
```

Não crie toda a árvore no primeiro encontro. Cada pasta deve aparecer quando uma responsabilidade concreta exigir sua existência.

---

## 5. Regra de progressão

Cada etapa segue o mesmo ciclo:

1. executar o baseline verde;
2. observar uma limitação do sistema;
3. registrar a necessidade em issue;
4. criar branch curta com nome fornecido;
5. adicionar um teste que expõe a limitação;
6. implementar o menor incremento útil;
7. fazer push e interpretar o CI;
8. abrir PR com diagrama ou decisão atualizada;
9. integrar somente com evidência de funcionamento.

Testes anteriores permanecem ativos. Esse caráter cumulativo transforma regressões em feedback formativo.

---

## 6. Camadas de aprendizagem

| Camada | Pergunta | Evidência |
|---|---|---|
| conceito | por que o padrão ou técnica existe? | explicação do problema e da regra |
| implementação | como aparece no código? | incremento compilável e legível |
| engenharia | como integrar com segurança? | testes, CI, histórico e revisão |

Não aceite uma automação verde sem explicação do modelo. Também não aceite uma explicação conceitual sem comportamento reproduzível.

---

## 7. Mini-caso prático

A equipe recebe o controlador que aciona uma bomba com limites fixos. A primeira mudança da Parte 2 será permitir outras estratégias de controle sem editar a classe central. O problema introduz `Strategy`; o padrão não aparece como catálogo antecipado.

---

## Perguntas de revisão rápida

1. Quais fundamentos precisam estar verdes antes de iniciar a Parte 2?
2. Por que exceções continuam presentes mesmo sem um capítulo introdutório nesta parte?
3. O que diferencia um teste unitário de um teste do contrato C++ → Python?

## Fontes de referência

- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
- [Python Docs](https://docs.python.org/3/)
- [SQLite Docs](https://www.sqlite.org/docs.html)
- [GitHub Actions Docs](https://docs.github.com/en/actions)
- [Streamlit Docs](https://docs.streamlit.io/)
