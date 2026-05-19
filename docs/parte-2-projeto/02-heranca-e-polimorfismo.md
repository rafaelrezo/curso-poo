# Aula Integrada - Listas Dinâmicas de Objetos, Herança e Polimorfismo

## Objetivos de aprendizagem

- Manipular listas dinâmicas de objetos em C++ usando `vector`, `unique_ptr`, `push_back` e `const auto&`.
- Comparar a mesma ideia em Python usando `list`, `append` e laços sobre objetos.
- Usar herança e polimorfismo para tratar objetos diferentes por uma interface comum.

**Tempo estimado:** 1h30.

## Vídeo da aula

![type:video](https://www.youtube.com/embed/wX2gozSqHfU)

---

## 1. Problema motivador

Um sistema supervisório raramente acompanha apenas uma variável. Ele pode receber dados de temperatura, umidade, corrente elétrica, pressão, nível, vazão ou qualquer outro sinal de processo.

O desafio de programação é simples de dizer, mas importante:

> Como guardar vários objetos diferentes em uma mesma coleção e percorrer todos eles de forma uniforme?

Em programação procedural, a tendência seria criar várias variáveis soltas:

```text
valorTemperatura
valorUmidade
valorCorrente
valorPressao
```

Em POO, a ideia é modelar objetos:

```text
SensorTemperatura
SensorUmidade        -> todos podem ser tratados como Sensor
SensorCorrente
SensorPressao
```

O ponto central da aula é a **lista dinâmica de objetos**. Herança e polimorfismo aparecem porque tornam essa lista mais flexível.

---

## 2. Conceito principal

Uma lista dinâmica cresce conforme o programa adiciona objetos. Em C++, usaremos `vector`. Em Python, usaremos `list`.

| Ideia | C++ | Python |
|---|---|---|
| Lista dinâmica | `vector` | `list` |
| Adicionar item | `push_back(...)` | `append(...)` |
| Percorrer itens | `for (const auto& item : lista)` | `for item in lista:` |
| Objeto base | classe com métodos `virtual` | classe base comum |
| Objeto especializado | classe derivada com `override` | subclasse com método sobrescrito |

Quando os objetos da lista possuem uma interface comum, o programa pode percorrê-los sem perguntar o tipo concreto de cada um.

---

## 3. Classe-base em C++

Este exemplo usa um contexto diferente da atividade prática: uma **sala técnica** com sensores de temperatura, umidade e corrente.

Bibliotecas necessárias:

- `<string>`: texto com `std::string`;
- `<vector>`: lista dinâmica;
- `<memory>`: ponteiros inteligentes com `std::unique_ptr` e `std::make_unique`;
- `<iostream>`: saída no terminal.

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

class Sensor {
protected:
    string tag;
    string unidade;
    double valorAtual;

public:
    Sensor(string tag, string unidade)
        : tag(tag), unidade(unidade), valorAtual(0.0) {}

    virtual ~Sensor() = default;

    virtual void simularLeitura() = 0;
    virtual string tipo() const = 0;

    string getTag() const {
        return tag;
    }

    string getUnidade() const {
        return unidade;
    }

    double getValorAtual() const {
        return valorAtual;
    }
};
```

### Leitura do código

`Sensor` é uma classe abstrata porque possui métodos virtuais puros:

```cpp
virtual void simularLeitura() = 0;
virtual string tipo() const = 0;
```

Isso significa que todo sensor concreto deve implementar essas operações. A lista poderá guardar sensores diferentes porque todos obedecem ao mesmo contrato.

---

## 4. Classes derivadas

```cpp
class SensorTemperatura : public Sensor {
public:
    SensorTemperatura(string tag) : Sensor(tag, "C") {}

    void simularLeitura() override {
        valorAtual = 32.5;
    }

    string tipo() const override {
        return "temperatura";
    }
};

class SensorUmidade : public Sensor {
public:
    SensorUmidade(string tag) : Sensor(tag, "%") {}

    void simularLeitura() override {
        valorAtual = 61.0;
    }

    string tipo() const override {
        return "umidade";
    }
};

class SensorCorrente : public Sensor {
public:
    SensorCorrente(string tag) : Sensor(tag, "A") {}

    void simularLeitura() override {
        valorAtual = 8.4;
    }

    string tipo() const override {
        return "corrente";
    }
};
```

Cada classe especializa a simulação e informa seu tipo. O `override` ajuda o compilador a verificar se o método realmente sobrescreve um método da classe-base.

---

## 5. Lista dinâmica em C++: `vector<unique_ptr<Sensor>>`

A declaração da lista é:

```cpp
vector<unique_ptr<Sensor>> sensores;
```

Leia por partes:

| Parte | Significado |
|---|---|
| `Sensor` | tipo base da hierarquia |
| `unique_ptr<Sensor>` | ponteiro inteligente para um objeto que se comporta como `Sensor` |
| `vector<...>` | lista dinâmica |
| `sensores` | nome da lista |

Usamos ponteiros porque a lista precisa guardar objetos derivados por meio do tipo base. Não usamos `vector<Sensor>` porque `Sensor` é abstrata e porque objetos derivados poderiam perder sua parte especializada ao serem copiados como objeto-base.

---

## 6. `push_back` e `make_unique`

```cpp
sensores.push_back(make_unique<SensorTemperatura>("TT-101"));
sensores.push_back(make_unique<SensorUmidade>("HT-201"));
sensores.push_back(make_unique<SensorCorrente>("IT-301"));
```

O que acontece em cada linha:

1. `make_unique<SensorTemperatura>("TT-101")` cria um objeto.
2. O objeto fica dentro de um `unique_ptr`.
3. `push_back(...)` adiciona esse ponteiro ao final do `vector`.
4. A lista passa a ter mais um sensor.

`unique_ptr` representa posse única. A lista passa a ser dona dos objetos. Quando a lista deixar de existir, os objetos são liberados automaticamente.

---

## 7. Percorrendo com `const auto&`

```cpp
for (const auto& sensor : sensores) {
    sensor->simularLeitura();

    cout << sensor->getTag() << " | "
         << sensor->tipo() << " | "
         << sensor->getValorAtual() << " "
         << sensor->getUnidade() << endl;
}
```

Parte por parte:

| Trecho | Explicação |
|---|---|
| `for (... : sensores)` | percorre todos os itens da lista |
| `auto` | deixa o compilador deduzir o tipo do item |
| `const` | indica que o ponteiro guardado na lista não será trocado no laço |
| `&` | evita copiar o `unique_ptr` |
| `sensor->` | acessa o objeto apontado pelo ponteiro inteligente |

O laço chama os mesmos métodos em todos os sensores. O comportamento concreto depende do objeto real. Isso é polimorfismo aplicado à lista dinâmica.

---

## 8. Exemplo C++ completo

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

class Sensor {
protected:
    string tag;
    string unidade;
    double valorAtual;

public:
    Sensor(string tag, string unidade)
        : tag(tag), unidade(unidade), valorAtual(0.0) {}

    virtual ~Sensor() = default;
    virtual void simularLeitura() = 0;
    virtual string tipo() const = 0;

    string getTag() const { return tag; }
    string getUnidade() const { return unidade; }
    double getValorAtual() const { return valorAtual; }
};

class SensorTemperatura : public Sensor {
public:
    SensorTemperatura(string tag) : Sensor(tag, "C") {}
    void simularLeitura() override { valorAtual = 32.5; }
    string tipo() const override { return "temperatura"; }
};

class SensorUmidade : public Sensor {
public:
    SensorUmidade(string tag) : Sensor(tag, "%") {}
    void simularLeitura() override { valorAtual = 61.0; }
    string tipo() const override { return "umidade"; }
};

class SensorCorrente : public Sensor {
public:
    SensorCorrente(string tag) : Sensor(tag, "A") {}
    void simularLeitura() override { valorAtual = 8.4; }
    string tipo() const override { return "corrente"; }
};

int main() {
    vector<unique_ptr<Sensor>> sensores;

    sensores.push_back(make_unique<SensorTemperatura>("TT-101"));
    sensores.push_back(make_unique<SensorUmidade>("HT-201"));
    sensores.push_back(make_unique<SensorCorrente>("IT-301"));

    for (const auto& sensor : sensores) {
        sensor->simularLeitura();

        cout << sensor->getTag() << " | "
             << sensor->tipo() << " | "
             << sensor->getValorAtual() << " "
             << sensor->getUnidade() << endl;
    }

    return 0;
}
```

---

## 9. A mesma ideia em Python

Python também permite criar objetos especializados e guardá-los em uma lista dinâmica.

```python
from abc import ABC, abstractmethod


class Sensor(ABC):
    def __init__(self, tag: str, unidade: str) -> None:
        self._tag = tag
        self._unidade = unidade
        self._valor_atual = 0.0

    @abstractmethod
    def simular_leitura(self) -> None:
        pass

    @abstractmethod
    def tipo(self) -> str:
        pass

    def como_dict(self) -> dict:
        return {
            "tag": self._tag,
            "tipo": self.tipo(),
            "valor": self._valor_atual,
            "unidade": self._unidade,
        }


class SensorTemperatura(Sensor):
    def __init__(self, tag: str) -> None:
        super().__init__(tag, "C")

    def simular_leitura(self) -> None:
        self._valor_atual = 32.5

    def tipo(self) -> str:
        return "temperatura"


class SensorUmidade(Sensor):
    def __init__(self, tag: str) -> None:
        super().__init__(tag, "%")

    def simular_leitura(self) -> None:
        self._valor_atual = 61.0

    def tipo(self) -> str:
        return "umidade"
```

### Lista dinâmica em Python

```python
sensores: list[Sensor] = []

sensores.append(SensorTemperatura("TT-101"))
sensores.append(SensorUmidade("HT-201"))

for sensor in sensores:
    sensor.simular_leitura()
    print(sensor.como_dict())
```

Comparação direta:

| Ação | C++ | Python |
|---|---|---|
| Criar lista dinâmica | `vector<unique_ptr<Sensor>> sensores;` | `sensores: list[Sensor] = []` |
| Criar objeto | `make_unique<SensorTemperatura>("TT-101")` | `SensorTemperatura("TT-101")` |
| Adicionar no fim | `push_back(...)` | `append(...)` |
| Percorrer lista | `for (const auto& sensor : sensores)` | `for sensor in sensores:` |
| Chamar método | `sensor->simularLeitura()` | `sensor.simular_leitura()` |

---

## 10. Perguntas de revisão rápida

1. Por que uma lista dinâmica é útil quando o número de sensores pode crescer?
2. Por que `vector<Sensor>` não é adequado para guardar objetos derivados?
3. Qual é a função de `unique_ptr` neste exemplo?
4. Qual é a diferença entre `push_back` em C++ e `append` em Python?

---

## Fontes de referência

- [cppreference - std::vector](https://en.cppreference.com/w/cpp/container/vector)
- [cppreference - std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr)
- [cppreference - virtual functions](https://en.cppreference.com/w/cpp/language/virtual)
- [Python Docs - Data Structures](https://docs.python.org/3/tutorial/datastructures.html)
- [Python Docs - abc](https://docs.python.org/3/library/abc.html)
