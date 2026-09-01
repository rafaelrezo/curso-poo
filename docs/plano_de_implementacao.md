# Plano de Implementação do Curso (Repositorio + Trilha)

## Objetivos de aprendizagem

- Implantar uma trilha de POO em MkDocs com publicacao no GitHub Pages.
- Operar atividades por repositórios-base públicos, fork, branch, testes, CI e PR no próprio fork.
- Garantir consistencia pedagogica para C++ e Python.

**Tempo estimado:** 4 semanas de implantacao inicial

## Vídeo de contexto

![type:video](https://www.youtube.com/embed/8hly31xKli0)

---

## 1. Escopo da implantacao

- Estruturar o repositório como hub de conteúdo publico.
- Publicar o material no GitHub Pages.
- Preparar repositórios-base públicos na namespace `rafaelrezo`.
- Definir politica de uso de IA em código e relatorios.
- Manter cada cenário prático em um repositório-base público independente do Pages.

## 2. Fases e entregáveis

| Fase | Prazo sugerido | Entregável | Critério de aceite |
|---|---|---|---|
| F1 - Base editorial | Semana 1 | `AGENTS.md` atualizado + estrutura `docs/` | Padrão de página aplicado e navegacao funcional |
| F2 - Conteudo nucleo | Semanas 1-2 | Modulos de POO (C++ e Python) | Cada modulo com caso prático, perguntas e referências |
| F3 - Operação de turma | Semana 2 | Guias de fork/Actions/PR + rubricas + repositórios-base | Fluxo de entrega e avaliação reproduzível |
| F4 - Projeto integrador | Semanas 3-4 | Enunciado, milestones e checkpoints | Equipes conseguem iniciar sem bloqueio técnico |
| F5 - Publicacao | Semana 4 | GitHub Pages ativo por Actions | Site acessivel publicamente e atualizado por push |

## 3. Cronograma pedagogico (60h)

| Bloco | Horas | Foco |
|---|---:|---|
| Fundamentos operacionais | 8h | Git, IDE, fork, GitHub Actions, PR e boas práticas |
| POO C++ + Python | 27h | Conceitos essenciais, modelagem e análise de código |
| Avaliações | 4h | Checkpoints de compreensao e refatoracao |
| Projeto final | 21h | Solucao integradora com backend, frontend e dados |

## 4. Implementação técnica no GitHub

1. Repositorio de conteúdo: este repositório (publico).
2. Publicacao: GitHub Pages via GitHub Actions.
3. Repositórios de atividade: starters públicos separados na namespace `rafaelrezo`.
4. Organização da turma: Google Classroom apenas para comunicação, prazos e coleta do link da PR quando necessário.
5. Material de atividade: README, checkpoints, testes, workflow, PR e `AI_LOG.md` no fork.

## 5. Padrão mínimo para cada aula em `docs/`

- Objetivos de aprendizagem (3 bullets).
- Vídeo de contexto.
- Conteudo numerado.
- Ponte C++ -> Python.
- Mini-caso prático.
- 3 perguntas de revisão rápida.
- Referências oficiais.

## Mini-caso prático

Implantar o modulo "Encapsulamento" com uma classe de controle de reservatório. A versão C++ simula leitura local e a versão Python oferece dashboard e persistência simples.

## Perguntas de revisão rápida

1. Quais entregáveis tornam o curso operacional nas primeiras 2 semanas?
2. Qual dependencia critica para publicar conteúdo aos alunos?
3. Como separar repositório de conteúdo e repositórios de entrega?

## Fontes de referência

- https://docs.github.com/en/pages
- https://docs.github.com/en/education/manage-coursework-with-github-classroom
- https://www.mkdocs.org/
