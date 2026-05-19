# Parte 2 - Projeto Integrador: Estação de Bombeamento Inteligente

## Objetivos de aprendizagem

- Integrar os conceitos de POO estudados na primeira parte em um projeto único e progressivo.
- Separar responsabilidades entre um dispositivo simulado em C++ e um supervisor em Python/Streamlit.
- Evoluir uma solução simples com JSON para persistência, comunicação TCP, testes e padrões de projeto.

**Tempo estimado:** 15h, organizadas em 10 aulas de 1h30.

## Vídeo de contexto

![type:video](https://www.youtube.com/embed/ZFDSPSU6JMw)

---

## 1. Visão geral da segunda parte

Nesta etapa, você construirá um projeto progressivo chamado **Estação de Bombeamento Inteligente em duas camadas**.

O sistema será dividido em duas partes:

- **dispositivo C++:** simula sensores, bomba, regras locais e geração de leituras;
- **supervisor Python/Streamlit:** recebe os dados, apresenta tabela, gráfico, resumo, alarmes e histórico.

A automação industrial aparece como contexto. O objetivo principal continua sendo Programação Orientada a Objetos: classes, objetos, encapsulamento, herança, polimorfismo, composição, associação, tratamento de exceções, coleções, testes e organização de código.

---

## 2. Contrato JSON inicial

O primeiro contrato entre o dispositivo e o supervisor será um objeto JSON simples:

```json
{
  "tag": "LT-101",
  "valor": 58.4,
  "unidade": "%",
  "timestamp": "2026-03-25T10:30:00-03:00",
  "status": "operando"
}
```

| Campo | Significado | Exemplo | Observação |
|---|---|---|---|
| `tag` | identificador do dispositivo | `LT-101` | deve ser único no sistema |
| `valor` | leitura simulada | `58.4` | usa ponto decimal |
| `unidade` | unidade de grandeza | `%` | ajuda a interpretar o valor |
| `timestamp` | instante da leitura | `2026-03-25T10:30:00-03:00` | formato textual ISO 8601 |
| `status` | estado operacional | `operando` | usado pelo supervisor e pelas regras |

### Ponte C++ -> Python

| Responsabilidade | C++ | Python/Streamlit |
|---|---|---|
| Simular leitura | classes de sensores | apenas consome e visualiza |
| Gerar JSON | serialização controlada | leitura com `json.loads()` |
| Aplicar regras | classes de controle | resumo e diagnóstico |
| Registrar histórico | CSV e SQLite | tabela, gráfico e filtros |

---

## 3. Sequência das aulas

| Aula | Tema | Entregável principal |
|---|---|---|
| 1 | Cenário e modelagem | diagrama inicial, contrato JSON e esqueleto de repositório |
| 2 | Aula integrada: listas dinâmicas em C++ e Python | `vector<unique_ptr<Sensor>>`, `push_back`, `const auto&`, `list` e `append` |
| 3 | Aula integrada: objetos da lista com estado protegido | validação de leituras e organização dos dados recebidos |
| 4 | Aula integrada: atividade prática | exercício único com sensores, lista de objetos e supervisor |
| 5 | Strategy e Command | regras e comandos intercambiáveis |
| 6 | Observer e alarmes | notificações desacopladas |
| 7 | Persistência | histórico em CSV e evolução para SQLite |
| 8 | Comunicação TCP | JSON por linha em cliente-servidor simples |
| 9 | Testes e validação | testes de contrato, regras e integração |
| 10 | Projeto final | mini-SCADA e defesa arquitetural |

---

## 4. Estrutura esperada do repositório

Uma organização simples e suficiente para o projeto:

```text
estacao-bombeamento/
  dispositivo_cpp/
    src/
    include/
    tests/
  supervisor_python/
    app.py
    requirements.txt
    tests/
  dados/
    leituras.csv
    comandos.csv
    alarmes.csv
  .github/
    ISSUE_TEMPLATE/
    pull_request_template.md
  README.md
  AI_LOG.md
```

### Arquivos obrigatórios desde o início

- `README.md`: explica o projeto, o contrato JSON e como executar.
- `AI_LOG.md`: registra uso de agentes de IA.
- `.github/ISSUE_TEMPLATE/atividade.md`: descreve problemas, tarefas e critérios.
- `.github/pull_request_template.md`: orienta revisão, testes e rastreabilidade.

---

## 5. Mini-caso prático

A estação possui um reservatório, uma bomba e sensores. O objetivo inicial é manter o nível dentro de uma faixa segura.

Uma regra inicial:

- se o nível estiver baixo, a bomba deve ligar;
- se o nível estiver alto, a bomba deve desligar;
- se a pressão estiver muito alta, a partida deve ser bloqueada.

Esse problema é pequeno, mas permite discutir várias decisões de POO:

- `Sensor` deve ser uma classe concreta ou abstrata?
- `SensorNivel` e `SensorPressao` devem herdar de `Sensor`?
- Como guardar vários sensores diferentes em uma lista dinâmica?
- `EstacaoBombeamento` deve conter uma `Bomba`?
- Quem conhece as regras: a bomba, o sensor, o supervisor ou um controlador?
- O supervisor deve alterar o estado interno do dispositivo diretamente?

---

## 6. Exercícios práticos iniciais

1. Criar um repositório local com a estrutura sugerida.
2. Escrever um `README.md` com o contrato JSON inicial.
3. Criar um `AI_LOG.md` com a primeira entrada, mesmo que a equipe ainda não use IA.
4. Criar os templates de issue e pull request.
5. Desenhar um diagrama de classes inicial com `Sensor`, `Bomba`, `Reservatorio`, `Supervisor` e `EstacaoBombeamento`.

---

## 7. Checklist de entrega da parte 2

- [ ] O projeto tem dispositivo C++ e supervisor Python/Streamlit.
- [ ] O contrato JSON está documentado no `README.md`.
- [ ] A assinatura operacional da dupla está documentada.
- [ ] O projeto final possui pelo menos 4 tipos de sensores.
- [ ] O projeto final possui pelo menos 2 bombas ou 1 bomba com 1 atuador auxiliar.
- [ ] O projeto possui `AI_LOG.md` preenchido.
- [ ] Existem templates de issue e pull request.
- [ ] O código usa herança, polimorfismo e encapsulamento de forma explícita.
- [ ] O projeto usa pelo menos dois padrões de projeto.
- [ ] O supervisor apresenta tabela, gráfico e resumo.
- [ ] O histórico é salvo em CSV e em SQLite.
- [ ] Há testes para regras de negócio e contrato JSON.
- [ ] Há testes cobrindo parâmetros específicos da dupla.
- [ ] A equipe apresenta uma defesa técnica final.

---

## 8. Perguntas de revisão rápida

1. Por que separar o dispositivo C++ do supervisor Python?
2. O que transforma um JSON válido em um contrato útil para integração?
3. Qual é o risco de deixar o supervisor alterar diretamente atributos internos do dispositivo?

---

## 9. Desafios opcionais

- Adicionar uma segunda bomba e discutir alternância de bombas.
- Criar um modo de simulação com comportamento normal e outro com falhas.
- Adicionar um campo `qualidade` ao JSON para indicar se a leitura é confiável.

---

## Fontes de referência

- [cppreference - C++ language](https://en.cppreference.com/w/cpp/language)
- [Python Docs - json](https://docs.python.org/3/library/json.html)
- [Streamlit Docs](https://docs.streamlit.io/)
- [SQLite Docs](https://www.sqlite.org/docs.html)
- [GitHub Docs - About pull requests](https://docs.github.com/en/pull-requests)
