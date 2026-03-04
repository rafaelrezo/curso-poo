# GitHub Classroom e GitHub Pages

## Objetivos de aprendizagem

- Configurar turma e atividades no GitHub Classroom.
- Organizar repositorio de conteudo e repositorios de entrega.
- Publicar o material de aula no GitHub Pages.

**Tempo estimado:** 2h (docente/monitoria)

## Video de contexto

![type:video](https://www.youtube.com/embed/N6AYLOgYec8)

---

## 1. Arquitetura recomendada

- Este repositorio: conteudo do curso (MkDocs + Pages).
- Repositorios de atividade: criados pelo GitHub Classroom por aluno/equipe.

**Recomendacao de visibilidade**

- Conteudo de aula: publico (facilita acesso sem login).
- Entregas de atividade: separadas por assignment no GitHub Classroom.
- Observacao importante: o site publicado no GitHub Pages e publicamente acessivel.

## 2. Fluxo GitHub Classroom

1. Criar classroom da disciplina.
2. Configurar atividade individual ou em grupo.
3. Definir prazo e cutoff quando necessario.
4. Ativar autograding para criterios objetivos.
5. Realizar feedback por PR/commits.

## 3. Fluxo GitHub Pages

1. Ativar `Source: GitHub Actions` em `Settings > Pages`.
2. Fazer push na `main`.
3. Aguardar workflow `Deploy MkDocs to GitHub Pages`.
4. Compartilhar URL publica com os alunos.

## Mini-caso pratico

Publicar a primeira unidade no Pages e abrir uma atividade de laboratorio no Classroom com template inicial e rubricas simples.

## Perguntas de revisao rapida

1. Por que separar repositorio de conteudo e repositorio de entrega?
2. Em que casos usar atividade em grupo?
3. Qual vantagem de cutoff em atividades com prazo fixo?

## Fontes de referencia

- https://docs.github.com/en/pages
- https://docs.github.com/en/education/manage-coursework-with-github-classroom
