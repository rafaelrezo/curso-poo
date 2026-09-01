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
- Horas restantes: nivelamento operacional (Git, IDE, fork e fluxo com GitHub, uso de IA com responsabilidade).

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
- Quando houver afirmações operacionais sujeitas a mudança (GitHub, Codespaces, Actions, Pages), validar em documentação oficial atual.
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

## 8) Atividades, avaliação e repositórios no GitHub

- Todas as atividades devem privilegiar fluxo de mercado:
  - branch, commit, push, pull request e revisão.
- O GitHub Classroom foi descontinuado e não deve ser recomendado ou usado em materiais novos ou revisados.
- Cada atividade de programação deve possuir um repositório-base público na namespace `rafaelrezo` do GitHub.
- O estudante deve fazer fork do repositório-base, clonar o próprio fork e manter somente `origin` apontando para ele.
- Não configurar `upstream` nas atividades didáticas, salvo necessidade excepcional explicitamente documentada.
- A implementação ocorre em branch nomeada; os testes são executados localmente e pelo GitHub Actions após o push.
- A entrega é organizada por pull request da branch para a `main` do próprio fork. Nenhuma PR deve ser aberta contra o repositório-base do docente.
- Google Classroom pode ser usado apenas para comunicação, prazos e envio do link da PR quando necessário; código, histórico, testes e revisão permanecem no fork do estudante.

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
- `revisao_programacao_algoritmos/`
- `fundamentos_poo_cpp_python/`
- `modelagem_analise_codigo/`
- `projeto_integrador/`
- `avaliacoes/`
- `guias_operacionais/` (fork, GitHub Actions, GitHub Pages e fluxo de entrega)

## 13) Padrão didático validado em sala

Ao criar ou revisar uma trilha, preservar o estilo validado na seção `01. Fundamentos Git e IDE`:

1. partir do que o estudante vê, já conhece ou consegue executar;
2. apresentar um problema concreto antes do conceito ou da ferramenta que o resolve;
3. introduzir uma ideia nova por vez e explicar `conceito -> necessidade -> ação -> resultado observável`;
4. manter um mesmo mini-domínio de engenharia ao longo da aula para reduzir trocas de contexto;
5. alternar explicações curtas com execução imediata, sem concentrar toda a prática no final;
6. fornecer comandos e código reproduzíveis, saída esperada e diagnóstico de erros comuns;
7. usar checkpoints cumulativos nos quais cada etapa reaproveita e melhora a anterior;
8. encerrar com uma entrega verificável, checklist de evidências e perguntas de revisão;
9. incorporar o fluxo profissional de `branch`, commits pequenos, `push` e pull request quando houver entrega;
10. nas transições entre linguagens ou paradigmas, fixar primeiro o conceito comum e só depois comparar a sintaxe.

### Critério de sequenciamento

Cada bloco deve responder, nesta ordem:

- **De onde partimos?** conhecimento prévio e estado atual do artefato;
- **Qual problema apareceu?** limitação que o aluno consegue observar;
- **Qual ideia resolve?** conceito apresentado sem antecipação desnecessária;
- **Como aplicar agora?** modificação curta, executável e guiada;
- **Como confirmar?** saída esperada, teste ou inspeção;
- **O que isso prepara?** ligação explícita com o bloco ou capítulo seguinte.

Evitar páginas organizadas como catálogos extensos de sintaxe. A progressão deve formar uma narrativa prática e cumulativa.

## 14) Padrão de aula conceitual com prática profissional

As novas aulas e revisões devem associar fundamentos conceituais a procedimentos atuais de engenharia de software. A complexidade operacional pode ser avançada, desde que seja introduzida de forma guiada, cumulativa e compatível com o tempo da aula.

### Estrutura recomendada

1. apresentar um problema observável do domínio;
2. explicitar o conceito necessário para resolver uma parte desse problema;
3. aplicar imediatamente o conceito em um artefato executável;
4. trabalhar em repositório real, preferencialmente distribuído por fork;
5. isolar cada incremento em uma branch com nome definido;
6. fornecer validação local reproduzível;
7. executar validação remota por CI a cada push;
8. usar a falha de compilação ou teste como feedback formativo;
9. integrar a branch somente depois de obter evidência de funcionamento;
10. encerrar mostrando limites da solução atual e a necessidade do conceito seguinte.

