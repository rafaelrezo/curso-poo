# GitHub Classroom e GitHub Pages

## Objetivos de aprendizagem

- Configurar turma e atividades no GitHub Classroom.
- Organizar repositório de conteúdo e repositórios de entrega.
- Publicar o material de aula no GitHub Pages.

**Tempo estimado:** 2h (docente/monitoria)

## Vídeo de contexto

![type:video](https://www.youtube.com/embed/N6AYLOgYec8)

---

## 1. Arquitetura recomendada

- Este repositório: conteúdo do curso (MkDocs + Pages).
- Repositorios de atividade: criados pelo GitHub Classroom por aluno/equipe.
- `Google Classroom`: comunicação da turma, avisos e rotinas administrativas.
- Cada cenário prático vira um repositório starter independente no GitHub Classroom.

**Recomendação de visibilidade**

- Conteudo de aula: publico (facilita acesso sem login).
- Entregas de atividade: separadas por assignment no GitHub Classroom.
- Observação importante: o site publicado no GitHub Pages e publicamente acessivel.

| Canal | Uso principal | Visibilidade |
|---|---|---|
| GitHub Pages | conteúdo da disciplina | pública |
| GitHub Classroom | atividades, cards, PRs e avaliação | restrita a turma |
| Google Classroom | comunicação e avisos | restrita a turma |

## 2. Fluxo GitHub Classroom

1. Criar o classroom da disciplina dentro da organização.
2. Definir se a atividade será individual ou em grupo.
3. Configurar o repo starter, o prazo e o cutoff quando necessário.
4. Inserir cards de issue para dividir o trabalho em etapas.
5. Configurar branch por card, PR por entrega e template de revisão.
6. Ativar autograding quando houver critério objetivo de verificacao.
7. Fechar a rodada com feedback por PR.

## 3. Estrutura mínima de um repositório de atividade

Cada repositório criado pelo Classroom deve trazer:

- `README.md` com enunciado e critérios de aceite.
- `.github/ISSUE_TEMPLATE/atividade.yml` para padronizar o card.
- `.github/pull_request_template.md` para orientar revisão.
- Arquivos base do exercicio e, quando preciso, um exemplo de JSON.
- `AI_LOG.md` ou instruções equivalentes quando houver uso de IA.

## 4. Fluxo GitHub Pages

1. Ativar `Source: GitHub Actions` em `Settings > Pages`.
2. Fazer push na `main`.
3. Aguardar o workflow de deploy do MkDocs.
4. Compartilhar a URL pública com os alunos.

## Mini-caso prático

Publicar a primeira unidade no Pages e abrir uma atividade de laboratorio no Classroom com template inicial, cards de issue e rubricas simples.

## Perguntas de revisão rápida

1. Por que separar repositório de conteúdo e repositório de entrega?
2. Em que casos usar atividade em grupo?
3. Qual vantagem de cutoff em atividades com prazo fixo?
4. Por que os cenários práticos não devem ser publicados no GitHub Pages?

## Fontes de referência

- https://docs.github.com/en/pages
- https://docs.github.com/en/education/manage-coursework-with-github-classroom
- https://docs.github.com/en/communities/using-templates-to-encourage-useful-issues-and-pull-requests
