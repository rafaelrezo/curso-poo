# Diretrizes do Repositório: Curso de Programação Orientada a Objetos (MkDocs)

Estas diretrizes definem o padrão esperado para criação e revisão de conteúdo em `docs/`, com foco em ensino de POO para engenharia, prática de versionamento e uso orientado de agentes de IA no processo de aprendizagem.

## 1) Contexto e público-alvo

- Curso: Programação Orientada a Objetos (POO), carga total de `60h`.
- Formato de aula: encontros de `2h`.
- Público: alunos de engenharia com base prévia em C/programação procedural.
- Objetivo central: aprender modelagem orientada a objetos, análise crítica de código e aplicação prática em problemas reais.

## 2) Arquitetura pedagógica do curso

Distribuição macro da carga horária:

- `45%` (~`27h`): fundamentos e conceitos de POO (C++ como referência + paralelo em Python).
- `4% a 10%` (`2.4h` a `6h`): avaliações e checkpoints.
- `35%` (`21h`): projeto integrador em equipe (frontend, backend, dados e padrões de projeto).
- Horas restantes: nivelamento operacional (Git, IDE, fluxo com GitHub Classroom, uso de IA com responsabilidade).

## 3) Estrutura padrão de página

Toda página nova ou revisada deve seguir, preferencialmente, esta ordem:

1. `# Título da Página`
2. Bloco de objetivos:
   - `Objetivos de aprendizagem` (3 bullets)
   - `Tempo estimado`
3. Seção de vídeo no topo:
   - `## Vídeo de contexto` ou `## Vídeo da aula`
   - Embed com `mkdocs-video`: `![type:video](https://www.youtube.com/embed/VIDEO_ID)`
4. Conteúdo principal em seções numeradas (`## 1.`, `## 2.`, ...)
5. Ponte C++ -> Python:
   - destacar o mesmo conceito nas duas linguagens
6. Exemplo aplicado:
   - `Mini-caso prático` com cenário de engenharia/serviços/sistemas inteligentes
7. Fixação:
   - `Perguntas de revisão rápida` (3 perguntas)
8. Referências:
   - `Fontes de referência` com links oficiais

## 4) Estilo de escrita

- Português claro, objetivo e técnico.
- Tom de professor especialista, orientado à prática.
- Explicar: conceito -> impacto -> ação recomendada.
- Evitar jargão sem contextualização.
- Usar listas, tabelas e comparações para reduzir carga cognitiva.
- Incluir separadores `---` entre blocos maiores.

## 5) Qualidade técnica e atualização

- Garantir coerência conceitual entre as aulas da trilha.
- Priorizar fontes primárias e oficiais para conteúdos técnicos:
  - `cppreference`, `C++ Core Guidelines`, `Python Docs`, `GitHub Docs`, `git-scm`, `SQLite Docs`, `Streamlit Docs`.
- Quando houver afirmações operacionais sujeitas a mudança (GitHub Classroom, Codespaces, Pages), validar em documentação oficial atual.
- Traduzir teoria para impacto prático em modelagem e manutenção de software.

## 6) Diretrizes de linguagem (C++ e Python)

- C++ é a referência principal para explicar conceitos de POO no contexto de engenharia e embarcados.
- Python é usado como espelho para mostrar aplicação em sistemas de nível mais alto.
- Reforçar diferença entre:
  - conceito de POO (estável)
  - detalhe sintático da linguagem (consultável em material externo)
- Sempre que possível, incluir blocos curtos `C++` e `Python` para o mesmo conceito.

## 7) Diretrizes para vídeos

- Inserir `1` vídeo principal no topo da página.
- Priorizar conteúdo em português (pt-BR/pt-PT).
- Preferir vídeos curtos/moderados (ideal até ~20 min).
- Evitar conteúdo sensacionalista ou sem credibilidade técnica.

## 8) Atividades, avaliação e GitHub Classroom

- Todas as atividades devem privilegiar fluxo de mercado:
  - branch, commit, push, pull request e revisão.
- Usar GitHub Classroom para:
  - atividades individuais,
  - atividades em grupo,
  - prazo com cutoff,
  - autograding quando aplicável,
  - feedback por PR.
- Google Classroom pode ser usado como apoio de comunicação no início, mas o repositório oficial de entregas é o GitHub Classroom.

## 9) Uso de agentes de IA e skills

- O aluno pode usar agentes de IDE para apoiar:
  - geração inicial de código,
  - revisão de legibilidade,
  - sugestão de testes,
  - documentação.
- Não é permitido envio de código sem entendimento.
- Toda entrega com IA deve incluir uma trilha de rastreabilidade mínima:
  - o que foi pedido ao agente,
  - o que foi aceito/rejeitado,
  - justificativa técnica do aluno.
- Em atividades avaliativas, exigir defesa oral curta da solução.

## 10) Padrão para conteúdo comparativo

Quando a página comparar técnicas/metodologias/padrões, incluir tabela com no mínimo:

- `Técnica/Padrão`
- `Melhor uso`
- `Esforço`
- `Entregável`
- `Limitação`

Fechar com recomendação prática por cenário.

## 11) Checklist de revisão antes de publicar

- [ ] Estrutura da página segue o padrão definido neste arquivo.
- [ ] Há vídeo de contexto em português no topo.
- [ ] Conceitos de POO foram conectados a C++ e Python.
- [ ] Conteúdo está tecnicamente coerente e atualizado.
- [ ] Há mini-caso prático contextualizado.
- [ ] Há perguntas de revisão rápida.
- [ ] Há referências confiáveis quando aplicável.
- [ ] Navegação do `mkdocs.yml` foi ajustada (se necessário).
- [ ] Build local validado com `./.venv/bin/mkdocs build --clean`.

## 12) Escopo prioritário em `docs/`

- `fundamentos_git_ide/`
- `fundamentos_poo_cpp_python/`
- `modelagem_analise_codigo/`
- `projeto_integrador/`
- `avaliacoes/`
- `guias_operacionais/` (GitHub Classroom, GitHub Pages, fluxo de entrega)
