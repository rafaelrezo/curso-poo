# UML e Diagrama de Classes: modelando objetos antes do código

## Objetivos de aprendizagem

- Entender como UML ajuda a representar classes, atributos, operações e relações de forma útil para POO.
- Ler e construir diagramas de classes com foco em responsabilidade, visibilidade, multiplicidade e tipo de relacionamento.
- Traduzir um diagrama de classes para decisões concretas em C++ e Python sem confundir modelo com sintaxe.

**Tempo estimado:** 4h

## Vídeo da aula

![type:video](https://www.youtube.com/embed/rDidOn6KN9k)

---

## 1. Por que UML entra agora na trilha?

A turma já viu `classe`, `objeto`, `método`, `construtor`, encapsulamento, herança e polimorfismo como ideias de POO. O próximo passo natural é aprender a **pensar o modelo antes de escrever o código**.

E aqui entra a UML.

UML não substitui implementação. UML também não existe para produzir desenho bonito. Em um curso de POO, ela serve para um objetivo muito mais útil:

- deixar explícitas as classes importantes do domínio;
- mostrar quem conhece quem;
- registrar responsabilidades e limites de acesso;
- discutir erros de modelagem antes de gastar tempo com código.

**Ideia central desta aula:** diagrama de classes é uma ferramenta de raciocínio. Ele ajuda a validar a estrutura do sistema antes que a sintaxe esconda os problemas de modelagem.

---

## 2. O que é UML e o que um diagrama de classes realmente mostra?

A UML é uma linguagem padrão de modelagem usada para visualizar e especificar a estrutura de sistemas de software. Dentro dela, o diagrama de classes é um dos artefatos mais importantes para POO porque ele mostra a **estrutura estática** do sistema:

- classes;
- atributos;
- operações;
- relacionamentos;
- multiplicidades;
- visibilidade e abstração.

### Classe UML mais simples possível

``` mermaid
classDiagram
    class Sensor
```

Esse primeiro desenho quase não diz nada. Ele apenas informa que `Sensor` é uma classe relevante no modelo.

### Classe UML com atributos e operações

``` mermaid
classDiagram
    class Sensor {
        -tag: string
        -unidade: string
        -valorAtual: double
        +atualizarLeitura(valor: double) void
        +foraDaFaixa(min: double, max: double) bool
        +exibirResumo() void
    }
```

### Como ler esse retângulo

- parte superior: nome da classe;
- parte central: atributos ou estado;
- parte inferior: operações ou comportamento;
- símbolos como `+`, `-` e `#`: nível de visibilidade.

### Leitura didática da visibilidade

| Notação UML | Significado | Relação com POO | Pergunta prática |
|---|---|---|---|
| `+` | público | faz parte do contrato externo | o restante do sistema realmente precisa chamar isso? |
| `-` | privado | protege detalhe interno | esse dado precisa ser mexido de fora? |
| `#` | protegido | extensão por herança | subclasses precisam mesmo enxergar isso? |
| `<<abstract>>` | classe abstrata | contrato parcial | faz sentido existir objeto direto dessa classe? |

### Ponto importante para a turma

Diagrama de classes **não é código executável**. Ele simplifica.

Por isso, o diagrama não precisa listar cada getter, cada setter ou cada detalhe de biblioteca. O foco deve ficar no que ajuda a entender o modelo.

---

## 3. Como o diagrama de classes se relaciona com POO?

O diagrama de classes não é um assunto paralelo a POO. Ele é uma forma de **explicitar visualmente** os mesmos conceitos que depois aparecem no código.

| Conceito de POO | Como aparece no diagrama | O que isso ajuda a decidir |
|---|---|---|
| Classe | retangulo com nome | quais entidades são centrais no domínio |
| Estado | atributos | o que cada objeto precisa guardar |
| Comportamento | operações | que ações pertencem ao objeto |
| Encapsulamento | visibilidade `+`, `-`, `#` | o que fica exposto e o que fica protegido |
| Herança | generalização | quando uma classe realmente é um subtipo de outra |
| Composição/agregação | relações entre classes | como objetos colaboram sem virar uma massa de código |
| Polimorfismo | abstração e especialização | que contrato comum diferentes objetos podem respeitar |

### Exemplo curto: o diagrama como ponte para encapsulamento e herança

``` mermaid
classDiagram
    class AtivoIndustrial {
        #tag: string
        #area: string
        +exibirIdentificacao() void
    }

    class SensorPressao {
        -valorAtual: double
        +atualizarLeitura(valor: double) void
    }

    class BombaRecalque {
        -ligada: bool
        +ligar() void
        +desligar() void
    }

    AtivoIndustrial <|-- SensorPressao
    AtivoIndustrial <|-- BombaRecalque
```

### O que esse desenho já comunica antes do código

- `SensorPressao` e `BombaRecalque` compartilham uma base comum;
- a classe-base concentra identidade e informação operacional comum;
- cada derivada tem estado e comportamento específicos;
- a estrutura sugere uma relação `is-a`, não apenas reaproveitamento oportunista.

**Leitura crítica:** se a única semelhança entre duas classes for ter `tag`, isso ainda não justifica herança.

---

## 4. Relacionamentos UML: como ler cada tipo corretamente

O maior erro dos iniciantes costuma ser este: desenhar classes isoladas, mas errar o tipo de relação entre elas.

Em POO, boa parte da qualidade da modelagem depende menos do nome das classes e mais de **como elas se conectam**.

### Primeiro treino de leitura: um diagrama com símbolos diferentes

Antes de separar cada relação por tipo, vale treinar a leitura de um diagrama pequeno que mistura símbolos diferentes no mesmo desenho.

``` mermaid
classDiagram
    class Planta
    class Tanque
    class SensorNivel
    class Controlador
    class HistoricoLeituras

    Planta "1" *-- "1..*" Tanque : contém
    Tanque "1" o-- "0..*" SensorNivel : agrega
    Controlador --> "1" Tanque : regula
    Controlador ..> HistoricoLeituras : consulta
```

### Como ler esse diagrama, relação por relação

1. `Planta "1" *-- "1..*" Tanque : contém`
   Leitura: uma `Planta` contém um ou muitos `Tanque`.
   Interpretação: o diamante **preenchido** indica **composição**. A `Planta` é o todo forte; `Tanque` foi modelado como parte estrutural dela.

2. `Tanque "1" o-- "0..*" SensorNivel : agrega`
   Leitura: um `Tanque` agrega zero ou muitos `SensorNivel`.
   Interpretação: o diamante **vazio** indica **agregação**. O tanque agrupa sensores, mas os sensores podem continuar existindo fora desse agrupamento.

3. `Controlador --> "1" Tanque : regula`
   Leitura: o `Controlador` regula um `Tanque`.
   Interpretação: a linha **contínua com seta** indica **associação navegável**. O controlador conhece o tanque e interáge com ele.

4. `Controlador ..> HistoricoLeituras : consulta`
   Leitura: o `Controlador` consulta `HistoricoLeituras`.
   Interpretação: a linha **tracejada com seta** indica **dependência**. O controlador usa esse elemento pontualmente, mas isso não significa, obrigatoriamente, que ele o mantenha como parte permanente do estado interno.

### O que cada símbolo significa nesse desenho

| Símbolo | Como identificar visualmente | Significado | Pergunta que ele responde |
|---|---|---|---|
| `*--` | diamante cheio | composição | "esta parte depende fortemente do todo?" |
| `o--` | diamante vazio | agregação | "o todo apenas agrupa partes independentes?" |
| `-->` | linha contínua com seta | associação | "uma classe conhece ou usa estruturalmente a outra?" |
| `..>` | linha tracejada com seta | dependência | "uma classe apenas usa a outra de forma pontual?" |
| `"1"`, `"0..*"`, `"1..*"` | números perto da linha | cardinalidade | "quantos objetos podem participar da relação?" |

### Dica de leitura que evita erro

Leia sempre em duas etapas:

1. descubra **o tipo do vínculo** pelo símbolo da linha;
2. descubra **a quantidade** pelos números perto da relação.

### Erros comuns nessa leitura

- achar que todo diamante significa a mesma coisa;
- ignorar o lado em que o diamante aparece;
- confundir seta contínua com dependência;
- esquecer de ler a cardinalidade junto com o tipo da relação.

### 4.1 Generalizacao: quando uma classe e um tipo mais específico de outra

Generalizacao e a forma UML de representar herança.

A pergunta certa aqui e:

**"a classe filha realmente e um tipo da classe pai?"**

Se a resposta for sim, generalização pode fazer sentido.

``` mermaid
classDiagram
    class AtivoIndustrial {
        -tag: string
        +exibirIdentificacao() void
    }

    class SensorTemperatura {
        -valorAtual: double
        +ler() double
    }

    class BombaCirculacao {
        -ligada: bool
        +ligar() void
        +desligar() void
    }

    AtivoIndustrial <|-- SensorTemperatura
    AtivoIndustrial <|-- BombaCirculacao
```

### Como ler

- `SensorTemperatura` e um tipo de `AtivoIndustrial`;
- `BombaCirculacao` também e um tipo de `AtivoIndustrial`;
- a classe-base concentra o que e comum;
- as classes derivadas especializam comportamento e estado.

### Generalizacao separada e compartilhada

Ao discutir herança com mais cuidado, vale apresentar duas leituras possíveis para as especializacoes:

- **separada** ou **disjunta**: um objeto pertence a apenas uma das subclasses;
- **compartilhada** ou **sobreposta**: um mesmo objeto pode pertencer a mais de uma subclasse ao mesmo tempo.

``` mermaid
classDiagram
    class Pessoa
    class Aluno
    class Monitor

    Pessoa <|-- Aluno
    Pessoa <|-- Monitor
```

### Leitura didatica desse caso

- se a generalização for **separada**, uma `Pessoa` será `Aluno` ou `Monitor`, mas não os dois;
- se a generalização for **compartilhada**, a mesma `Pessoa` pode ser `Aluno` e `Monitor` ao mesmo tempo.

### Exemplo fácil de entender

- **separada**: `Veiculo` especializado em `Carro` e `Moto` em um cadastro simples;
- **compartilhada**: `Pessoa` especializada em `Aluno` e `Monitor` em um sistema academico.

### Regra prática

Se você está usando herança apenas porque duas classes possuem atributos parecidos, a modelagem provavelmente esta fraca.

### 4.2 Realizacao: quando uma classe concreta cumpre um contrato

Realizacao aparece quando uma classe concreta implementa um contrato, interface ou comportamento esperado.

Em POO, a diferença principal para a generalização e esta:

- generalização responde "e um tipo de";
- realizacao responde "cumpre este contrato".

``` mermaid
classDiagram
    class Operavel {
        +ligar() void
        +desligar() void
    }

    class BombaCirculacao {
        +ligar() void
        +desligar() void
    }

    Operavel <|.. BombaCirculacao
```

### Como ler

- `BombaCirculacao` realiza o contrato `Operavel`;
- isso comunica que a bomba oferece as operações exigidas por esse contrato;
- o foco aqui não e compartilhar estado, e sim garantir uma interface esperada.

### Exemplo prático

Se o sistema precisa operar vários equipamentos por um contrato comum, a realizacao ajuda a deixar isso explícito.

### 4.3 Associação: quando uma classe conhece, usa ou colabora com outra

Associação e a relacao mais comum.

Ela indica que existe uma ligacao estrutural entre classes, mas sem necessariamente dizer que uma controla o ciclo de vida da outra.

``` mermaid
classDiagram
    class Professor
    class Turma

    Professor "1" --> "1..*" Turma : leciona
```

### Como ler

- um `Professor` leciona uma ou várias `Turma`;
- existe colaboracao entre as classes;
- a relacao e importante para o domínio, mas uma classe não e "parte interna" da outra.

### Exemplo prático

`ControladorQualidade --> SensorPH` significa que o controlador usa o sensor para tomar decisão.

### 4.4 Agregacao: quando o todo agrupa partes que podem continuar existindo separadamente

Agregacao e uma associação com ideia de agrupamento.

Ela comunica que um objeto reune outros, mas as partes podem existir fora desse todo.

``` mermaid
classDiagram
    class Time
    class Jogador

    Time "1" o-- "0..*" Jogador : agrega
```

### Como ler

- um `Time` agrega zero ou muitos `Jogador`;
- os jogadores pertencem ao conjunto, mas ainda fazem sentido fora dele;
- o ciclo de vida das partes não depende totalmente do todo.

### Exemplo prático

Uma `Bancada` de laboratorio pode agregar equipamentos que depois são movidos para outra bancada.

### 4.5 Composição: quando a parte depende fortemente do todo

Composição e a relacao mais forte entre todo e parte.

Ela comunica que a parte faz sentido apenas dentro do todo modelado.

``` mermaid
classDiagram
    class Pedido
    class ItemPedido

    Pedido "1" *-- "1..*" ItemPedido : contem
```

### Como ler

- um `Pedido` contem um ou muitos `ItemPedido`;
- os itens existem como parte daquele pedido;
- se o pedido deixa de existir naquele modelo, os itens perdem sentido isoladamente.

### Exemplo prático

Uma `ETA` pode ser modelada em composição com `TanqueMistura` se o projeto quiser comunicar pertencimento estrutural forte.

### 4.6 Dependencia: quando uma classe apenas usa outra de forma pontual

Dependencia representa uma colaboracao mais leve e temporaria.

Ela costuma aparecer quando uma classe usa outra em um método, consulta um serviço ou depende de um resultado para executar uma tarefa.

``` mermaid
classDiagram
    class GeradorRelatorio
    class Medicao

    GeradorRelatorio ..> Medicao : usa para calcular
```

### Como ler

- `GeradorRelatorio` depende de `Medicao` para funcionar;
- a ligacao existe, mas não necessariamente como atributo permanente;
- costuma ser mais fraca que associação estrutural.

### Exemplo prático

Um exportador de resultados pode depender de `ResultadoEnsaio` apenas no momento da geração do arquivo.

### Tabela de leitura rápida

| Relacao | Notacao em Mermaid/UML | Como ler | Exemplo fácil |
|---|---|---|---|
| Generalizacao | `<|--` | e um tipo de | `SensorTemperatura` e um `AtivoIndustrial` |
| Realizacao | `<|..` | cumpre um contrato | `BombaCirculacao` realiza `Operavel` |
| Associação | `-->` | conhece, usa ou colabora com | `Professor` leciona `Turma` |
| Agregacao | `o--` | agrupa partes independentes | `Time` agrega `Jogador` |
| Composição | `*--` | contem partes fortemente dependentes | `Pedido` contem `ItemPedido` |
| Dependencia | `..>` | usa pontualmente | `GeradorRelatorio` usa `Medicao` |

### Regra de bolso para decidir

- se a relacao e `e um tipo de`, pense em **generalização**;
- se a relacao e `cumpre um contrato`, pense em **realizacao**;
- se a relacao e `conhece` ou `colabora com`, pense em **associação**;
- se a relacao e `agrupa`, pense em **agregação**;
- se a relacao e `contem` com dependencia forte, pense em **composição**;
- se a relacao e `usa momentaneamente`, pense em **dependencia**.

---

## 5. Cardinalidade (multiplicidade): quantos objetos podem se relacionar?

Um bom diagrama de classes não responde apenas "quem se conecta com quem". Ele também responde "em que quantidade essa relacao faz sentido?".

### Exemplo com cardinalidade

``` mermaid
classDiagram
    class Cliente
    class Pedido
    class ItemPedido
    class Produto

    Cliente "1" --> "0..*" Pedido : cria
    Pedido "1" *-- "1..*" ItemPedido : possui
    ItemPedido "*" --> "1" Produto : referência
```

### Como interpretar

- um `Cliente` pode criar zero ou muitos `Pedido`;
- cada `Pedido` possui um ou muitos `ItemPedido`;
- cada `ItemPedido` referência exatamente um `Produto`.

### Cardinalidades mais frequentes

| Notacao | Leitura | Exemplo de interpretacao |
|---|---|---|
| `1` | exatamente um | cada pedido tem um código |
| `0..1` | zero ou um | um equipamento pode ter ou não um controlador associado |
| `1..*` | um ou muitos | um tanque possui uma ou várias medicoes ao longo do tempo |
| `*` | muitos | um operador pode consultar muitos alarmes |

### O que a cardinalidade ajuda a decidir no código

- se um atributo deve guardar um objeto ou uma coleção;
- se a ausencia da relacao e aceitavel;
- se o construtor deve exigir um objeto associado;
- se a classe precisa validar quantidade mínima ou máxima de elementos.

### Erros comuns de leitura

- ignorar a cardinalidade e pensar apenas no tipo da relacao;
- desenhar `*` em tudo por inseguranca;
- usar `1` quando, na prática, a relacao pode não existir;
- esquecer que cardinalidade muda a implementação concreta.

---

## 6. Como sair de um texto de requisito para um diagrama de classes

Em sala, o aluno precisa aprender um método, não apenas decorar simbolos.

### Texto inicial

> Uma estação de monitoramento possui reservatórios. Cada reservatório recebe leituras de sensores. Um controlador analisa as leituras e pode gerar alarmes quando a faixa segura e violada.

### Passo 1. Destacar entidades candidatas

- estação de monitoramento;
- reservatório;
- sensor;
- controlador;
- alarme.

### Passo 2. Perguntar qual responsabilidade pertence a cada classe

- `Reservatorio` guarda capacidade e nível atual?
- `SensorNivel` mede e atualiza leitura?
- `ControladorNivel` avalia faixa segura e decide ação?
- `Alarme` representa um evento ou apenas uma mensagem solta?

### Passo 3. Decidir relações

- a estação contem reservatórios;
- um reservatório recebe leituras de sensores;
- o controlador usa sensores e pode emitir alarmes.

### Resultado inicial em Mermaid

``` mermaid
classDiagram
    class EstacaoMonitoramento {
        -nome: string
        +adicionarReservatorio() void
    }

    class Reservatorio {
        -codigo: string
        -capacidadeLitros: double
        -nivelAtual: double
        +atualizarNivel(valor: double) void
    }

    class SensorNivel {
        -tag: string
        -valorAtual: double
        +coletar() double
    }

    class ControladorNivel {
        +avaliarFaixa() bool
        +gerarAlarme() void
    }

    class Alarme {
        -mensagem: string
        -severidade: int
    }

    EstacaoMonitoramento "1" *-- "1..*" Reservatorio : contem
    Reservatorio "1" o-- "1..*" SensorNivel : monitora
    ControladorNivel --> "1..*" SensorNivel : le
    ControladorNivel --> "0..*" Alarme : gera
```

### Licao didatica importante

O primeiro diagrama raramente sai perfeito.

Ele deve ser tratado como **hipotese de modelagem**:

1. desenhar;
2. discutir;
3. simplificar ou corrigir;
4. so depois implementar.

---

## 7. Ponte C++ -> Python

O diagrama deve preservar o conceito. A linguagem concreta muda a forma de escrever.

### Mesmo modelo, mesma intencao

``` mermaid
classDiagram
    class Tanque {
        -codigo: string
        -capacidadeLitros: double
        -nivelAtual: double
        +atualizarNivel(valor: double) void
        +percentualOcupacao() double
    }
```

### Traducao para C++

```cpp
#include <stdexcept>
#include <string>

class Tanque {
private:
    std::string codigo;
    double capacidadeLitros;
    double nivelAtual;

public:
    Tanque(std::string codigoTanque, double capacidade)
        : codigo(codigoTanque), capacidadeLitros(capacidade), nivelAtual(0.0) {}

    void atualizarNivel(double valor) {
        if (valor < 0.0 || valor > capacidadeLitros) {
            throw std::out_of_range("nivel invalido");
        }
        nivelAtual = valor;
    }

    double percentualOcupacao() const {
        return (nivelAtual / capacidadeLitros) * 100.0;
    }
};
```

### Traducao para Python

```python
class Tanque:
    def __init__(self, codigo, capacidade_litros):
        self._codigo = codigo
        self._capacidade_litros = capacidade_litros
        self._nivel_atual = 0.0

    def atualizar_nivel(self, valor):
        if valor < 0 or valor > self._capacidade_litros:
            raise ValueError("nivel invalido")
        self._nivel_atual = valor

    def percentual_ocupacao(self):
        return (self._nivel_atual / self._capacidade_litros) * 100.0
```

### O que destacar em aula

| Aspecto | C++ | Python | Impacto didatico |
|---|---|---|---|
| Classe no código | `class Nome {}` | `class Nome:` | a ideia e a mesma |
| Visibilidade | `private`, `public`, `protected` | convencoes como `_atributo` e API pública | C++ explícita mais a fronteira |
| Operacoes | assinaturas tipadas | métodos mais flexiveis | Python reduz ruido, mas o modelo contínua |
| Regra de negócio | validação no método | validação no método | UML ajuda a ver a regra antes da sintaxe |

### Mensagem principal da ponte

O conceito de POO e mais estavel do que a sintaxe da linguagem.

Se o aluno entende o diagrama, ele consegue discutir o modelo em C++, Python ou outra linguagem orientada a objetos.

---

## 8. Como UML e usada na prática sem virar burocracia

Em projetos reais, diagrama de classes não deve ser tratado como decoracao de relatório. Ele funciona melhor quando aparece em momentos de decisão.

### Uso prático mais comum

1. alinhar entendimento do problema antes de implementar;
2. revisar responsabilidades quando uma classe está crescendo demais;
3. discutir composição versus herança em PR ou reuniao técnica;
4. explicar arquitetura de um modulo para novos membros da equipe;
5. apoiar refatoracao sem depender so de conversa abstrata.

### Quando o diagrama ajuda muito

- quando o requisito ainda está confuso;
- quando ha muitas classes colaborando;
- quando o time está discutindo responsabilidade errada;
- quando o código existente ficou procedural demais dentro de uma classe grande.

### Quando o diagrama atrapalha

- quando tenta espelhar cada detalhe da implementação;
- quando nunca e revisado apos mudança importante;
- quando substitui leitura de código em vez de apoiar a leitura;
- quando vira documento morto feito apenas para entrega.

### Regra prática para este curso

Desenhe o suficiente para responder três perguntas:

1. quais classes importam;
2. que responsabilidade cada uma assume;
3. como elas se relacionam.

Se o diagrama não ajuda nisso, ele está detalhado demais ou genérico demais.

---

## 9. Mini-caso prático: ETA com tanque, sensor e bomba dosadora

Imagine uma pequena estação de tratamento de agua onde o software precisa monitorar o pH do tanque de mistura e acionar uma bomba dosadora quando a faixa segura e violada.

### Modelo inicial

``` mermaid
classDiagram
    class ETA {
        -nome: string
    }

    class TanqueMistura {
        -codigo: string
        -volumeAtual: double
        +receberAgua(litros: double) void
    }

    class SensorPH {
        -tag: string
        -valorAtual: double
        +atualizarLeitura(valor: double) void
        +foraDaFaixa(min: double, max: double) bool
    }

    class BombaDosadora {
        -tag: string
        -vazaoLh: double
        +dosar(qtd: double) void
        +parar() void
    }

    class ControladorQualidade {
        +avaliarTanque() void
        +acionarCorrecao() void
    }

    ETA "1" *-- "1..*" TanqueMistura : contem
    TanqueMistura "1" o-- "1" SensorPH : monitora
    ControladorQualidade --> "1" SensorPH : le
    ControladorQualidade --> "1" BombaDosadora : aciona
```

### O que discutir com a turma

- por que `ETA` e `TanqueMistura` aparecem em composição;
- por que `SensorPH` foi representado como agregação, e não composição forte;
- por que `ControladorQualidade` usa `SensorPH` e `BombaDosadora`, mas não precisa herdar nada deles;
- que atributos devem ficar públicos, privados ou protegidos quando esse modelo virar código.

### Licao de modelagem

Observe como o diagrama obriga o aluno a separar papeis:

- sensor mede;
- tanque armazena;
- bomba atua;
- controlador decide.

Quando essa separação aparece cedo no desenho, o código tende a nascer com mais coesao.

---

## 10. Atividade da unidade: via GitHub Classroom

Nesta unidade, a atividade principal saiu do material do site e passou a existir como um **repositório starter independente**, próprio para uso no GitHub Classroom.

### Onde está a atividade

- Starter repo: [RafaelEmerick-POO/poo-20261-cenário-04-modelagem-uml](https://github.com/RafaelEmerick-POO/poo-20261-cenário-04-modelagem-uml)

### Por que a atividade foi movida

- para ensinar fluxo real com `assignment`, branch, issue, commit e pull request;
- para separar claramente **conteúdo de estudo** e **repositório de entrega**;
- para permitir que cada aluno ou equipe receba sua própria cópia pelo GitHub Classroom.

### O que o aluno vai encontrar no starter

- `README.md` com o enunciado completo;
- arquivos-base para o diagrama e para a implementação inicial;
- `AI_LOG.md`;
- template de issue;
- template de pull request;
- referências oficiais para Mermaid, Markdown e modelagem.

### Recomendação para o docente

Use esse starter como `assigned repository` no GitHub Classroom e mantenha esta página apenas como referência conceitual da aula.

---

## 11. Videos e materiais complementares

### Vídeo principal desta unidade

- [Tutorial de Diagramas de Classes UML](https://www.youtube.com/watch?v=rDidOn6KN9k)

### Leitura/laboratorio complementar

- [Mermaid Live Editor](https://mermaid.live/)
- [Material for MkDocs - Diagrams](https://squidfunk.github.io/mkdocs-material/reference/diagrams/)
- [Mermaid - Class diagrams](https://mermaid.js.org/syntax/classDiagram.html)
- [GitHub Docs - Basic writing and formatting syntax](https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax)
- [Python Docs - Classes](https://docs.python.org/3/tutorial/classes.html)
- [cppreference - Classes](https://en.cppreference.com/w/cpp/language/classes)
- [cppreference - Derived classes](https://en.cppreference.com/w/cpp/language/derived_class)

### Observação didatica

O vídeo ajuda a visualizar o mecanismo. A consolidacao real acontece quando o aluno pega um requisito curto, desenha o modelo, debate relações e so depois escreve o código.

---

## Perguntas de revisão rápida

1. Qual a diferença entre associação, agregação e composição em um diagrama de classes?
2. Como a multiplicidade ajuda a decidir se uma classe deve guardar um objeto ou uma coleção?
3. Em que situacoes um diagrama de classes melhora a modelagem antes da implementação?

## Fontes de referência

- https://www.omg.org/spec/UML/2.5.1/About-UML
- https://mermaid.live/
- https://mermaid.js.org/syntax/classDiagram.html
- https://squidfunk.github.io/mkdocs-material/reference/diagrams/
- https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax
- https://docs.python.org/3/tutorial/classes.html
- https://en.cppreference.com/w/cpp/language/classes
- https://en.cppreference.com/w/cpp/language/derived_class
