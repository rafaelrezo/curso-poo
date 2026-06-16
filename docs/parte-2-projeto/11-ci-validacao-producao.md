# Aula 11 - Execução Local, CI e Validação em Ambiente Real

## Objetivos de aprendizagem

- Executar localmente a mesma sequência de testes usada no pipeline.
- Configurar um workflow básico de GitHub Actions para C++ e Python.
- Entender como testes, smoke checks, logs e aprovação humana entram em um fluxo produtivo real.

**Tempo estimado:** 1h30.

## Vídeo da aula

![type:video](https://www.youtube.com/embed/ugrB9hNT9qw)

---

## 1. Do computador do aluno ao ambiente real

Testes não existem só para a máquina local. Em um fluxo profissional, a mesma verificação aparece em etapas diferentes.

| Etapa | Onde roda | Objetivo |
|---|---|---|
| Desenvolvimento local | máquina do aluno | feedback rápido antes do commit |
| Pull request | GitHub Actions | proteger a branch principal |
| Pré-entrega | máquina da equipe | reproduzir demonstração completa |
| Publicação | ambiente de hospedagem | disponibilizar versão aprovada |
| Pós-publicação | ambiente real | validar que o sistema subiu e continua saudável |

No curso, a equipe não precisa simular uma indústria inteira. Precisa demonstrar maturidade: comandos reproduzíveis, CI passando, evidência de validação manual e explicação clara do que cada teste cobre.

---

## 2. Checklist local antes do commit

Antes de fazer commit:

```bash
cd dispositivo_cpp
mkdir -p build
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude tests/test_controle.cpp -o build/test_controle
./build/test_controle
```

```bash
cd supervisor_python
python3 -m pytest -q
```

Antes de abrir pull request:

- [ ] testes C++ passaram;
- [ ] testes Python passaram;
- [ ] Streamlit abriu localmente;
- [ ] dados de exemplo foram carregados;
- [ ] README explica como reproduzir;
- [ ] `AI_LOG.md` registra ajuda de agente, se houve;
- [ ] PR descreve mudança e evidências.

---

## 3. O que é CI

CI, integração contínua, é a prática de integrar mudanças frequentemente e verificar automaticamente se o projeto ainda compila e passa nos testes.

No GitHub Actions:

- um workflow é um arquivo YAML em `.github/workflows/`;
- um evento, como `push` ou `pull_request`, dispara a execução;
- um job roda em um runner;
- steps executam comandos;
- se qualquer step falhar, o workflow falha.

Impacto prático: a equipe deixa de depender apenas de "funcionou na minha máquina".

---

## 4. Workflow mínimo para o projeto

Arquivo `.github/workflows/testes.yml`:

```yaml
name: Testes

on:
  push:
  pull_request:

jobs:
  cpp:
    name: C++ - controlador
    runs-on: ubuntu-latest

    steps:
      - name: Baixar código
        uses: actions/checkout@v6

      - name: Compilar testes C++
        working-directory: dispositivo_cpp
        run: |
          mkdir -p build
          g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude tests/test_controle.cpp -o build/test_controle

      - name: Executar testes C++
        working-directory: dispositivo_cpp
        run: ./build/test_controle

  python:
    name: Python - supervisor
    runs-on: ubuntu-latest

    steps:
      - name: Baixar código
        uses: actions/checkout@v6

      - name: Configurar Python
        uses: actions/setup-python@v6
        with:
          python-version: "3.12"

      - name: Instalar dependências
        working-directory: supervisor_python
        run: |
          python -m pip install --upgrade pip
          python -m pip install -r requirements.txt

      - name: Executar testes Python
        working-directory: supervisor_python
        run: python -m pytest -q
```

Se o repositório da equipe tiver outra estrutura, ajuste apenas `working-directory` e nomes de arquivos.

!!! tip "CI executando em um repositório real"

    O exemplo público [`testes_ci_estacao`](https://github.com/rafaelrezo/curso-poo-exemplos/tree/main/testes_ci_estacao) é acompanhado por um workflow executável em [`.github/workflows/testes-ci-estacao.yml`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/.github/workflows/testes-ci-estacao.yml). Depois de publicado no GitHub, a aba **Actions** permite ver cada execução dos jobs C++ e Python.

---

## 5. CI não é produção

CI responde: "o código passou nos testes em um ambiente limpo?".

Produção responde: "a versão publicada funciona com configuração, dados e restrições reais?".

| Verificação | CI | Ambiente real/produtivo |
|---|---|---|
| Compilação C++ | sim | normalmente não |
| Testes unitários | sim | normalmente não |
| Testes de contrato | sim | sim, antes de publicar |
| Migração/criação de banco | pode simular | precisa validar com cuidado |
| Smoke test | opcional | essencial |
| Logs e erros | limitado | essencial |
| Rollback | não é foco | plano necessário em sistemas reais |

No curso, "ambiente produtivo real" deve ser tratado como cultura de engenharia: publicar somente depois de passar nos testes, validar o básico após subir e saber observar falhas.

---

## 6. Smoke test depois de publicar

Smoke test é uma verificação curta para descobrir se a aplicação subiu minimamente.

Checklist para o mini-SCADA:

- [ ] aplicação abre sem exceção;
- [ ] banco ou arquivo de dados é encontrado;
- [ ] tabela principal aparece;
- [ ] gráfico carrega com dados de exemplo;
- [ ] lista de alarmes aparece;
- [ ] uma leitura inválida não derruba a tela;
- [ ] reiniciar o app não apaga histórico obrigatório.

Esse checklist deve ser curto. Se ele vira uma prova completa, deixou de ser smoke test.

---

## 7. Observabilidade mínima

Depois da publicação, testes já não enxergam tudo. O sistema precisa deixar rastros.

| Necessidade | Evidência simples no curso | Equivalente em produção real |
|---|---|---|
| saber se o app abriu | print ou log inicial | health check |
| saber erro de leitura | mensagem controlada | log estruturado |
| saber volume de dados | contador de leituras | métrica |
| saber falha crítica | histórico de alarmes | alerta |
| saber versão entregue | commit no README/PR | release/tag |

Ação recomendada: registrar no README o commit apresentado e incluir uma seção "Como validar a entrega".

---

## 8. Dados de teste, dados reais e segurança

Dados de teste devem ser previsíveis e versionáveis. Dados reais podem ter ruído, volume maior e informações sensíveis.

| Tipo de dado | Melhor uso | Pode versionar? |
|---|---|---|
| `leituras_exemplo.jsonl` | contrato e integração | sim |
| SQLite temporário | testes de Repository | não precisa |
| banco da demonstração | roteiro final | depende do conteúdo |
| dados reais de operação | análise e produção | somente com cuidado e autorização |

Regra prática: teste automatizado deve criar seus próprios dados ou usar arquivos pequenos de exemplo. Não dependa de banco manual do dia da apresentação.

---

## 9. Pull request como barreira de qualidade

Um PR do projeto final deve conter:

- resumo da mudança;
- quais testes foram executados;
- evidência do resultado;
- impacto na arquitetura;
- atualização de README quando comandos mudarem;
- rastreabilidade de IA no `AI_LOG.md`, quando aplicável.

Exemplo de bloco no PR:

```markdown
## Testes executados

- [x] `./build/test_controle`
- [x] `python3 -m pytest -q`
- [x] validação manual do Streamlit com `dados/leituras_exemplo.jsonl`

## Riscos

- O teste de tela ainda é apenas smoke test.
- Não há teste de carga; fora do escopo desta entrega.
```

---

## 10. Matriz final de testes

Cada equipe deve manter uma matriz simples no README.

| Item testado | Tipo de teste | Local | CI | Evidência |
|---|---|---|---|---|
| regra de nível baixo | unitário | sim | sim | `test_controle` |
| regra de pressão alta | unitário | sim | sim | `test_controle` |
| comando de bloqueio | unitário | sim | sim | `test_controle` |
| contrato JSON | contrato | sim | sim | `pytest` |
| alarme crítico | unitário | sim | sim | `pytest` |
| persistência SQLite | integração | sim | sim | `pytest` com `tmp_path` |
| fluxo C++ -> Python | integração | sim | opcional | JSON Lines validado |
| tela Streamlit | fumaça/manual | sim | opcional | checklist |
| publicação/demonstração | aceitação | sim | não | roteiro final |

---

## 11. Tópico relacionado que faltava

Além de testes, o curso precisa fechar a parte 2 com uma ponte para operação: CI, smoke test, logs, matriz de evidências e PR como barreira de qualidade. Esses tópicos não são "mais POO", mas mostram por que encapsulamento, padrões e testes importam em um fluxo real de entrega.

Recomendação: não criar uma aula separada de DevOps completa. Para a carga do curso, basta esta aula curta conectando testes com integração contínua e validação operacional.

---

## 12. Mini-caso prático

A equipe publicou o mini-SCADA em um ambiente compartilhado para demonstração. O CI passou, mas a tela abriu sem gráfico.

Diagnóstico orientado por testes:

1. teste unitário de regra passou: o problema não está na decisão da bomba;
2. teste de contrato passou: o JSON básico está correto;
3. teste de Repository passou: o dado foi salvo;
4. smoke test falhou: a tela não encontrou a coluna esperada;
5. correção: padronizar nome de coluna entre Repository e Streamlit;
6. regressão: criar teste que confirma a presença dos campos usados no gráfico.

---

## 13. Exercícios práticos

1. Criar `.github/workflows/testes.yml` no repositório da equipe.
2. Fazer o workflow rodar testes C++ e Python.
3. Adicionar a matriz de testes ao README.
4. Criar checklist de smoke test do Streamlit.
5. Abrir um pull request com evidência de execução.
6. Registrar no `AI_LOG.md` qualquer uso de agente na criação dos testes.

---

## 14. Perguntas de revisão rápida

1. Qual é a diferença entre CI e ambiente produtivo?
2. Por que o pipeline deve rodar comandos parecidos com os comandos locais?
3. O que um smoke test deve verificar depois da publicação?

---

## Fontes de referência

- [GitHub Docs - Guia Rápido do GitHub Actions](https://docs.github.com/pt/actions/get-started/quickstart)
- [GitHub Docs - Criando um exemplo de fluxo de trabalho](https://docs.github.com/pt/actions/tutorials/create-an-example-workflow)
- [GitHub Docs - Compilar e testar Python](https://docs.github.com/pt/actions/tutorials/build-and-test-code/python)
- [GitHub Docs - Workflow syntax for GitHub Actions](https://docs.github.com/actions/using-workflows/workflow-syntax-for-github-actions)
- [Streamlit Docs - Automate your tests with CI](https://docs.streamlit.io/develop/concepts/app-testing/automate-tests)