### Práticas modernas a incorporar quando pertinentes

- fork, clone e remoto `origin` apontando para o fork do estudante;
- branches curtas e cumulativas;
- commits pequenos, intencionais e verificáveis;
- build automatizado com avisos habilitados;
- testes de contrato, casos comuns, fronteiras e erros;
- GitHub Actions com permissões mínimas e sem segredos desnecessários;
- feedback de CI associado ao commit;
- prevenção de regressões por testes cumulativos;
- pull request e revisão quando contribuírem para o objetivo da aula;
- documentação da decisão técnica e rastreabilidade do uso de IA.

Essas práticas não devem aparecer como uma lista desconectada de ferramentas. Cada procedimento precisa resolver uma necessidade percebida durante a atividade e produzir uma evidência que o estudante consiga interpretar.

### Critérios para repositórios de atividade

Quando a aula usar um repositório-base:

- publicar o starter em um repositório público separado na namespace `rafaelrezo`;
- manter um `README.md` com requisitos, fluxo, branches e comandos;
- separar contrato, implementação e testes quando isso favorecer a aprendizagem;
- fazer o código inicial compilar, ainda que os testes funcionais falhem de forma intencional;
- apresentar mensagens de teste específicas e acionáveis;
- repetir testes anteriores nas etapas seguintes para detectar regressões;
- validar a solução de referência antes da publicação, sem publicá-la junto ao exercício;
- documentar limites da automação, especialmente quando testes e workflows forem visíveis no fork;
- incluir orientações separadas para estudantes e docente quando houver operação de CI;
- restringir workflows ao menor conjunto de permissões necessário;
- garantir que a atividade possa ser concluída no tempo declarado.

### Padrão obrigatório de prática cumulativa com fork e CI

Quando uma aula introduzir ou ampliar um artefato de programação, preservar o padrão validado nos capítulos de Git e de revisão de C:

1. publicar um repositório-base do docente e orientar o estudante a trabalhar em um fork;
2. fazer o código inicial compilar e executar, com marcadores claros de comportamento ainda incompleto;
3. usar o GitHub Pages como guia para o estudante completar o primeiro incremento, sem publicar a solução pronta no repositório-base;
4. reservar a prática seguinte para uma extensão do exemplo, exigindo decisão e adaptação do estudante em vez de simples cópia;
5. criar uma branch nomeada para cada etapa e integrar somente depois da validação;
6. oferecer um único comando local explícito, preferencialmente `make test ETAPA=NN`, e executar exatamente o mesmo contrato na CI após o push;
7. tornar os testes cumulativos: cada nova etapa repete as anteriores para detectar regressões;
8. fazer mensagens de falha e saídas esperadas indicarem ao estudante o comportamento que precisa ser corrigido;
9. executar o workflow em pushes das branches previstas, com permissões mínimas e associação inequívoca entre branch e etapa;
10. exigir no pull request evidências da validação local e remota, explicação técnica e rastreabilidade do uso de IA.

O pull request deve ser aberto da branch de atividade para a `main` do próprio fork. O clone deve possuir somente o remoto `origin` apontando para o fork do estudante; não usar `upstream` nem enviar contribuições ao repositório-base como parte da entrega.

Os testes devem privilegiar comportamento observável e contratos, não buscas superficiais por palavras-chave. Como workflows e testes ficam visíveis no fork, a automação deve ser complementada por inspeção do diff, justificativa no pull request e, em avaliações, defesa oral curta.

### Equilíbrio pedagógico

O conteúdo deve preservar três camadas conectadas:

| Camada | Pergunta orientadora | Evidência |
|---|---|---|
| Conceito | por que essa ideia existe? | explicação do problema e da regra |
| Implementação | como a ideia aparece no código? | programa compilável e legível |
| Engenharia | como verificar e integrar com segurança? | testes, CI, histórico e revisão |

Uma camada não deve substituir as outras. A automação confirma comportamentos observáveis, mas o estudante ainda deve explicar o conceito, ler o código e justificar as decisões tomadas.
