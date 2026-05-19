# Fundamentos de POO em C++: classes, objetos, métodos e construtores

## Objetivos de aprendizagem

- Entender o que `classe`, `objeto`, `método` e `construtor` resolvem na prática.
- Ler exemplos simples em C++ com olhar critico, identificando estado, comportamento e riscos.
- Traduzir o mesmo conceito de C++ para Python sem perder o foco no modelo do problema.

**Tempo estimado:** 4h

## Vídeo da aula

![type:video](https://www.youtube.com/embed/GHLFEhdk7Ys)

---

## 1. Antes de falar de sintaxe: por que POO importa aqui?

Quem veio de C já sabe escrever passos. O novo desafio agora e organizar o programa em partes que facam sentido no domínio.

Para alunos de engenharia de controle e automação, isso aparece o tempo todo:

- um `Sensor` tem estado: tag, unidade, valor lido;
- uma `Valvula` tem comportamento: abrir, fechar, limitar abertura;
- um `Reservatorio` precisa garantir regras: nível não pode ficar negativo.

Em vez de espalhar dados e funções pelo programa inteiro, a POO aproxima o código do equipamento, do processo ou do serviço que estamos modelando.

**Ideia central desta aula:** classe não existe para "deixar o código mais bonito". Classe existe para concentrar responsabilidade.

---

## 2. Roteiro base com W3Schools

Você já usou em sala os links da W3Schools. Nesta unidade eles viram o trilho principal de leitura é experimentacao.

| Secao | Leitura base | Link direto para testar | O que observar |
|---|---|---|---|
| Visão geral de OO | [C++ OOP](https://www.w3schools.com/cpp/cpp_oop.asp) | [Tutorial C++](https://www.w3schools.com/cpp/default.asp) | C++ suporta POO, mas contínua sendo uma linguagem multiparadigma |
| Classe e objeto | [Classes/Objects](https://www.w3schools.com/cpp/cpp_classes.asp) | [Try: atributos em objeto](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_object_attr) | diferença entre molde (`class`) e instancia (`obj`) |
| Metodos | [Class Methods](https://www.w3schools.com/cpp/cpp_class_methods.asp) | [Try: método na classe](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_object_method) | o método age sobre um objeto concreto |
| Construtores | [Constructors](https://www.w3schools.com/cpp/cpp_constructors.asp) | [Try: construtor simples](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_constructor) | o objeto nasce com uma configuração inicial |
| Construtor com parâmetros | [Constructors](https://www.w3schools.com/cpp/cpp_constructors.asp) | [Try: construtor com parâmetros](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_constructor_param) | criar objeto pronto costuma ser melhor que "criar vazio e remendar depois" |

### Como usar esta trilha

1. Leia a secao na W3Schools.
2. Abra o `Try it Yourself`.
3. Rode o código sem alterar nada.
4. Mude nomes, valores e mensagens.
5. Responda: "o que esta guardado no objeto?" e "o que o método faz por esse objeto?".

---

## 3. Classe e objeto no contexto de engenharia

Uma forma simples de pensar:

- `classe` = especificação do componente;
- `objeto` = componente concreto criado a partir dessa especificação.

### Exemplo em C++

```cpp
#include <iostream>
#include <string>
using namespace std;

class Sensor {
public:
    string tag;
    string unidade;
    double valor;
};

int main() {
    Sensor sensorSala;
    sensorSala.tag = "TQ-01";
    sensorSala.unidade = "C";
    sensorSala.valor = 26.4;

    cout << sensorSala.tag << " = "
         << sensorSala.valor << " "
         << sensorSala.unidade << "\n";
    return 0;
}
```

### O que ler criticamente aqui

- A classe representa algo do mundo real ou e apenas um pacote solto de variáveis?
- O nome da classe ajuda a entender o domínio?
- O objeto nasce em estado válido ou qualquer valor serve?
- Se outro trecho do programa escrever `valor = -999`, quem impede?

### Primeira conclusao importante

Para começar, atributos públicos ajudam a visualizar o conceito. Para manter software real, isso costuma ser insuficiente. Em breve você vai estudar encapsulamento exatamente por causa desse limite.

---

## 4. Metodos: comportamento também pertence ao objeto

Se a classe so guarda dados, o risco e virar uma "estrutura com nome bonito". O próximo passo e colocar comportamento relevante dentro dela.

### Exemplo em C++

```cpp
#include <iostream>
#include <string>
using namespace std;

class Sensor {
public:
    string tag;
    double valor;

    void exibirLeitura() {
        cout << tag << " -> " << valor << "\n";
    }
};

int main() {
    Sensor sensorSala;
    sensorSala.tag = "TQ-01";
    sensorSala.valor = 26.4;
    sensorSala.exibirLeitura();
    return 0;
}
```

### Leitura critica

Perguntas que o aluno deve aprender a fazer:

1. Esse método realmente pertence ao objeto ou deveria estar fora da classe?
2. O nome do método descreve uma ação observavel?
3. O método depende de dados que estao coerentes?
4. Se eu tiver 20 sensores, esse método ajuda a evitar repeticao?

### Quando faz sentido mover comportamento para a classe

- quando a ação depende do estado interno do objeto;
- quando a regra precisa acompanhar o dado;
- quando isso reduz repeticao e ambiguidade.

---

## 5. Construtores: o objeto deve nascer pronto

Uma classe melhora muito quando ela evita objetos "quebrados" logo na criação.

### Exemplo em C++

```cpp
#include <iostream>
#include <string>
using namespace std;

class Sensor {
public:
    string tag;
    string unidade;
    double valor;

    Sensor(string novaTag, string novaUnidade, double valorInicial) {
        tag = novaTag;
        unidade = novaUnidade;
        valor = valorInicial;
    }

    void exibirLeitura() {
        cout << tag << " = " << valor << " " << unidade << "\n";
    }
};

int main() {
    Sensor sensorSala("TQ-01", "C", 26.4);
    sensorSala.exibirLeitura();
    return 0;
}
```

### O ganho prático

Sem construtor, o programa pode criar um objeto vazio e preencher depois.
Com construtor, o programa cria um objeto que já nasce com identidade mínima.

Para o aluno iniciante, a pergunta certa não e "qual a sintaxe do construtor?".
A pergunta certa e:

**"Quais dados são obrigatórios para esse objeto existir de forma coerente?"**

Essa pergunta melhora a modelagem.

---

## 6. Ponte C++ -> Python

O conceito e o mesmo. A diferença principal, neste ponto do curso, e sintatica.

### Mesmo conceito em C++

```cpp
#include <iostream>
#include <string>
using namespace std;

class Valvula {
public:
    string tag;
    double abertura;

    Valvula(string novaTag, double aberturaInicial) {
        tag = novaTag;
        abertura = aberturaInicial;
    }

    void exibirEstado() {
        cout << tag << " -> " << abertura << "%\n";
    }
};

int main() {
    Valvula v1("XV-02", 35.0);
    v1.exibirEstado();
    return 0;
}
```

### Mesmo conceito em Python

```python
class Valvula:
    def __init__(self, tag, abertura):
        self.tag = tag
        self.abertura = abertura

    def exibir_estado(self):
        print(f"{self.tag} -> {self.abertura}%")


v1 = Valvula("XV-02", 35.0)
v1.exibir_estado()
```

### O que destacar em aula

| Aspecto | C++ | Python | Impacto didatico |
|---|---|---|---|
| Criação da classe | `class Nome { ... };` | `class Nome:` | muda a forma, não muda a ideia |
| Construtor | mesmo nome da classe | `__init__` | ambos inicializam o objeto |
| Chamada de método | `obj.método()` | `obj.método()` | o uso do objeto e muito parecido |
| Declaracao de atributos | mais explícita | mais flexivel | C++ ajuda a ver melhor o tipo; Python reduz ruido visual |

---

## 7. Pratica guiada: do exemplo da W3Schools para um problema real

### Etapa 1. Comece pelo exemplo pronto

Abra:

- [Try: atributos em objeto](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_object_attr)
- [Try: método na classe](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_object_method)
- [Try: construtor com parâmetros](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_constructor_param)

### Etapa 2. Troque o domínio

Substitua `MyClass` por `Sensor`.

Troque os campos genéricos por:

- `tag`
- `unidade`
- `valor`

### Etapa 3. Adicione um comportamento

Crie um método `exibirLeitura()` que mostre:

```text
TQ-01 = 26.4 C
```

### Etapa 4. Force um estado inicial

Agora remova a criação "vazia" do objeto e use construtor:

```cpp
Sensor sensorSala("TQ-01", "C", 26.4);
```

### Etapa 5. Faca a revisão do próprio código

Antes de considerar a atividade pronta, responda:

1. O nome da classe representa um objeto do domínio?
2. O objeto pode nascer sem informacao essencial?
3. O método criado usa dados do próprio objeto?
4. Algum atributo aceitaria valores absurdos sem nenhuma regra?
5. O código está fácil de ler para um colega?

---

## 8. Leitura critica de código: não aceite qualquer classe

Leia este exemplo:

```cpp
class Sensor {
public:
    string tag;
    double valor;

    void setValor(double v) {
        valor = v;
    }
};
```

### O que está bom

- o nome da classe faz sentido;
- existe uma tentativa de centralizar a alteração do valor.

### O que está fraco

- `tag` contínua exposta e pode ser alteráda livremente;
- `setValor()` não válida nada;
- o código não deixa claro qual faixa de valor e aceitavel;
- qualquer objeto pode nascer sem `tag`;
- a classe ainda está muito próxima de um pacote de dados.

### O pensamento critico que queremos formar

O aluno não deve olhar para o código e perguntar apenas:

**"Compila?"**

Ele deve perguntar também:

- faz sentido para o domínio?
- protege o estado do objeto?
- facilita manutenção?
- comunica a intencao?

Essa habilidade de leitura será mais importante do que apenas gerar código.

---

## 9. Mini-caso prático

Uma bancada de teste possui três componentes:

- `SensorTemperatura`
- `Valvula`
- `Reservatorio`

Escolha um deles e modele:

1. quais dados mínimos ele precisa guardar;
2. qual método simples ele deve oferecer;
3. quais dados devem ser obrigatórios no construtor.

**Desafio de revisão:** troque seu código com um colega e peca que ele responda:

- o objeto nasce coerente?
- o nome dos membros está claro?
- o método representa uma ação real?
- falta alguma regra de negócio?

---

## 10. Videos e cursos ilustrativos

### Vídeo principal desta unidade

- [UNIVESP - Programação Orientada a Objetos: Classes e Objetos](https://www.youtube.com/watch?v=GHLFEhdk7Ys)

### Vídeo complementar curto para construtores em C++

- [Tutorial C++ - Construtores (MrAppleBR)](https://www.youtube.com/watch?v=lkaDJMwwGXw)

### Trilha externa opcional

- [W3Schools C++ Tutorial](https://www.w3schools.com/cpp/default.asp)
- [UNIVESP - Apresentação da disciplina de POO](https://www.youtube.com/watch?v=tHXO_j4RfGw)

Observação didatica: o vídeo da UNIVESP ajuda no conceito. A W3Schools entra como laboratorio rápido porque permite editar e executar exemplos no navegador.

---

## Perguntas de revisão rápida

1. Qual a diferença prática entre `classe` e `objeto`?
2. Por que um `método` pode ser melhor do que uma função solta em alguns casos?
3. O que um construtor bem pensado evita no início da vida do objeto?

## Fontes de referência

- https://www.w3schools.com/cpp/cpp_oop.asp
- https://www.w3schools.com/cpp/cpp_classes.asp
- https://www.w3schools.com/cpp/cpp_class_methods.asp
- https://www.w3schools.com/cpp/cpp_constructors.asp
- https://www.w3schools.com/cpp/trycpp.asp?filename=demo_object_attr
- https://www.w3schools.com/cpp/trycpp.asp?filename=demo_object_method
- https://www.w3schools.com/cpp/trycpp.asp?filename=demo_constructor
- https://www.w3schools.com/cpp/trycpp.asp?filename=demo_constructor_param
- https://en.cppreference.com/w/cpp/language/class
- https://en.cppreference.com/w/cpp/language/constructor
- https://docs.python.org/3/tutorial/classes.html
