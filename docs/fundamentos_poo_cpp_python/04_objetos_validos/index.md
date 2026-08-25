# Objetos válidos: construtores, encapsulamento e invariantes

## Objetivos de aprendizagem

- Explicar encapsulamento como proteção de regras, e não apenas como uso de `private`.
- Construir objetos válidos e controlar alterações de estado em C++ e Python.
- Verificar casos comuns, limites e entradas inválidas com testes reproduzíveis.

**Tempo estimado:** 4h, em dois encontros de 2h.

## Vídeo da aula

![type:video](https://www.youtube.com/embed/pONnilIFY64)

---

## 1. Qual problema apareceu?

O capítulo anterior permite isto:

```cpp
SensorNivel sensor{"LT-101", -30.0};
sensor.valor = 180.0;
```

O programa compila, mas o objeto não representa uma leitura percentual possível. O problema não é somente “atributo público”: falta uma regra que permaneça verdadeira durante toda a vida do objeto.

Uma **invariante** é uma condição que o objeto se compromete a preservar. Para este sensor:

```text
0.0 <= valor <= 100.0
```

---

## 2. Ideia que resolve: uma fronteira para o estado

```cpp
#include <stdexcept>
#include <string>

class SensorNivel {
private:
    std::string tag_;
    double valor_;

public:
    SensorNivel(std::string tag, double valorInicial)
        : tag_(tag), valor_(0.0) {
        atualizar(valorInicial);
    }

    void atualizar(double novoValor) {
        if (novoValor < 0.0 || novoValor > 100.0) {
            throw std::out_of_range("nivel fora da faixa de 0 a 100");
        }
        valor_ = novoValor;
    }

    const std::string& tag() const { return tag_; }
    double valor() const { return valor_; }
};
```

O construtor inicia o objeto. O método `atualizar` concentra a regra. Os métodos de consulta permitem observar o estado sem abri-lo para alteração direta.

### Conceito -> impacto -> ação

| Conceito | Impacto | Ação recomendada |
|---|---|---|
| estado privado | reduz alterações arbitrárias | exponha operações com significado |
| construtor | evita objeto parcialmente configurado | exija os dados indispensáveis |
| invariante | mantém coerência ao longo do tempo | valide em toda porta de entrada |
| método `const` | declara que a consulta não altera o objeto | use em operações somente de leitura |

---

## 3. Como confirmar antes de avançar?

```cpp
#include <cassert>

SensorNivel sensor{"LT-101", 0.0};
assert(sensor.valor() == 0.0);

sensor.atualizar(100.0);
assert(sensor.valor() == 100.0);
```

Casos mínimos:

| Caso | Entrada | Resultado esperado |
|---|---:|---|
| comum | `42.5` | aceita |
| limite inferior | `0.0` | aceita |
| limite superior | `100.0` | aceita |
| inválido | `-0.1` | rejeita e preserva estado anterior |

O tratamento completo da exceção será estudado no capítulo 09. Aqui ela apenas torna explícito que a operação não conseguiu cumprir o contrato.

---

## 4. Ponte C++ -> Python

```python
class SensorNivel:
    def __init__(self, tag: str, valor_inicial: float):
        self._tag = tag
        self._valor = 0.0
        self.atualizar(valor_inicial)

    def atualizar(self, novo_valor: float) -> None:
        if not 0.0 <= novo_valor <= 100.0:
            raise ValueError("nível fora da faixa de 0 a 100")
        self._valor = novo_valor

    @property
    def tag(self) -> str:
        return self._tag

    @property
    def valor(self) -> float:
        return self._valor
```

Python não impõe `private` como C++. O sublinhado comunica uso interno, propriedades controlam a leitura e os testes verificam o contrato. Encapsulamento continua sendo uma decisão de projeto.

---

## 5. Sobrecarga: técnica opcional, não objetivo central

C++ permite vários construtores com assinaturas diferentes. Use isso somente quando cada forma representar um nascimento válido e claro:

```cpp
SensorNivel(std::string tag) : SensorNivel(tag, 0.0) {}
```

Python não oferece sobrecarga real de `__init__` com várias definições. Parâmetros padrão ou métodos de classe nomeados costumam expressar as alternativas.

| Técnica | Melhor uso | Esforço | Entregável | Limitação |
|---|---|---:|---|---|
| construtor único | dados obrigatórios claros | baixo | objeto válido | menos formas de criação |
| sobrecarga em C++ | poucas origens de criação bem distintas | médio | APIs alternativas | pode criar ambiguidade |
| fábrica nomeada | origem precisa ser explícita | médio | `a_partir_de_configuracao` | mais métodos para documentar |

Para esta etapa, prefira um construtor principal simples.

---

## 6. Prática profissional

1. Parta do `SensorNivel` do capítulo 03.
2. Crie `cap04-estado-valido`.
3. Faça primeiro o teste de entrada inválida falhar.
4. Torne o estado privado e implemente `atualizar`.
5. Rode casos comum, limites e inválido.
6. Faça push e leia o relatório do CI.
7. Abra PR explicando qual invariante passou a ser preservada.

### Erros comuns

- validar somente no construtor e esquecer atualizações posteriores;
- criar um setter para cada atributo sem perguntar qual operação existe no domínio;
- alterar o estado antes de terminar a validação;
- capturar a exceção no mesmo método apenas para ignorá-la.

---

## 7. Mini-caso prático

O sensor agora rejeita nível impossível. Porém ele ainda trabalha sozinho. No próximo capítulo, um `Controlador` coordenará um sensor e uma bomba por composição.

---

## Perguntas de revisão rápida

1. Qual é a diferença entre colocar um atributo em `private` e preservar uma invariante?
2. Por que a validação deve ocorrer no construtor e nas operações de atualização?
3. Quando uma fábrica nomeada comunica melhor a intenção que uma sobrecarga?

## Fontes de referência

- [C++ Core Guidelines — classes](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-class)
- [cppreference — constructors](https://en.cppreference.com/w/cpp/language/constructor)
- [Python Docs — classes](https://docs.python.org/3/tutorial/classes.html)
- [W3Schools — C++ Encapsulation](https://www.w3schools.com/cpp/cpp_encapsulation.asp)
- [W3Schools — Python Classes](https://www.w3schools.com/python/python_classes.asp)
