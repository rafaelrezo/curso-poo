# Plano de Implementacao do Curso (Repositorio + Trilha)

## Objetivos de aprendizagem

- Implantar uma trilha de POO em MkDocs com publicacao no GitHub Pages.
- Operar atividades via GitHub Classroom com fluxo de mercado.
- Garantir consistencia pedagogica para C++ e Python.

**Tempo estimado:** 4 semanas de implantacao inicial

## Video de contexto

![type:video](https://www.youtube.com/embed/8hly31xKli0)

---

## 1. Escopo da implantacao

- Estruturar o repositorio como hub de conteudo publico.
- Publicar o material no GitHub Pages.
- Preparar templates de atividade para GitHub Classroom.
- Definir politica de uso de IA em codigo e relatorios.

## 2. Fases e entregaveis

| Fase | Prazo sugerido | Entregavel | Criterio de aceite |
|---|---|---|---|
| F1 - Base editorial | Semana 1 | `AGENTS.md` atualizado + estrutura `docs/` | Padrao de pagina aplicado e navegacao funcional |
| F2 - Conteudo nucleo | Semanas 1-2 | Modulos de POO (C++ e Python) | Cada modulo com caso pratico, perguntas e referencias |
| F3 - Operacao de turma | Semana 2 | Guias de Git/GitHub Classroom + rubricas | Fluxo de entrega e avaliacao reproduzivel |
| F4 - Projeto integrador | Semanas 3-4 | Enunciado, milestones e checkpoints | Equipes conseguem iniciar sem bloqueio tecnico |
| F5 - Publicacao | Semana 4 | GitHub Pages ativo por Actions | Site acessivel publicamente e atualizado por push |

## 3. Cronograma pedagogico (60h)

| Bloco | Horas | Foco |
|---|---:|---|
| Fundamentos operacionais | 8h | Git, IDE, GitHub Classroom, boas praticas |
| POO C++ + Python | 27h | Conceitos essenciais, modelagem e analise de codigo |
| Avaliacoes | 4h | Checkpoints de compreensao e refatoracao |
| Projeto final | 21h | Solucao integradora com backend, frontend e dados |

## 4. Implementacao tecnica no GitHub

1. Repositorio de conteudo: este repositorio (publico).
2. Publicacao: GitHub Pages via GitHub Actions.
3. Repositorios de atividade: criados pelo GitHub Classroom.
4. Organizacao da turma: importar roster do Google Classroom (fase de transicao) ou operar apenas no GitHub Classroom.

## 5. Padrao minimo para cada aula em `docs/`

- Objetivos de aprendizagem (3 bullets).
- Video de contexto.
- Conteudo numerado.
- Ponte C++ -> Python.
- Mini-caso pratico.
- 3 perguntas de revisao rapida.
- Referencias oficiais.

## Mini-caso pratico

Implantar o modulo "Encapsulamento" com uma classe de controle de reservatorio. A versao C++ simula leitura local e a versao Python oferece dashboard e persistencia simples.

## Perguntas de revisao rapida

1. Quais entregaveis tornam o curso operacional nas primeiras 2 semanas?
2. Qual dependencia critica para publicar conteudo aos alunos?
3. Como separar repositorio de conteudo e repositorios de entrega?

## Fontes de referencia

- https://docs.github.com/en/pages
- https://docs.github.com/en/education/manage-coursework-with-github-classroom
- https://www.mkdocs.org/
