# Fundamentos de POO em C++: classes, objetos, metodos e construtores

## Objetivos de aprendizagem

- Entender o que `classe`, `objeto`, `metodo` e `construtor` resolvem na pratica.
- Ler exemplos simples em C++ com olhar critico, identificando estado, comportamento e riscos.
- Traduzir o mesmo conceito de C++ para Python sem perder o foco no modelo do problema.

**Tempo estimado:** 4h

## Video da aula

![type:video](https://www.youtube.com/embed/GHLFEhdk7Ys)

---

## 1. Antes de falar de sintaxe: por que POO importa aqui?

Quem veio de C ja sabe escrever passos. O novo desafio agora e organizar o programa em partes que facam sentido no dominio.

Para alunos de engenharia de controle e automacao, isso aparece o tempo todo:

- um `Sensor` tem estado: tag, unidade, valor lido;
- uma `Valvula` tem comportamento: abrir, fechar, limitar abertura;
- um `Reservatorio` precisa garantir regras: nivel nao pode ficar negativo.

Em vez de espalhar dados e funcoes pelo programa inteiro, a POO aproxima o codigo do equipamento, do processo ou do servico que estamos modelando.

**Ideia central desta aula:** classe nao existe para "deixar o codigo mais bonito". Classe existe para concentrar responsabilidade.

---

## 2. Roteiro base com W3Schools

Voce ja usou em sala os links da W3Schools. Nesta unidade eles viram o trilho principal de leitura e experimentacao.

| Secao | Leitura base | Link direto para testar | O que observar |
|---|---|---|---|
| Visao geral de OO | [C++ OOP](https://www.w3schools.com/cpp/cpp_oop.asp) | [Tutorial C++](https://www.w3schools.com/cpp/default.asp) | C++ suporta POO, mas continua sendo uma linguagem multiparadigma |
| Classe e objeto | [Classes/Objects](https://www.w3schools.com/cpp/cpp_classes.asp) | [Try: atributos em objeto](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_object_attr) | diferenca entre molde (`class`) e instancia (`obj`) |
| Metodos | [Class Methods](https://www.w3schools.com/cpp/cpp_class_methods.asp) | [Try: metodo na classe](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_object_method) | o metodo age sobre um objeto concreto |
| Construtores | [Constructors](https://www.w3schools.com/cpp/cpp_constructors.asp) | [Try: construtor simples](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_constructor) | o objeto nasce com uma configuracao inicial |
| Construtor com parametros | [Constructors](https://www.w3schools.com/cpp/cpp_constructors.asp) | [Try: construtor com parametros](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_constructor_param) | criar objeto pronto costuma ser melhor que "criar vazio e remendar depois" |

### Como usar esta trilha

1. Leia a secao na W3Schools.
2. Abra o `Try it Yourself`.
3. Rode o codigo sem alterar nada.
4. Mude nomes, valores e mensagens.
5. Responda: "o que esta guardado no objeto?" e "o que o metodo faz por esse objeto?".

---

## 3. Classe e objeto no contexto de engenharia

Uma forma simples de pensar:

- `classe` = especificacao do componente;
- `objeto` = componente concreto criado a partir dessa especificacao.

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

- A classe representa algo do mundo real ou e apenas um pacote solto de variaveis?
- O nome da classe ajuda a entender o dominio?
- O objeto nasce em estado valido ou qualquer valor serve?
- Se outro trecho do programa escrever `valor = -999`, quem impede?

### Primeira conclusao importante

Para comecar, atributos publicos ajudam a visualizar o conceito. Para manter software real, isso costuma ser insuficiente. Em breve voce vai estudar encapsulamento exatamente por causa desse limite.

---

## 4. Metodos: comportamento tambem pertence ao objeto

Se a classe so guarda dados, o risco e virar uma "estrutura com nome bonito". O proximo passo e colocar comportamento relevante dentro dela.

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

1. Esse metodo realmente pertence ao objeto ou deveria estar fora da classe?
2. O nome do metodo descreve uma acao observavel?
3. O metodo depende de dados que estao coerentes?
4. Se eu tiver 20 sensores, esse metodo ajuda a evitar repeticao?

### Quando faz sentido mover comportamento para a classe

- quando a acao depende do estado interno do objeto;
- quando a regra precisa acompanhar o dado;
- quando isso reduz repeticao e ambiguidade.

---

## 5. Construtores: o objeto deve nascer pronto

Uma classe melhora muito quando ela evita objetos "quebrados" logo na criacao.

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

### O ganho pratico

Sem construtor, o programa pode criar um objeto vazio e preencher depois.
Com construtor, o programa cria um objeto que ja nasce com identidade minima.

Para o aluno iniciante, a pergunta certa nao e "qual a sintaxe do construtor?".
A pergunta certa e:

**"Quais dados sao obrigatorios para esse objeto existir de forma coerente?"**

Essa pergunta melhora a modelagem.

---

## 6. Ponte C++ -> Python

O conceito e o mesmo. A diferenca principal, neste ponto do curso, e sintatica.

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
| Criacao da classe | `class Nome { ... };` | `class Nome:` | muda a forma, nao muda a ideia |
| Construtor | mesmo nome da classe | `__init__` | ambos inicializam o objeto |
| Chamada de metodo | `obj.metodo()` | `obj.metodo()` | o uso do objeto e muito parecido |
| Declaracao de atributos | mais explicita | mais flexivel | C++ ajuda a ver melhor o tipo; Python reduz ruido visual |

---

## 7. Pratica guiada: do exemplo da W3Schools para um problema real

### Etapa 1. Comece pelo exemplo pronto

Abra:

- [Try: atributos em objeto](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_object_attr)
- [Try: metodo na classe](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_object_method)
- [Try: construtor com parametros](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_constructor_param)

### Etapa 2. Troque o dominio

Substitua `MyClass` por `Sensor`.

Troque os campos genericos por:

- `tag`
- `unidade`
- `valor`

### Etapa 3. Adicione um comportamento

Crie um metodo `exibirLeitura()` que mostre:

```text
TQ-01 = 26.4 C
```

### Etapa 4. Force um estado inicial

Agora remova a criacao "vazia" do objeto e use construtor:

```cpp
Sensor sensorSala("TQ-01", "C", 26.4);
```

### Etapa 5. Faca a revisao do proprio codigo

Antes de considerar a atividade pronta, responda:

1. O nome da classe representa um objeto do dominio?
2. O objeto pode nascer sem informacao essencial?
3. O metodo criado usa dados do proprio objeto?
4. Algum atributo aceitaria valores absurdos sem nenhuma regra?
5. O codigo esta facil de ler para um colega?

---

## 8. Leitura critica de codigo: nao aceite qualquer classe

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

### O que esta bom

- o nome da classe faz sentido;
- existe uma tentativa de centralizar a alteracao do valor.

### O que esta fraco

- `tag` continua exposta e pode ser alterada livremente;
- `setValor()` nao valida nada;
- o codigo nao deixa claro qual faixa de valor e aceitavel;
- qualquer objeto pode nascer sem `tag`;
- a classe ainda esta muito proxima de um pacote de dados.

### O pensamento critico que queremos formar

O aluno nao deve olhar para o codigo e perguntar apenas:

**"Compila?"**

Ele deve perguntar tambem:

- faz sentido para o dominio?
- protege o estado do objeto?
- facilita manutencao?
- comunica a intencao?

Essa habilidade de leitura sera mais importante do que apenas gerar codigo.

---

## 9. Mini-caso pratico

Uma bancada de teste possui tres componentes:

- `SensorTemperatura`
- `Valvula`
- `Reservatorio`

Escolha um deles e modele:

1. quais dados minimos ele precisa guardar;
2. qual metodo simples ele deve oferecer;
3. quais dados devem ser obrigatorios no construtor.

**Desafio de revisao:** troque seu codigo com um colega e peca que ele responda:

- o objeto nasce coerente?
- o nome dos membros esta claro?
- o metodo representa uma acao real?
- falta alguma regra de negocio?

---

## 10. Videos e cursos ilustrativos

### Video principal desta unidade

- [UNIVESP - Programacao Orientada a Objetos: Classes e Objetos](https://www.youtube.com/watch?v=GHLFEhdk7Ys)

### Video complementar curto para construtores em C++

- [Tutorial C++ - Construtores (MrAppleBR)](https://www.youtube.com/watch?v=lkaDJMwwGXw)

### Trilha externa opcional

- [W3Schools C++ Tutorial](https://www.w3schools.com/cpp/default.asp)
- [UNIVESP - Apresentacao da disciplina de POO](https://www.youtube.com/watch?v=tHXO_j4RfGw)

Observacao didatica: o video da UNIVESP ajuda no conceito. A W3Schools entra como laboratorio rapido porque permite editar e executar exemplos no navegador.

---

## Perguntas de revisao rapida

1. Qual a diferenca pratica entre `classe` e `objeto`?
2. Por que um `metodo` pode ser melhor do que uma funcao solta em alguns casos?
3. O que um construtor bem pensado evita no inicio da vida do objeto?

## Fontes de referencia

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
