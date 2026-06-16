# Checklist Profissional de Entrega

## Objetivos de aprendizagem

- Definir quando uma tarefa de software pode ser considerada pronta.
- Revisar código com foco em responsabilidade, legibilidade, testes e rastreabilidade.
- Conectar Git, pull request, testes, documentação e uso de IA em um fluxo único de entrega.

**Tempo estimado:** 2h

## Vídeo de contexto

![type:video](https://www.youtube.com/embed/TO9xK4XTBbQ)

---

## 1. Por que este guia existe?

Em um curso de POO, não basta escrever uma classe que compila. O aluno precisa mostrar que a solução:

- representa corretamente o problema;
- protege o estado dos objetos;
- separa responsabilidades;
- é testável;
- pode ser revisada por outra pessoa;
- possui histórico de evolução;
- pode ser executada novamente depois da entrega.

Esse guia consolida o processo mínimo esperado para atividades práticas e projeto final.

---

## 2. Definition of Done da disciplina

Uma tarefa só deve ser considerada pronta quando todos os itens aplicáveis forem atendidos.

| Critério | Evidência esperada |
|---|---|
| Requisito implementado | código atende ao enunciado ou issue |
| Modelagem coerente | classes têm responsabilidades claras |
| Encapsulamento preservado | atributos críticos não ficam públicos sem justificativa |
| Testes executados | evidência local ou CI |
| Casos de erro tratados | exceções, validações ou mensagens controladas |
| README atualizado | comandos, contrato JSON e decisões relevantes |
| Pull request revisável | descrição, escopo e checklist preenchidos |
| AI_LOG atualizado | quando houver uso de agente de IA |
| Defesa possível | integrante consegue explicar o código entregue |

Regra prática: se outra dupla não consegue rodar ou revisar a entrega seguindo o README, a tarefa ainda não está pronta.

---

## 3. Checklist de qualidade de código

Use este checklist antes de abrir um pull request.

### Modelagem e POO

- [ ] Cada classe tem uma responsabilidade principal clara.
- [ ] Herança representa relação `é um`, não apenas reaproveitamento de código.
- [ ] Composição foi considerada antes de criar uma hierarquia.
- [ ] Métodos públicos formam uma interface pequena e compreensível.
- [ ] O estado interno é protegido por encapsulamento.
- [ ] Polimorfismo reduz `if` por tipo quando há famílias de comportamento.

### Legibilidade

- [ ] Nomes indicam intenção, não apenas tipo ou abreviação.
- [ ] Funções longas foram quebradas quando misturavam responsabilidades.
- [ ] Código duplicado foi reduzido quando a duplicação representava a mesma regra.
- [ ] Comentários explicam decisões não óbvias, não repetem a linha de código.

### Testabilidade

- [ ] Regras importantes podem ser testadas sem abrir a interface.
- [ ] Persistência está isolada por Repository ou função equivalente.
- [ ] Dados de teste são controlados.
- [ ] Há pelo menos um teste para caso de erro ou entrada inválida.
- [ ] O teste falha quando a regra principal é quebrada.

### Integração e operação

- [ ] Contrato JSON está documentado.
- [ ] Comandos de execução estão no README.
- [ ] Banco, arquivos e caminhos foram documentados.
- [ ] CI ou evidência local mostra os testes executados.
- [ ] A entrega não depende de arquivo local ausente no repositório.

---

## 4. Revisão por pull request

O pull request é a fronteira entre "funciona na minha máquina" e "pode entrar no projeto".

Um bom PR deve responder:

1. Qual problema foi resolvido?
2. Quais arquivos principais mudaram?
3. Como a mudança foi testada?
4. Qual risco ainda existe?
5. O README, contrato JSON ou AI_LOG precisou ser atualizado?

Modelo mínimo:

```markdown
## Resumo

Implementa validação de contrato JSON e testes de leitura inválida.

## Testes executados

- [x] `python3 -m pytest -q`
- [x] Validação manual do Streamlit com `dados/leituras_exemplo.jsonl`

## Riscos

- O teste de tela ainda é manual.

## Rastreabilidade de IA

- [x] `AI_LOG.md` atualizado
```

---

## 5. Qualidade de código não é estética

Qualidade de código é a capacidade de alterar o sistema sem quebrar comportamento importante.

| Sinal de baixa qualidade | Consequência | Ação recomendada |
|---|---|---|
| classe faz tudo | difícil testar e explicar | separar responsabilidades |
| muitos `if` por tipo | mudança espalhada | avaliar polimorfismo ou Strategy |
| atributos públicos | estado inválido aparece fácil | encapsular e validar |
| SQL dentro da tela inteira | persistência difícil de testar | criar Repository |
| JSON aceito sem validação | falhas aparecem tarde | criar teste de contrato |
| README sem comandos | entrega não reproduzível | documentar execução |
| teste só de caso feliz | erro real passa despercebido | testar falha e limite |

---

## 6. Mini-caso prático

A dupla implementou uma regra de bloqueio por pressão alta. O código funciona na demonstração, mas a bomba fica com `ligada = true` e `bloqueada = true` ao mesmo tempo.

Diagnóstico:

- há violação de invariante do objeto `Bomba`;
- o encapsulamento não protegeu completamente o estado;
- falta teste de comando para garantir que bloquear também desliga;
- a revisão por PR deveria detectar a inconsistência.

Ação recomendada:

1. ajustar o método `bloquear()`;
2. criar teste de regressão;
3. atualizar o PR com evidência;
4. explicar na defesa por que o estado anterior era inválido.

---

## 7. Perguntas de revisão rápida

1. Qual é a diferença entre "código compila" e "tarefa pronta"?
2. Por que qualidade de código ajuda a testar?
3. Qual evidência mínima um pull request deve trazer?

---

## Fontes de referência

- [GitHub Docs - About pull requests](https://docs.github.com/en/pull-requests)
- [GitHub Docs - Issue and pull request templates](https://docs.github.com/en/communities/using-templates-to-encourage-useful-issues-and-pull-requests)
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
- [Python Docs - Errors and Exceptions](https://docs.python.org/3/tutorial/errors.html)
