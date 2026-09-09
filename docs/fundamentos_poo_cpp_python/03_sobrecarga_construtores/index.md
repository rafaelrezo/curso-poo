# Construtores e sobrecarga: diferentes formas de iniciar o sensor

## Objetivos de aprendizagem

- Explicar como C++ seleciona um construtor pelos argumentos e distinguir sobrecarga de argumento padrão.
- Usar delegação para oferecer diferentes formas de construção preservando as invariantes.
- Interpretar exemplos completos de construção em C++ e Python e explicar seus resultados.

**Tempo estimado:** 2h de aula expositiva, com leitura comentada dos programas e discussão das saídas. Esta aula vem após [Encapsulamento](../04_objetos_validos/index.md) e prepara [Composição](../05_composicao/index.md).

## Vídeo da aula

![type:video](https://www.youtube.com/embed/lkaDJMwwGXw)

Retome o vídeo de construtores utilizado na versão anterior desta aula. Observe o momento em que o estado inicial é definido. A sobrecarga e a delegação serão desenvolvidas nos experimentos abaixo.

---

## 1. Sobrecarga: veja a classe e quem a utiliza

O cliente conhece somente a tag em uma situação e conhece tag e leitura em outra. A classe precisa atender às duas formas de criação.

- **Sobrecarga:** mais de um construtor, com listas de parâmetros diferentes.
- **Assinatura:** quantidade e tipos dos parâmetros distinguem as alternativas deste exemplo.
- **Seleção:** o compilador escolhe o construtor compatível com os argumentos da chamada.

A leitura começa pelo `main`: há três objetos criados por duas formas de construção. As declarações na classe mostram qual construtor atende a cada um. A validação retoma a aula 04; o foco novo são as duas formas de construir.

Neste exemplo, o sensor guarda identificação e leitura. Quando o valor é omitido, a simulação começa com 50%; quando informado, preserva esse valor, inclusive zero. Os 50% são uma configuração didática, não uma medição presumida de equipamento real. [Programa completo para consulta](exemplo_01_sobrecarga.cpp).

```cpp
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

class SensorNivel {
    std::string tag_;
    double valor_;

    void validar() const {
        if (tag_.empty() || !std::isfinite(valor_) || valor_ < 0 || valor_ > 100) {
            throw std::invalid_argument("tag vazia ou nivel fora de 0..100");
        }
    }

public:
    explicit SensorNivel(std::string tag)
        : tag_(tag), valor_(50.0) {
        validar();
    }

    SensorNivel(std::string tag, double valor)
        : tag_(tag), valor_(valor) {
        validar();
    }

    const std::string& tag() const { return tag_; }
    double valor() const { return valor_; }
};

int main() {
    SensorNivel usual{"LT-101"};              // um argumento
    SensorNivel informado{"LT-102", 42.5};    // dois argumentos
    SensorNivel zero{"LT-104", 0.0};          // zero foi informado

    std::cout << usual.tag() << ": " << usual.valor() << " %\n";
    std::cout << informado.tag() << ": " << informado.valor() << " %\n";
    std::cout << zero.tag() << ": " << zero.valor() << " %\n";
}
```

Saída esperada:

```text
LT-101: 50 %
LT-102: 42.5 %
LT-104: 0 %
```

**Leitura do resultado:**

- **`usual`:** um argumento; recebe a leitura usual de 50%.
- **`informado`:** dois argumentos; preserva 42,5%.
- **`zero`:** dois argumentos; zero informado continua zero.
- **Resolução de sobrecarga:** considera as assinaturas e, em casos gerais, as conversões possíveis.

### 1.1 Por que escrever `explicit`?

**`explicit` impede que o construtor seja usado nessa conversão automática de texto em sensor.**

- **Sem `explicit`:** uma função que espera `SensorNivel` pode receber uma `std::string`; o construtor cria o objeto implicitamente.
- **Com `explicit`:** o programador precisa indicar a criação, como em `SensorNivel{nome}`.

O programa completo abaixo mostra o caso **sem `explicit`**. [Arquivo para consulta](exemplo_05_sem_explicit.cpp).

```cpp
#include <iostream>
#include <stdexcept>
#include <string>

class SensorNivel {
    std::string tag_;
public:
    // Sem explicit: permite converter uma std::string em SensorNivel.
    SensorNivel(std::string tag) : tag_(tag) {
        if (tag.empty()) throw std::invalid_argument("tag vazia");
    }

    const std::string& tag() const { return tag_; }
};

void imprimir(SensorNivel sensor) {
    std::cout << sensor.tag() << '\n';
}

int main() {
    std::string nome = "LT-101";
    imprimir(nome); // a funcao espera SensorNivel, mas recebe uma string
}
```

Saída: `LT-101`. A chamada funciona porque o construtor converte `nome` no objeto esperado por `imprimir`.

| Construtor | Chamada | Resultado |
|---|---|---|
| Sem `explicit` | `imprimir(nome)` | aceita: conversão automática |
| Com `explicit` | `imprimir(nome)` | rejeitada pelo compilador |
| Com `explicit` | `imprimir(SensorNivel{nome})` | aceita: criação indicada no código |

**Ponto-chave:** `explicit` controla a conversão; a validação dos dados continua sendo responsabilidade do construtor.

### 1.2 Como ler a declaração do construtor

No primeiro programa da seção 1, com tag e leitura, observe:

- **`SensorNivel` é o nome do construtor**, igual ao da classe. Ele participa da criação do objeto e não declara tipo de retorno: não se escreve `void`, `int` ou `SensorNivel` antes de seu nome como retorno.
- **`std::string tag` é o parâmetro recebido**. No `main`, esse parâmetro recebe a identificação fornecida entre chaves.
- **Após `:`, `tag_(tag)` inicializa o atributo `tag_` com o parâmetro `tag`**, e `valor_(50.0)` inicializa a leitura. O sublinhado é uma convenção de nomes usada aqui para distinguir atributos de parâmetros.
- **O corpo entre chaves executa depois da inicialização dos atributos**. Neste exemplo, ele verifica se a identificação está vazia.

- **Ordem efetiva:** segue a declaração dos atributos na classe: `tag_` antes de `valor_`.
- **Legibilidade:** a lista de inicialização acompanha essa mesma ordem.
- **Significado de `:`:** aqui introduz os inicializadores do construtor; não declara herança.

**Verificação rápida:** na chamada que passa só o texto, quem criaria o sensor se `explicit` fosse removido? E onde a leitura de 50% seria definida?


**Qual limitação apareceu?** Os dois construtores repetem a inicialização dos membros e a chamada de validação. A regra está em um método só, mas cada caminho ainda precisa lembrar de chamá-lo. Vamos encaminhar uma construção à outra.

---

## 2. Delegação: o mesmo programa com uma construção central

A delegação centraliza o trabalho de construção.

- **Origem:** o construtor com apenas a tag encaminha `tag` e `50.0`.
- **Destino:** o construtor de dois argumentos inicializa os atributos e valida os dados.
- **Ordem:** o destino termina antes da execução do corpo do construtor que delegou.
- **Benefício:** as duas formas de criação percorrem a mesma proteção.

Essa é a ordem definida para [construtores delegados em C++](https://eel.is/c++draft/class.base.init).

O exemplo mantém a mesma identificação, leitura e chamadas do primeiro programa. A única mudança conceitual é centralizar a construção: a forma de um argumento delega à de dois.

[Programa completo de delegação para consulta](exemplo_02_delegacao.cpp).

```cpp
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

class SensorNivel {
    std::string tag_;
    double valor_;

public:
    explicit SensorNivel(std::string tag) : SensorNivel(tag, 50.0) {}

    SensorNivel(std::string tag, double valor) : tag_(tag), valor_(valor) {
        if (tag.empty() || !std::isfinite(valor) || valor < 0 || valor > 100) {
            throw std::invalid_argument("tag vazia ou nivel fora de 0..100");
        }
    }

    const std::string& tag() const { return tag_; }
    double valor() const { return valor_; }
};

int main() {
    SensorNivel usual{"LT-101"};              // um argumento
    SensorNivel informado{"LT-102", 42.5};    // dois argumentos
    SensorNivel zero{"LT-104", 0.0};          // zero foi informado

    std::cout << usual.tag() << ": " << usual.valor() << " %\n";
    std::cout << informado.tag() << ": " << informado.valor() << " %\n";
    std::cout << zero.tag() << ": " << zero.valor() << " %\n";
}
```

Saída esperada:

```text
LT-101: 50 %
LT-102: 42.5 %
LT-104: 0 %
```

**Explique o caminho:** a criação de `usual` entra na forma de um argumento, encaminha a leitura usual, inicializa os dois atributos e valida os dados. `informado` entra diretamente no destino. Ambos percorrem a mesma proteção.

A construção inválida continua sendo rejeitada; não substitua silenciosamente uma leitura inválida por 50. O lançamento protege a construção; a [seção 10 — Exceções](../11_excecoes/index.md) desenvolverá o tratamento pelo cliente.


**Pontos de atenção:**

- **Delegação:** ocorre na lista após `:`.
- **Criação dentro do corpo:** produz outro objeto; não inicializa o objeto atual.
- **Ciclo de delegação:** um construtor não pode retornar a si mesmo, direta ou indiretamente.

---

## 3. Ponte C++ → Python: classe completa, um único inicializador

O conceito permanece: permitir omitir a leitura e preservar o valor informado.

- **Um inicializador:** Python usa um único `__init__` neste exemplo.
- **Argumento padrão:** fornece 50 quando a leitura é omitida.
- **Valor explícito:** prevalece sobre o padrão, inclusive quando é zero.
- **Duas definições de `__init__`:** a segunda substitui a primeira; não há seleção por assinatura como no C++.

O padrão `50.0` atende à criação com apenas a tag; as outras duas chamadas no `main` preservam as leituras informadas. [Programa Python completo para consulta](exemplo_03_construcao.py).

```python
from math import isfinite


class SensorNivel:
    def __init__(self, tag: str, valor: float = 50.0):
        if not tag or not isfinite(valor) or not 0 <= valor <= 100:
            raise ValueError("tag vazia ou nivel fora de 0..100")
        self._tag = tag
        self._valor = valor

    @property
    def tag(self):
        return self._tag

    @property
    def valor(self):
        return self._valor


def main():
    usual = SensorNivel("LT-101")
    informado = SensorNivel("LT-102", 42.5)
    zero = SensorNivel("LT-104", 0.0)
    print(f"{usual.tag}: {usual.valor:g} %")
    print(f"{informado.tag}: {informado.valor:g} %")
    print(f"{zero.tag}: {zero.valor:g} %")


if __name__ == "__main__":
    main()
```

Saída esperada:

```text
LT-101: 50 %
LT-102: 42.5 %
LT-104: 0 %
```


O [argumento padrão](https://docs.python.org/3/tutorial/controlflow.html#default-argument-values) é usado quando o argumento é omitido. Uma expressão como `valor or 50.0` confundiria zero com ausência e quebraria o resultado de `zero`.

**E em C++?** Também é possível resolver o caso simples com um único construtor de tag e leitura, atribuindo `50.0` como argumento padrão da leitura. Seria uma assinatura com parâmetro opcional, em vez das duas assinaturas demonstradas. Não combine essa alternativa com a sobrecarga de um argumento: a chamada apenas com tag ficaria ambígua.

| Técnica/Padrão | Melhor uso | Esforço | Entregável | Limitação |
|---|---|---|---|---|
| Sobrecarga C++ | formas distintas de construção | médio | assinaturas específicas | conversões e padrões podem gerar ambiguidade |
| Delegação C++ | compartilhar inicialização entre construtores | baixo | uma regra central de construção | exige evitar ciclos |
| Argumento padrão C++/Python | mesmo fluxo com um valor usual | baixo | uma assinatura com argumento opcional | não expressa sozinho intenções muito diferentes |

Para um valor usual simples, argumento padrão é suficiente. Sobrecarga oferece assinaturas distintas; delegação permite compartilhar a construção entre elas. A escolha depende das formas de criação que o modelo precisa expressar.

---

## 4. Dúvidas frequentes sobre a construção

| Dúvida | Explicação |
|---|---|
| Zero informado deve virar 50? | Não. O padrão só é usado quando o argumento é omitido. |
| Pode combinar sobrecarga de um argumento e padrão na de dois? | A chamada apenas com tag fica ambígua; há duas alternativas compatíveis. |
| Chamar outro construtor dentro do corpo inicializa o objeto atual? | Não. Isso cria outro objeto; delegação ocorre na lista após `:`. |
| `explicit` valida a leitura? | Não. Ele restringe a conversão implícita; a validação continua no construtor. |

---

## 5. O que isso prepara?

Agora o aluno sabe criar um objeto por caminhos distintos e manter seu contrato. Na [aula 06 — Composição](../05_composicao/index.md), um controlador precisará inicializar seus próprios componentes. Na herança, veremos a inicialização da classe-base.

- **Sobrecarga:** oferece formas diferentes de chamar; não exige hierarquia.
- **Delegação:** encaminha a construção para outro construtor da mesma classe.
- **Herança:** acrescentará a inicialização da parte-base.
- **Polimorfismo dinâmico:** selecionará uma implementação virtual conforme o objeto.
- **Construtores:** não são virtuais nem são sobrescritos com `override`.

A UML continuará encerrando a Parte 1, após esses fundamentos.

### Aprofundamento do material anterior

O [exemplo de pirâmide de automação](exemplo_piramide_automacao.cpp) foi preservado como leitura opcional: compare as formas mínima e completa de `SensorAnalogico`. Ele usa ajustes automáticos de faixa e valor; o contrato desta aula rejeita entradas inválidas. Explique essa diferença antes de reaproveitar sua implementação. É uma leitura complementar, fora do tempo de exposição previsto.

## Perguntas de revisão rápida

1. Qual a diferença entre dois construtores sobrecarregados e um único construtor com argumento padrão? Identifique cada caso no C++ e no Python desta aula.
2. Por que delegar pelo inicializador preserva a validação, enquanto escrever uma chamada de construtor dentro do corpo não inicializa o objeto atual?
3. Por que passar uma string a uma função que recebe SensorNivel pode compilar sem explicit e ser rejeitado com explicit?

## Fontes de referência

- [Rascunho público do padrão C++ — construtores](https://eel.is/c++draft/class.ctor).
- [Rascunho público do padrão C++ — inicialização e delegação](https://eel.is/c++draft/class.base.init).
- [C++ Core Guidelines — C.42, construção de objetos válidos](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-throw).
- [C++ Core Guidelines — C.51, construtores delegados](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-delegating).
- [Python Docs — argumentos padrão](https://docs.python.org/3/tutorial/controlflow.html#default-argument-values).
- [Python Docs — classes](https://docs.python.org/3/tutorial/classes.html).
