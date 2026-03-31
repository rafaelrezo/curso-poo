# UML e Diagrama de Classes: modelando objetos antes do codigo

## Objetivos de aprendizagem

- Entender como UML ajuda a representar classes, atributos, operacoes e relacoes de forma util para POO.
- Ler e construir diagramas de classes com foco em responsabilidade, visibilidade, multiplicidade e tipo de relacionamento.
- Traduzir um diagrama de classes para decisoes concretas em C++ e Python sem confundir modelo com sintaxe.

**Tempo estimado:** 4h

## Video da aula

![type:video](https://www.youtube.com/embed/rDidOn6KN9k)

---

## 1. Por que UML entra agora na trilha?

A turma ja viu `classe`, `objeto`, `metodo`, `construtor`, encapsulamento, heranca e polimorfismo como ideias de POO. O proximo passo natural e aprender a **pensar o modelo antes de escrever o codigo**.

E aqui entra a UML.

UML nao substitui implementacao. UML tambem nao existe para produzir desenho bonito. Em um curso de POO, ela serve para um objetivo muito mais util:

- deixar explicitas as classes importantes do dominio;
- mostrar quem conhece quem;
- registrar responsabilidades e limites de acesso;
- discutir erros de modelagem antes de gastar tempo com codigo.

**Ideia central desta aula:** diagrama de classes e uma ferramenta de raciocinio. Ele ajuda a validar a estrutura do sistema antes que a sintaxe esconda os problemas de modelagem.

---

## 2. O que e UML e o que um diagrama de classes realmente mostra?

A UML e uma linguagem padrao de modelagem usada para visualizar e especificar a estrutura de sistemas de software. Dentro dela, o diagrama de classes e um dos artefatos mais importantes para POO porque ele mostra a **estrutura estatica** do sistema:

- classes;
- atributos;
- operacoes;
- relacionamentos;
- multiplicidades;
- visibilidade e abstracao.

### Classe UML mais simples possivel

``` mermaid
classDiagram
    class Sensor
```

Esse primeiro desenho quase nao diz nada. Ele apenas informa que `Sensor` e uma classe relevante no modelo.

### Classe UML com atributos e operacoes

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

### Como ler esse retangulo

- parte superior: nome da classe;
- parte central: atributos ou estado;
- parte inferior: operacoes ou comportamento;
- simbolos como `+`, `-` e `#`: nivel de visibilidade.

### Leitura didatica da visibilidade

| Notacao UML | Significado | Relacao com POO | Pergunta pratica |
|---|---|---|---|
| `+` | publico | faz parte do contrato externo | o restante do sistema realmente precisa chamar isso? |
| `-` | privado | protege detalhe interno | esse dado precisa ser mexido de fora? |
| `#` | protegido | extensao por heranca | subclasses precisam mesmo enxergar isso? |
| `<<abstract>>` | classe abstrata | contrato parcial | faz sentido existir objeto direto dessa classe? |

### Ponto importante para a turma

Diagrama de classes **nao e codigo executavel**. Ele simplifica.

Por isso, o diagrama nao precisa listar cada getter, cada setter ou cada detalhe de biblioteca. O foco deve ficar no que ajuda a entender o modelo.

---

## 3. Como o diagrama de classes se relaciona com POO?

O diagrama de classes nao e um assunto paralelo a POO. Ele e uma forma de **explicitar visualmente** os mesmos conceitos que depois aparecem no codigo.

| Conceito de POO | Como aparece no diagrama | O que isso ajuda a decidir |
|---|---|---|
| Classe | retangulo com nome | quais entidades sao centrais no dominio |
| Estado | atributos | o que cada objeto precisa guardar |
| Comportamento | operacoes | que acoes pertencem ao objeto |
| Encapsulamento | visibilidade `+`, `-`, `#` | o que fica exposto e o que fica protegido |
| Heranca | generalizacao | quando uma classe realmente e um subtipo de outra |
| Composicao/agregacao | relacoes entre classes | como objetos colaboram sem virar uma massa de codigo |
| Polimorfismo | abstracao e especializacao | que contrato comum diferentes objetos podem respeitar |

### Exemplo curto: o diagrama como ponte para encapsulamento e heranca

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

### O que esse desenho ja comunica antes do codigo

- `SensorPressao` e `BombaRecalque` compartilham uma base comum;
- a classe-base concentra identidade e informacao operacional comum;
- cada derivada tem estado e comportamento especificos;
- a estrutura sugere uma relacao `is-a`, nao apenas reaproveitamento oportunista.

**Leitura critica:** se a unica semelhanca entre duas classes for ter `tag`, isso ainda nao justifica heranca.

---

## 4. Relacionamentos UML: quando usar cada um

O maior erro dos iniciantes costuma ser este: desenhar classes isoladas, mas errar o tipo de relacao entre elas.

Em POO, boa parte da qualidade da modelagem depende menos do nome das classes e mais de **como elas se conectam**.

### Exemplo com relacoes diferentes

``` mermaid
classDiagram
    class Planta
    class Tanque
    class SensorNivel
    class Controlador
    class HistoricoLeituras

    Planta *-- "1..*" Tanque : contem
    Tanque o-- "0..*" SensorNivel : agrega
    Controlador --> "1" Tanque : regula
    Controlador ..> HistoricoLeituras : consulta
```

### Comparacao de relacionamentos

| Tecnica/Padrao | Melhor uso | Esforco | Entregavel | Limitacao |
|---|---|---|---|---|
| Associacao | quando duas classes colaboram de forma direta | baixo | ligacao clara entre objetos | nao explica propriedade nem ciclo de vida |
| Agregacao | quando um objeto agrupa outros que podem existir separadamente | medio | nocao de conjunto ou colecao | costuma ser confundida com associacao simples |
| Composicao | quando a parte depende fortemente do todo | medio | relacao forte de pertencimento | exige mais cuidado com ciclo de vida |
| Generalizacao | quando existe relacao real `is-a` | medio | hierarquia de classes coerente | vira erro estrutural se usada so para reaproveitar codigo |
| Dependencia | quando uma classe apenas usa outra pontualmente | baixo | acoplamento mais leve e temporario | diz menos sobre estrutura duradoura |

### Recomendacao pratica por cenario

- use **associacao** quando um objeto conhece ou chama outro;
- use **agregacao** quando existe agrupamento, mas as partes podem continuar existindo fora do todo;
- use **composicao** quando a parte faz sentido apenas dentro do todo modelado;
- use **generalizacao** quando a pergunta "isso e um tipo de..." faz sentido no dominio;
- use **dependencia** para colaboracoes temporarias, parametros de metodo ou servicos auxiliares.

### Erro classico de modelagem

Muitos alunos tentam resolver tudo com heranca. Isso costuma produzir hierarquias artificiais.

Regra de bolso:

- se a relacao e `tem um`, pense primeiro em composicao ou agregacao;
- se a relacao e `usa`, pense em associacao ou dependencia;
- se a relacao e `e um tipo de`, so entao investigue heranca.

---

## 5. Multiplicidade: quantos objetos podem se relacionar?

Um bom diagrama de classes nao responde apenas "quem se conecta com quem". Ele tambem responde "em que quantidade essa relacao faz sentido?".

### Exemplo com multiplicidade

``` mermaid
classDiagram
    class Cliente
    class Pedido
    class ItemPedido
    class Produto

    Cliente "1" --> "0..*" Pedido : cria
    Pedido "1" *-- "1..*" ItemPedido : possui
    ItemPedido "*" --> "1" Produto : referencia
```

### Como interpretar

- um `Cliente` pode criar zero ou muitos `Pedido`;
- cada `Pedido` possui um ou muitos `ItemPedido`;
- cada `ItemPedido` referencia exatamente um `Produto`.

### Multiplicidades mais frequentes

| Notacao | Leitura | Exemplo de interpretacao |
|---|---|---|
| `1` | exatamente um | cada pedido tem um codigo |
| `0..1` | zero ou um | um equipamento pode ter ou nao um controlador associado |
| `1..*` | um ou muitos | um tanque possui uma ou varias medicoes ao longo do tempo |
| `*` | muitos | um operador pode consultar muitos alarmes |

### Impacto pratico no codigo

Multiplicidade bem pensada ajuda a decidir:

- se um atributo deve ser singular ou colecao;
- se a ausencia da relacao e aceitavel;
- se o construtor deve exigir um objeto associado;
- se ha necessidade de validacao para evitar estado invalido.

---

## 6. Como sair de um texto de requisito para um diagrama de classes

Em sala, o aluno precisa aprender um metodo, nao apenas decorar simbolos.

### Texto inicial

> Uma estacao de monitoramento possui reservatorios. Cada reservatorio recebe leituras de sensores. Um controlador analisa as leituras e pode gerar alarmes quando a faixa segura e violada.

### Passo 1. Destacar entidades candidatas

- estacao de monitoramento;
- reservatorio;
- sensor;
- controlador;
- alarme.

### Passo 2. Perguntar qual responsabilidade pertence a cada classe

- `Reservatorio` guarda capacidade e nivel atual?
- `SensorNivel` mede e atualiza leitura?
- `ControladorNivel` avalia faixa segura e decide acao?
- `Alarme` representa um evento ou apenas uma mensagem solta?

### Passo 3. Decidir relacoes

- a estacao contem reservatorios;
- um reservatorio recebe leituras de sensores;
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
| Classe no codigo | `class Nome {}` | `class Nome:` | a ideia e a mesma |
| Visibilidade | `private`, `public`, `protected` | convencoes como `_atributo` e API publica | C++ explicita mais a fronteira |
| Operacoes | assinaturas tipadas | metodos mais flexiveis | Python reduz ruido, mas o modelo continua |
| Regra de negocio | validacao no metodo | validacao no metodo | UML ajuda a ver a regra antes da sintaxe |

### Mensagem principal da ponte

O conceito de POO e mais estavel do que a sintaxe da linguagem.

Se o aluno entende o diagrama, ele consegue discutir o modelo em C++, Python ou outra linguagem orientada a objetos.

---

## 8. Como UML e usada na pratica sem virar burocracia

Em projetos reais, diagrama de classes nao deve ser tratado como decoracao de relatorio. Ele funciona melhor quando aparece em momentos de decisao.

### Uso pratico mais comum

1. alinhar entendimento do problema antes de implementar;
2. revisar responsabilidades quando uma classe esta crescendo demais;
3. discutir composicao versus heranca em PR ou reuniao tecnica;
4. explicar arquitetura de um modulo para novos membros da equipe;
5. apoiar refatoracao sem depender so de conversa abstrata.

### Quando o diagrama ajuda muito

- quando o requisito ainda esta confuso;
- quando ha muitas classes colaborando;
- quando o time esta discutindo responsabilidade errada;
- quando o codigo existente ficou procedural demais dentro de uma classe grande.

### Quando o diagrama atrapalha

- quando tenta espelhar cada detalhe da implementacao;
- quando nunca e revisado apos mudanca importante;
- quando substitui leitura de codigo em vez de apoiar a leitura;
- quando vira documento morto feito apenas para entrega.

### Regra pratica para este curso

Desenhe o suficiente para responder tres perguntas:

1. quais classes importam;
2. que responsabilidade cada uma assume;
3. como elas se relacionam.

Se o diagrama nao ajuda nisso, ele esta detalhado demais ou generico demais.

---

## 9. Mini-caso pratico: ETA com tanque, sensor e bomba dosadora

Imagine uma pequena estacao de tratamento de agua onde o software precisa monitorar o pH do tanque de mistura e acionar uma bomba dosadora quando a faixa segura e violada.

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

- por que `ETA` e `TanqueMistura` aparecem em composicao;
- por que `SensorPH` foi representado como agregacao, e nao composicao forte;
- por que `ControladorQualidade` usa `SensorPH` e `BombaDosadora`, mas nao precisa herdar nada deles;
- que atributos devem ficar publicos, privados ou protegidos quando esse modelo virar codigo.

### Licao de modelagem

Observe como o diagrama obriga o aluno a separar papeis:

- sensor mede;
- tanque armazena;
- bomba atua;
- controlador decide.

Quando essa separacao aparece cedo no desenho, o codigo tende a nascer com mais coesao.

---

## 10. Atividade pratica final: modelar e implementar um nucleo minimo

### Enunciado

Modele um **sistema de monitoramento de laboratorio de ensaios** com pelo menos estas classes:

- `Laboratorio`
- `Equipamento`
- `SensorTemperatura`
- `OrdemCalibracao`
- uma classe adicional escolhida por voce

### Regras da atividade

- o diagrama deve ser feito em `mermaid`;
- use no minimo `5` classes;
- inclua pelo menos `1` relacao de generalizacao;
- inclua pelo menos `1` relacao de composicao ou agregacao;
- explicite multiplicidade em pelo menos `3` relacoes;
- cada classe principal deve ter ao menos `2` atributos e `1` operacao.

### Tarefas

1. Escrever um pequeno texto de requisito com `5` a `8` linhas.
2. Extrair as classes candidatas.
3. Montar o diagrama de classes em `mermaid`.
4. Justificar em comentario curto por que cada relacao foi escolhida.
5. Implementar o esqueleto de `2` classes em C++ ou Python.
6. Fazer `commit` com uma mensagem objetiva, por exemplo: `feat: modela laboratorio com diagrama UML inicial`.

### Entregaveis esperados

- um arquivo `.md` com o texto do problema e o bloco `mermaid`;
- um arquivo `.cpp` ou `.py` com pelo menos duas classes do modelo;
- uma justificativa curta dizendo onde houve composicao, heranca e associacao.

### Checklist de revisao

- as classes representam entidades do dominio e nao apenas telas ou menus;
- os nomes das operacoes indicam comportamento observavel;
- ha pelo menos uma decisao clara de encapsulamento;
- a heranca usada representa mesmo uma relacao `is-a`;
- o diagrama ajuda a prever a implementacao, e nao o contrario.

### Desafio opcional

Transforme uma relacao inicialmente desenhada como heranca em composicao e explique qual versao ficou melhor para o dominio escolhido.

---

## 11. Videos e materiais complementares

### Video principal desta unidade

- [Tutorial de Diagramas de Classes UML](https://www.youtube.com/watch?v=rDidOn6KN9k)

### Leitura/laboratorio complementar

- [Material for MkDocs - Diagrams](https://squidfunk.github.io/mkdocs-material/reference/diagrams/)
- [Mermaid - Class diagrams](https://mermaid.js.org/syntax/classDiagram.html)
- [Python Docs - Classes](https://docs.python.org/3/tutorial/classes.html)
- [cppreference - Classes](https://en.cppreference.com/w/cpp/language/classes)
- [cppreference - Derived classes](https://en.cppreference.com/w/cpp/language/derived_class)

### Observacao didatica

O video ajuda a visualizar o mecanismo. A consolidacao real acontece quando o aluno pega um requisito curto, desenha o modelo, debate relacoes e so depois escreve o codigo.

---

## Perguntas de revisao rapida

1. Qual a diferenca entre associacao, agregacao e composicao em um diagrama de classes?
2. Como a multiplicidade ajuda a decidir se uma classe deve guardar um objeto ou uma colecao?
3. Em que situacoes um diagrama de classes melhora a modelagem antes da implementacao?

## Fontes de referencia

- https://www.omg.org/spec/UML/2.5.1/About-UML
- https://mermaid.js.org/syntax/classDiagram.html
- https://squidfunk.github.io/mkdocs-material/reference/diagrams/
- https://docs.python.org/3/tutorial/classes.html
- https://en.cppreference.com/w/cpp/language/classes
- https://en.cppreference.com/w/cpp/language/derived_class
