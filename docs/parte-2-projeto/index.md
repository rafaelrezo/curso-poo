# Parte 2 — Projeto Integrador: controle e supervisão

## Objetivos de aprendizagem

- Integrar os objetos e contratos construídos na Parte 1 em um sistema de engenharia.
- Justificar arquitetura, padrões, persistência e comunicação a partir de requisitos.
- Entregar incrementos em equipe com testes, revisão e defesa técnica.

**Tempo estimado:** proposta de 22h em sala (2h iniciais de princípios/testes e 20h de projeto), dentro das 60h do curso; conferir a [reserva provisória de horas](../index.md#3-organizacao-das-60h). Os capítulos são recursos dos marcos, não encontros adicionais.

## Vídeo de contexto

![type:video](https://www.youtube.com/embed/rDidOn6KN9k)

---

## 1. De onde partimos?

A Parte 1 termina com [UML e modelagem](../modelagem_analise_codigo/index.md). Associação, interfaces, exceções, igualdade, coleções genéricas já foram estudados. Os testes fornecidos foram executados; agora começamos a escrever testes próprios e revisar responsabilidades. Aqui a equipe aplica esses fundamentos em um controlador C++ e um supervisório Python.

Reutilize o fork do [starter de fundamentos](https://github.com/rafaelrezo/poo-fundamentos-estacao), com UML12 concluída e as etapas técnicas até 13 integradas. Execute `make test ETAPA=13` e confira o diagrama antes de iniciar o projeto. O starter público contém comportamentos incompletos intencionais; cloná-lo novamente não substitui as implementações da Parte 1.

Comece pelo [capítulo 01 — Princípios de Projeto e Testes de Objetos](00-principios-testes/index.md), na branch `projeto/00-testes`. Execute `make test-projeto`, complete o controlador e escreva seus testes. O alvo valida a etapa técnica14; ele inicialmente falha porque essa implementação ainda não foi feita. Integre esse primeiro PR antes de abrir a arquitetura.

## 2. Marcos de projeto

| Marco e branch | Horas em sala | Recursos de consulta | Evidência |
|---|---:|---|---|
| Abertura — `projeto/00-testes` | 2h | princípios de projeto e testes | controlador e testes autorais, com UML atualizada |
| 1 — `projeto/01-arquitetura` | 2h | cenário e arquitetura | escopo, responsabilidades, UML e critérios de aceite |
| 2 — `projeto/02-integracao` | 4h | JSON e integração resiliente | produtor C++, consumidor Python e testes de contrato |
| 3 — `projeto/03-regras` | 4h | Strategy, Command e Observer | regra substituível e alarme desacoplado |
| 4 — `projeto/04-persistencia` | 4h | banco de dados e Repository | histórico consultável e testes isolados |
| 5 — `projeto/05-comunicacao` | 4h | TCP, qualidade e testes de integração | comunicação entre processos e falhas verificadas |
| 6 — `projeto/06-entrega` | 2h | CI e projeto final | demonstração, evidências e defesa das decisões |

A página antiga de coleções encaminha para a seção 11 da Parte 1. Os capítulos de testes e CI apoiam todos os marcos desde o primeiro; não se espera o fim do projeto para testar.

## 3. Mini-caso prático: estação de monitoramento

Uma estação consulta fontes de nível e temperatura. O controlador aplica uma política a uma leitura; o supervisório recebe telemetria, apresenta alarmes e consulta o histórico. A equipe começa pelo caminho completo de uma leitura e amplia o sistema quando esse caminho estiver validado.

```text
Fontes -> controlador C++ -> contrato JSONL -> supervisório Python
                 |                                |
          política de alarme                histórico e tela
```

C++ e Python compartilham o significado dos campos e das falhas. Eles não precisam compartilhar classes ou detalhes de memória. Trata-se de uma simulação didática de controle e supervisão.

## 4. Como executar cada incremento

Depois de integrar a abertura de testes, no fork concluído, atualize a main, crie a branch do marco e execute o baseline:

```bash
git switch main
git pull --ff-only origin main
git switch -c projeto/01-arquitetura
make test-projeto
```

O alvo `test-projeto` inicialmente repete os contratos da etapa 14. Em cada marco, acrescente ao alvo os testes do novo comportamento. A CI já executa esse mesmo comando em pushes de branches `projeto/**`; um baseline verde não comprova que uma integração nova foi testada.

Faça commits pequenos, push para `origin` e PR para a `main` do próprio fork. Mantenha somente `origin`; não configure `upstream` nem abra PR no repositório do docente. Em equipe, registre os papéis e as contribuições; preserve o histórico anterior ao organizar pastas.

## 5. Como confirmar e integrar

Cada PR inclui requisito, teste que evidencia o problema, resultado local e link da CI do commit, explicação do diff, decisão de projeto e atualização de UML quando pertinente. Registre o uso de IA e justifique o que foi aceito ou rejeitado. Integre após revisão; a defesa oral confirma compreensão além da automação.

## Perguntas de revisão rápida

1. Quais fundamentos permitem começar o projeto sem novas aulas introdutórias de coleções e exceções?
2. Que teste comprova a comunicação entre o produtor e o consumidor?
3. Por que passar os testes da Parte 1 não basta para aceitar um marco novo?

## Fontes de referência

- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
- [Python — JSON](https://docs.python.org/3/library/json.html)
- [SQLite](https://www.sqlite.org/docs.html)
- [GitHub Actions](https://docs.github.com/en/actions)
