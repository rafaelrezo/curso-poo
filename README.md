# Curso de Programacao Orientada a Objetos (POO)

Repositorio base do curso de POO com foco em engenharia, modelagem da realidade e desenvolvimento orientado a praticas de mercado.

## Objetivo

Ensinar POO com `C++` como referencia inicial e `Python` como espelho de aplicacao, evoluindo para um projeto integrador com dados, backend e frontend.

## Publicacao com MkDocs

### Build local

```bash
python3 -m venv .venv
./.venv/bin/pip install -r requirements.txt
./.venv/bin/mkdocs build --clean
./.venv/bin/mkdocs serve
```

### Publicacao no GitHub Pages

Este repositorio inclui workflow em `.github/workflows/pages.yml` para deploy automatico quando houver push na branch `main`.

No GitHub:

1. Acesse `Settings > Pages`.
2. Em `Build and deployment`, selecione `Source: GitHub Actions`.
3. Garanta que a branch padrao do repositorio seja `main`.

## Uso com GitHub Classroom

Modelo recomendado:

1. Este repositorio funciona como base publica de conteudo (GitHub Pages).
2. Atividades sao distribuidas via GitHub Classroom (repos por aluno ou por grupo).
3. Conteudo e roteiros ficam centralizados aqui, e entregas em repos de tarefas.

## Estrutura

- `docs/`: conteudo do curso
- `mkdocs.yml`: navegacao e configuracao do site
- `AGENTS.md`: diretrizes editoriais e operacionais para agentes
- `.github/workflows/pages.yml`: deploy do site
