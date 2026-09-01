# Curso de Programação Orientada a Objetos (POO)

Repositorio base do curso de POO com foco em engenharia, modelagem da realidade e desenvolvimento orientado a práticas de mercado.

## Objetivo

Ensinar POO com `C++` como referência inicial e `Python` como espelho de aplicação, evoluindo para um projeto integrador com dados, backend e frontend.

## Uso no curso

- `GitHub Pages` pública o conteúdo didatico.
- Repositórios-base públicos na namespace `rafaelrezo` fornecem starters, testes e workflows.
- Cada estudante trabalha em seu fork; o Google Classroom pode comunicar prazos e receber o link da PR.
- Cada cenário prático vira um repositório-base público e independente na namespace `rafaelrezo`.
- Diretórios locais `repositorio-*-capNN/` mantêm os starters antes da publicação em seus repositórios próprios.

## Padrão de entrega

Cada repositório de atividade deve conter, no mínimo:

- `README.md` com contexto, escopo, arquitetura, comandos e critérios de aceite.
- Checkpoints e branches nomeadas para orientar o trabalho por etapas.
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

## Fluxo das atividades no GitHub

Modelo recomendado:

1. Este repositório funciona como base pública de conteúdo (GitHub Pages).
2. Atividades são distribuídas por repositórios-base públicos na namespace `rafaelrezo`.
3. O estudante faz fork, clona o próprio fork e trabalha em uma branch indicada.
4. Testes locais e GitHub Actions validam o mesmo contrato.
5. A entrega é uma PR da branch para a `main` do próprio fork; o repositório-base não recebe PRs dos estudantes.
6. Conteúdo e roteiros ficam centralizados aqui; código e evidências permanecem nos forks das atividades.

## Estrutura

- `docs/`: conteúdo do curso
- `mkdocs.yml`: navegacao e configuração do site
- `AGENTS.md`: diretrizes editoriais e operacionais para agentes
- `repositorio-*-capNN/`: fontes locais dos repositórios-base públicos por cenário
- `.github/workflows/pages.yml`: deploy do site
