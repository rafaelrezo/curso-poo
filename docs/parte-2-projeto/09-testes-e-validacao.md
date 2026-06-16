# Aula 9 - Fundamentos de Testes, Qualidade e Risco

## Objetivos de aprendizagem

- Diferenciar testes unitários, de integração, de contrato, de sistema, regressão, aceitação, carga e fumaça.
- Relacionar cada tipo de teste com o risco que ele reduz no projeto da estação.
- Conectar testes com qualidade de código, revisão por pull request e defesa técnica.

**Tempo estimado:** 1h30.

## Vídeo da aula

![type:video](https://www.youtube.com/embed/5HBS5M97wXo)

---

## 1. Por que testar no projeto integrador?

No início do projeto, parece suficiente executar o programa e olhar a saída. Essa estratégia falha quando o sistema passa a ter regras, persistência, alarmes e duas linguagens conversando entre si.

Uma alteração pequena pode quebrar:

- a regra que liga ou desliga a bomba;
- o bloqueio por pressão alta;
- a geração de comandos;
- o formato do JSON enviado pelo controlador;
- a separação entre leitura válida e inválida;
- a gravação e consulta no SQLite;
- a lista de alarmes exibida no mini-SCADA.

Teste transforma expectativa em verificação repetível.

Exemplo de expectativa operacional:

```text
Se o nível estiver abaixo de 30% e a pressão estiver normal,
a regra deve gerar LIGAR_BOMBA.
```

Essa frase deve virar teste antes da apresentação final.

---

## 2. Vocabulário essencial

| Tipo de teste | Pergunta principal | Exemplo no projeto | Custo típico | Limitação |
|---|---|---|---|---|
| Unitário | uma unidade isolada funciona? | `ControleNormal.decidir(25, 2)` gera `LIGAR_BOMBA` | baixo | não prova integração |
| Integração | duas ou mais partes conversam? | Repository salva e consulta no SQLite | médio | pode ser mais lento |
| Contrato | o formato combinado foi respeitado? | JSON tem `tag`, `valor`, `unidade`, `timestamp`, `status` | baixo/médio | não valida a tela |
| Sistema | o fluxo completo funciona? | C++ gera JSON, Python lê, salva e exibe | alto | mais difícil de diagnosticar |
| Regressão | um erro corrigido continua corrigido? | leitura inválida não derruba o supervisor | baixo/médio | depende de histórico de falhas |
| Aceitação | o usuário consegue validar o requisito? | checklist da demonstração no Streamlit | médio | pode envolver avaliação manual |
| Carga | o sistema suporta volume esperado? | importar 10.000 leituras sem travar | médio/alto | pode ser exagero para o curso |
| Fumaça | o sistema básico subiu? | app abre, banco inicializa e há rota/tela principal | baixo | cobre pouco comportamento |

### Recomendação prática por cenário

| Cenário | Teste prioritário | Por quê |
|---|---|---|
| Regra de controle | unitário | regra deve ser rápida de verificar e fácil de explicar |
| JSON entre C++ e Python | contrato | evita quebra silenciosa entre as camadas |
| SQLite | integração | o risco está na conversa com o banco |
| Streamlit | fumaça e aceitação | parte visual precisa de evidência de funcionamento |
| Falha já encontrada | regressão | impede que o erro volte antes da entrega |
| Demonstração final | sistema e checklist | comprova fluxo visível para a banca |

---

## 3. Pirâmide de testes aplicada

Uma forma prática de pensar testes é separar quantidade e custo.

```text
          poucos testes de sistema
        alguns testes de integração
      muitos testes unitários e de contrato
```

No projeto da estação, a base deve ser:

- muitos testes de regras, comandos e validação de JSON;
- alguns testes de Repository com SQLite temporário;
- poucos testes de fluxo completo;
- uma validação manual objetiva da interface.

Impacto prático: quando um teste unitário falha, a causa costuma estar perto. Quando um teste de sistema falha, o problema pode estar em regra, JSON, arquivo, banco, tempo de execução ou interface.

---

## 4. O que testar neste projeto

| Parte do projeto | Risco | Teste recomendado | Evidência esperada |
|---|---|---|---|
| `Strategy` | regra errada liga bomba em condição insegura | unitário | casos de nível baixo, nível alto e pressão alta |
| `Command` | comando altera estado indevido | unitário | bomba ligada, desligada ou bloqueada |
| `Observer` | alarme crítico não é registrado | unitário ou integração leve | evento gera alarme esperado |
| Contrato JSON | Python não entende saída do C++ | contrato | arquivo JSON Lines validado |
| Repository | dado some ou volta incorreto | integração | salvar e listar leitura |
| Streamlit | tela quebra durante a apresentação | fumaça/app test | app executa sem exceção |
| Projeto final | fluxo real da dupla funciona | sistema + aceitação | roteiro de demonstração reproduzível |

!!! tip "Referência opcional"

    Para visualizar uma base pequena e executável com testes C++ e Python, consulte o repositório de apoio [`testes_ci_estacao`](https://github.com/rafaelrezo/curso-poo-exemplos/tree/main/testes_ci_estacao). Ele não é modelo de entrega final; é uma referência para estudar organização, testes e evidências.

---

## 5. Ponte C++ -> Python

O conceito de teste é o mesmo nas duas linguagens: preparar entrada, executar comportamento e verificar saída ou efeito.

| Conceito | C++ | Python |
|---|---|---|
| Verificação simples | `assert(condicao)` | `assert condicao` |
| Falha esperada | testar exceção ou retorno de erro | `pytest.raises(...)` |
| Dados temporários | arquivo em pasta de teste | `tmp_path` do `pytest` |
| Regra de negócio | classe `Strategy` | função, classe ou serviço |
| Integração com banco | SQLite temporário | SQLite temporário |
| Interface | execução manual ou framework externo | `streamlit.testing.v1.AppTest` |

Para este curso, a recomendação é começar simples:

- C++: `assert` em arquivos de teste pequenos;
- Python: `pytest`;
- Streamlit: primeiro separar lógica da tela, depois testar a tela quando fizer sentido;
- CI: executar os mesmos comandos locais em GitHub Actions.

---

## 6. Estrutura de um bom teste

Um teste legível costuma seguir o padrão AAA.

| Etapa | Pergunta | Exemplo |
|---|---|---|
| Arrange | quais dados e objetos preparo? | criar `ControleNormal` e leituras de entrada |
| Act | qual ação executo? | chamar `decidir(nivel, pressao)` |
| Assert | o que precisa ser verdadeiro? | comparar com `TipoComando::LigarBomba` |

Exemplo em pseudocódigo:

```text
preparar regra de controle
executar decisão com nível baixo e pressão normal
verificar se o comando é LIGAR_BOMBA
```

Características esperadas:

- nome descreve comportamento, não implementação;
- entrada é controlada;
- não depende de horário real, arquivo manual ou ordem aleatória;
- falha aponta para uma causa compreensível;
- roda rápido o bastante para ser executado antes de cada pull request.

---

## 7. Dublês de teste: fake, stub e mock

Em projetos reais, nem sempre é desejável chamar banco, rede ou serviço externo em todo teste. Dublês de teste substituem dependências para reduzir custo e instabilidade.

| Técnica/Padrão | Melhor uso | Esforço | Entregável | Limitação |
|---|---|---|---|---|
| Fake | substituir dependência por implementação simples | baixo | `RepositorioLeiturasMemoria` | pode esconder problema do banco real |
| Stub | devolver resposta fixa para um cenário | baixo | sensor simulado com valor constante | não verifica interação |
| Mock | verificar chamada ou interação esperada | médio | objeto que registra chamada de alarme | pode prender o teste à implementação |
| Fixture | preparar dados repetíveis | baixo | leitura válida padrão | fixture ruim vira acoplamento |
| Banco temporário | testar persistência real sem sujar dados | médio | SQLite em arquivo temporário | mais lento que teste unitário |

Recomendação prática: use fake para regra e fluxo interno; use banco temporário para provar que o Repository real funciona; use mock apenas quando a interação for o comportamento principal.

---

## 8. Testabilidade e POO

Código orientado a objetos fica mais testável quando as responsabilidades estão separadas.

| Decisão de projeto | Impacto no teste |
|---|---|
| `Strategy` para regra de controle | permite testar a regra sem ligar Streamlit ou banco |
| `Command` para atuação | permite testar efeito no estado da bomba |
| `Repository` para persistência | permite trocar SQLite por repositório em memória |
| `Observer` para alarmes | permite testar reação a eventos separadamente |
| validação de JSON fora da tela | permite testar contrato sem abrir o app |
| lógica fora de callbacks Streamlit | reduz teste manual e aumenta teste automático |

Ação recomendada: se uma parte importante só pode ser testada abrindo a interface, provavelmente há lógica demais dentro da tela.

---

## 9. Qualidade de código: visão geral

Teste não é a única evidência de qualidade. Um teste pode passar em um código difícil de entender, duplicado ou acoplado demais. Qualidade de código, neste curso, significa facilidade de entender, testar, alterar e defender tecnicamente a solução.

| Critério | Pergunta de revisão | Relação com POO |
|---|---|---|
| Responsabilidade | esta classe tem um motivo claro para existir? | classe representa conceito do domínio |
| Encapsulamento | o estado interno pode ficar inválido por acesso externo? | atributos críticos devem ser protegidos |
| Coesão | os métodos da classe trabalham para o mesmo propósito? | comportamento fica perto do dado que governa |
| Acoplamento | uma mudança pequena obriga alterar muitos arquivos? | abstrações reduzem dependência direta |
| Legibilidade | outro aluno entende o fluxo sem adivinhar intenção? | nomes e interfaces comunicam modelo |
| Testabilidade | a regra pode ser testada sem abrir a tela? | responsabilidades separadas permitem teste menor |

### Sinais de alerta

| Sinal | Risco | Ação recomendada |
|---|---|---|
| classe com muitas responsabilidades | teste fica grande e frágil | separar regra, persistência e interface |
| muitos `if` verificando tipo | mudança espalhada | avaliar polimorfismo ou Strategy |
| atributos públicos em objetos centrais | estado inválido | encapsular e criar métodos de consulta |
| SQL dentro da tela inteira | persistência difícil de testar | usar Repository |
| JSON aceito sem validação | erro aparece tarde | criar teste de contrato |
| função longa misturando leitura, regra e saída | revisão difícil | quebrar por intenção |

Qualidade de código não é estética. É reduzir o custo de mudança. No projeto final, o aluno deve conseguir apontar uma decisão de modelagem, mostrar o teste correspondente e explicar por que aquela organização facilita manutenção.

---

## 10. Revisão por PR e Definition of Done

Antes de considerar uma tarefa pronta, a dupla deve verificar se existe evidência suficiente.

| Item | Evidência mínima |
|---|---|
| requisito implementado | issue ou descrição do PR |
| regra importante testada | teste unitário ou de contrato |
| integração validada | teste de Repository, JSON Lines ou checklist |
| README atualizado | comandos de execução e contrato |
| IA rastreada | `AI_LOG.md`, quando aplicável |
| risco declarado | observação no PR ou na defesa |

Um pull request deve responder três perguntas:

1. o que mudou?
2. como foi testado?
3. o que ainda pode falhar?

Esse processo é parte da qualidade. Ele obriga a equipe a transformar "acho que funciona" em evidência revisável.

!!! note "Guia operacional"

    Para aplicar esse processo em atividades e projeto final, use o [Checklist Profissional de Entrega](../guias_operacionais/checklist_entrega_profissional.md).

---

## 11. Teste local, CI e ambiente real

Testes são executados em momentos diferentes.

| Momento | Quem executa | Objetivo |
|---|---|---|
| Local, antes do commit | aluno | detectar erro rápido |
| Local, antes do push | dupla | evitar subir código quebrado |
| Pull request | GitHub Actions | impedir integração de mudança problemática |
| Antes da demonstração | equipe | validar roteiro completo |
| Ambiente produtivo real | equipe ou pipeline | checar configuração, dependências, saúde e regressões críticas |

Em um ambiente produtivo real, os testes não substituem monitoramento. Depois de publicar, a equipe precisa observar logs, erros, tempo de resposta, conexão com banco e funcionamento mínimo da tela. Para o curso, isso entra como simulação profissional: CI antes do merge, smoke test depois de subir e checklist de aceitação para a entrega.

---

## 12. Mini-caso prático

A dupla definiu uma regra própria:

```text
Se a vazão ficar abaixo de 12 m3/h por dois ciclos seguidos,
gerar FALHA_BAIXA_VAZAO e bloquear a partida.
```

Testes mínimos para essa regra:

1. uma leitura isolada de vazão baixa ainda não bloqueia;
2. duas leituras seguidas de vazão baixa bloqueiam;
3. vazão normal depois da falha registra recuperação ou mantém histórico;
4. o JSON do alarme possui `origem`, `alarme`, `severidade` e `timestamp`;
5. o supervisor exibe o alarme no histórico.

Esse mini-caso mostra como transformar requisito operacional em evidência técnica e revisão de qualidade.

Checklist de qualidade para esse mini-caso:

- a regra de vazão não deve ficar misturada com a tela;
- o bloqueio da partida deve preservar o estado válido da bomba;
- o alarme deve ter contrato JSON documentado;
- o teste deve cobrir o primeiro ciclo baixo, o segundo ciclo baixo e a recuperação;
- o PR deve declarar qual risco ainda não foi automatizado.

---

## 13. Perguntas de revisão rápida

1. Por que teste de contrato é diferente de apenas verificar se o JSON abre?
2. Qual é o risco de testar somente pela tela do Streamlit?
3. Por que qualidade de código não é apenas estética?
4. Em que situação um teste de regressão deve ser criado?

---

## Fontes de referência

- [cppreference - `assert`](https://en.cppreference.com/w/cpp/error/assert)
- [GoogleTest - Primer](https://google.github.io/googletest/primer.html)
- [pytest Docs - Get Started](https://docs.pytest.org/en/stable/getting-started.html)
- [pytest Docs - Good Integration Practices](https://docs.pytest.org/en/stable/explanation/goodpractices.html)
- [Streamlit Docs - App testing](https://docs.streamlit.io/develop/concepts/app-testing)
- [GitHub Docs - Guia Rápido do GitHub Actions](https://docs.github.com/pt/actions/get-started/quickstart)
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
