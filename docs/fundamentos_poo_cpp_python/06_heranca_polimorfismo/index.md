# Contratos e variação: herança e polimorfismo

## Objetivos de aprendizagem

- Reconhecer quando diferentes dispositivos podem cumprir um mesmo contrato.
- Aplicar herança pública e polimorfismo dinâmico em um exemplo pequeno de C++.
- Comparar o contrato explícito de C++ com o polimorfismo por comportamento em Python.

**Tempo estimado:** 4h, em dois encontros de 2h.

## Vídeo da aula

![type:video](https://www.youtube.com/embed/wX2gozSqHfU)

---

## 1. Qual problema apareceu?

O controlador precisa adquirir nível, pressão e vazão. Uma solução frágil espalha decisões pelo tipo:

```cpp
if (tipo == "nivel") { /* ... */ }
else if (tipo == "pressao") { /* ... */ }
```

O que existe em comum? Todo sensor deve informar uma tag, adquirir uma leitura e dizer seu tipo. O algoritmo concreto pode variar.

---

## 2. Contrato em C++

```cpp
#include <string>

class Sensor {
public:
    virtual ~Sensor() = default;
    virtual void adquirir() = 0;
    virtual double valor() const = 0;
    virtual std::string tipo() const = 0;
};
```

Uma função virtual pura termina em `= 0`. A classe representa um contrato e não um sensor concreto. O destrutor virtual permite destruir corretamente objetos derivados por meio do tipo-base.

```cpp
class SensorNivel : public Sensor {
private:
    double valor_{40.0};

public:
    void adquirir() override { valor_ += 1.0; }
    double valor() const override { return valor_; }
    std::string tipo() const override { return "nivel"; }
};

class SensorPressao : public Sensor {
private:
    double valor_{2.0};

public:
    void adquirir() override { valor_ += 0.1; }
    double valor() const override { return valor_; }
    std::string tipo() const override { return "pressao"; }
};
```

`override` pede ao compilador que confirme a correspondência com o contrato.

---

## 3. Onde está o polimorfismo?

```cpp
void executarAquisicao(Sensor& sensor) {
    sensor.adquirir();
    std::cout << sensor.tipo() << ": " << sensor.valor() << '\n';
}
```

A função conhece apenas `Sensor&`. O objeto concreto decide qual implementação executar. Isso é polimorfismo dinâmico.

Não use ainda uma coleção de ponteiros. Neste capítulo entra apenas uma referência por vez; a coleção será a necessidade do capítulo 07.

---

## 4. Herança ou composição?

| Técnica | Melhor uso | Esforço | Entregável | Limitação |
|---|---|---:|---|---|
| composição | relação “tem um” ou política substituível | médio | objetos colaboradores | exige desenhar interfaces |
| herança pública | relação “é um” com substituição válida | médio/alto | família com contrato comum | aumenta acoplamento |
| função parametrizada | variação pequena de cálculo | baixo | comportamento configurável | pode não representar identidade de tipo |

Recomendação: mantenha `Controlador tem Sensores`; use herança apenas para dizer que `SensorNivel é um Sensor`.

---

## 5. Ponte C++ -> Python

```python
class SensorNivel:
    def __init__(self) -> None:
        self._valor = 40.0

    def adquirir(self) -> None:
        self._valor += 1.0

    @property
    def valor(self) -> float:
        return self._valor

    @property
    def tipo(self) -> str:
        return "nivel"


def executar_aquisicao(sensor) -> None:
    sensor.adquirir()
    print(f"{sensor.tipo}: {sensor.valor}")
```

Python permite polimorfismo por comportamento: se o objeto cumpre as operações usadas, a função pode trabalhar com ele. Uma classe-base abstrata ou `Protocol` pode tornar o contrato mais explícito, mas não é necessária neste primeiro exemplo.

---

## 6. Validação e prática profissional

Teste separadamente:

1. `SensorNivel` altera sua leitura;
2. `SensorPressao` altera sua leitura;
3. a mesma função recebe os dois tipos;
4. remover `override` não deve ser a “correção” de uma assinatura errada.

Fluxo sugerido:

```text
issue -> cap06-contrato-sensores -> teste local -> push -> CI -> PR
```

No PR, responda: “qual operação o código cliente consegue executar sem descobrir o tipo concreto?”

---

## 7. Mini-caso prático

Dois sensores já podem passar pela mesma função, um por vez. O controlador ainda não consegue cadastrar uma quantidade variável deles. Essa limitação prepara coleções dinâmicas e responsabilidade sobre memória.

---

## Perguntas de revisão rápida

1. Que promessa a classe-base `Sensor` faz ao código cliente?
2. Qual é o papel de `virtual` e `override` no exemplo?
3. Por que `Controlador` não deve herdar de `Sensor`?

## Fontes de referência

- [cppreference — virtual functions](https://en.cppreference.com/w/cpp/language/virtual)
- [C++ Core Guidelines — hierarquias](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-class)
- [Python Docs — classes](https://docs.python.org/3/tutorial/classes.html)
- [W3Schools — C++ Polymorphism](https://www.w3schools.com/cpp/cpp_polymorphism.asp)
- [W3Schools — Python Inheritance](https://www.w3schools.com/python/python_inheritance.asp)
