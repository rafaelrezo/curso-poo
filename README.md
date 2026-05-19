# Curso de Programação Orientada a Objetos (POO)

Repositorio base do curso de POO com foco em engenharia, modelagem da realidade e desenvolvimento orientado a práticas de mercado.

## Objetivo

Ensinar POO com `C++` como referência inicial e `Python` como espelho de aplicação, evoluindo para um projeto integrador com dados, backend e frontend.

## Uso no curso

- `GitHub Pages` pública o conteúdo didatico.
- `GitHub Classroom` hospeda os repositórios de entrega, os cards de issue e os PRs.
- `Google Classroom` fica como canal de comunicação da turma.
- Cada cenário prático vira um repositório starter privado e independente na organização do curso.
- `classroom_starters/` espelha localmente esses repositórios privados para manutenção.

## Padrão de entrega

Cada repositório de atividade deve conter, no mínimo:

- `README.md` com contexto, escopo, arquitetura, comandos e critérios de aceite.
- Cards de issue para orientar o trabalho por etapas.
- `AI_LOG.md` com o que foi pedido ao agente, o que foi aceito, o que foi rejeitado e a justificativa.
- Branch por card e `pull request` para revisão.

## Publicacao com MkDocs

### Build local

```bash
python3 -m venv .venv
./.venv/bin/pip install -r requirements.txt
./.venv/bin/mkdocs build --clean
./.venv/bin/mkdocs serve
```

### Publicacao no GitHub Pages

Este repositório inclui workflow em `.github/workflows/pages.yml` para deploy automático quando houver push na branch `main`.

No GitHub:

1. Acesse `Settings > Pages`.
2. Em `Build and deployment`, selecione `Source: GitHub Actions`.
3. Garanta que a branch padrão do repositório seja `main`.

## Uso com GitHub Classroom

Modelo recomendado:

1. Este repositório funciona como base pública de conteúdo (GitHub Pages).
2. Atividades são distribuidas via GitHub Classroom (repos por aluno ou por grupo).
3. Conteudo e roteiros ficam centralizados aqui, e entregas em repos de tarefas.

## Estrutura

- `docs/`: conteúdo do curso
- `mkdocs.yml`: navegacao e configuração do site
- `AGENTS.md`: diretrizes editoriais e operacionais para agentes
- `classroom_starters/`: espelho local dos repositórios starter privados por cenário
- `.github/workflows/pages.yml`: deploy do site
