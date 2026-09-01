# Repositórios-base, forks, GitHub Actions e GitHub Pages

## Objetivos de aprendizagem

- Publicar conteúdo didático no GitHub Pages e atividades em repositórios-base separados.
- Orientar o fluxo `fork → branch → teste local → push → Actions → PR no próprio fork`.
- Separar comunicação da turma, material didático e evidências verificáveis de código.

**Tempo estimado:** 1h para configuração inicial.

## Vídeo de contexto

![type:video](https://www.youtube.com/embed/UBAX-13g8OM)

---

## 1. Arquitetura adotada

O GitHub Classroom foi descontinuado e não faz parte do fluxo do curso.

| Espaço | Melhor uso | Acesso |
|---|---|---|
| GitHub Pages do curso | aulas, exemplos e guias | público |
| namespace `rafaelrezo` | repositórios-base das atividades | público |
| fork do estudante | implementação, histórico, testes, Actions e PR | conta do estudante |
| Google Classroom | avisos, prazos e link da PR, quando necessário | turma |

Cada atividade possui um repositório-base independente. A solução de referência é validada separadamente e não é publicada junto com o starter.

---

## 2. Fluxo do estudante

1. Abrir o repositório-base indicado na aula.
2. Selecionar **Fork** e criar uma cópia na própria conta.
3. Clonar o fork, não o repositório do docente.
4. Confirmar que existe somente `origin`, apontando para o fork.
5. Criar a branch obrigatória da etapa.
6. Executar o baseline antes de editar.
7. Implementar em incrementos e executar `make test ETAPA=NN`.
8. Fazer push e conferir a mesma validação no GitHub Actions.
9. Abrir PR da branch para a `main` do próprio fork.
10. Integrar somente depois dos testes verdes e da revisão.

Exemplo:

```bash
git clone https://github.com/SEU_USUARIO/NOME_DA_ATIVIDADE.git
cd NOME_DA_ATIVIDADE
git remote -v
git switch -c pratica/NN-descricao
make test ETAPA=NN
git push -u origin pratica/NN-descricao
```

Não configure `upstream` e não abra PR contra o repositório-base do docente.

---

## 3. Conteúdo mínimo do repositório-base

- `README.md` com baseline, arquivos permitidos, checkpoints e entrega;
- `CONTRIBUTING.md` com branch e limites de alteração;
- `AI_LOG.md` para rastreabilidade;
- código inicial compilável com comportamento incompleto observável;
- testes cumulativos e mensagens acionáveis;
- um único comando local por etapa;
- workflow com `contents: read` e branches explicitamente aceitas;
- `GUIA_DOCENTE.md` separado das instruções do estudante;
- diagrama ou documentação parcial quando fizer parte do objetivo.

---

## 4. Evidências da entrega

A PR no fork deve reunir:

- saída do teste local;
- link da execução verde no Actions;
- explicação técnica solicitada pela aula;
- diagrama atualizado, quando aplicável;
- commits pequenos e intencionais;
- `AI_LOG.md` preenchido ou declaração de não uso.

Testes visíveis não comprovam compreensão. A avaliação também considera diff, histórico, justificativa e defesa oral curta.

---

## 5. Publicação do material no Pages

O repositório `curso-poo` publica apenas conteúdo didático. Em **Settings → Pages**, use **GitHub Actions** como fonte. O workflow do site executa o build do MkDocs após mudanças na `main`.

Repositórios-base são separados para que os testes, branches e ciclos das atividades não se misturem com a publicação das aulas.

---

## Perguntas de revisão rápida

1. Por que a PR da atividade aponta para a `main` do próprio fork?
2. Qual diferença existe entre o GitHub Pages e o repositório-base?
3. Por que teste verde não substitui inspeção do diff e defesa oral?

## Fontes de referência

- [GitHub Docs — criar um fork](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/working-with-forks/fork-a-repo)
- [GitHub Docs — criar pull request a partir de um fork](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/working-with-forks/creating-a-pull-request-from-a-fork)
- [GitHub Docs — GitHub Actions](https://docs.github.com/en/actions)
- [GitHub Docs — GitHub Pages](https://docs.github.com/en/pages)
