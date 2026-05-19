# Aula Integrada - Objetos da Lista com Estado Protegido

## Objetivos de aprendizagem

- Aplicar encapsulamento em objetos armazenados em listas dinâmicas.
- Validar valores antes de aceitar uma leitura dentro do objeto.
- Montar uma lista de leituras confiáveis para exibição no supervisor.

**Tempo estimado:** parte da mesma aula integrada.

## Vídeo da aula

![type:video](https://www.youtube.com/embed/wX2gozSqHfU)

---

## 1. Por que validar objetos dentro da lista?

Na página anterior, a lista dinâmica guardava objetos de sensores diferentes. Agora o problema é outro: a lista pode estar bem estruturada, mas os objetos dentro dela podem guardar valores inválidos.

Exemplos:

- temperatura impossível para o ambiente monitorado;
- umidade abaixo de `0%` ou acima de `100%`;
- corrente elétrica negativa;
- status escrito de várias formas diferentes.

O objetivo é usar encapsulamento e validação como apoio à manipulação da lista. A ênfase continua sendo a lista de objetos.

---

## 2. Encapsulamento aplicado

Encapsular significa controlar como o estado interno muda.

Em vez de permitir isto:

```cpp
sensor.valorAtual = -500.0;
```

o objeto deve oferecer um método que verifica a regra:

```cpp
sensor.atualizarValor(32.5);
```

| Conceito | Uso na lista de objetos |
|---|---|
| Atributo protegido | impede escrita direta fora da classe |
| Método de atualização | concentra a validação |
| Faixa mínima e máxima | define o que é leitura aceitável |
| Status | informa se a leitura está normal ou em falha |

---

## 3. Classe-base com validação em C++

O exemplo continua usando a sala técnica da página anterior.

```cpp
#include <stdexcept>
#include <string>
using namespace std;

enum class StatusLeitura {
    Operando,
    Falha
};

string statusParaTexto(StatusLeitura status) {
    switch (status) {
        case StatusLeitura::Operando:
            return "operando";
        case StatusLeitura::Falha:
            return "falha";
    }
    return "falha";
}

class Sensor {
protected:
    string tag;
    string unidade;
    double valorAtual;
    double minimo;
    double maximo;
    StatusLeitura status;

    void atualizarValor(double valor) {
        if (valor < minimo || valor > maximo) {
            status = StatusLeitura::Falha;
            throw out_of_range("Leitura fora da faixa");
        }

        valorAtual = valor;
        status = StatusLeitura::Operando;
    }

public:
    Sensor(string tag, string unidade, double minimo, double maximo)
        : tag(tag),
          unidade(unidade),
          valorAtual(0.0),
          minimo(minimo),
          maximo(maximo),
          status(StatusLeitura::Operando) {}

    virtual ~Sensor() = default;

    virtual void simularLeitura() = 0;
    virtual string tipo() const = 0;

    string getTag() const { return tag; }
    string getUnidade() const { return unidade; }
    double getValorAtual() const { return valorAtual; }
    StatusLeitura getStatus() const { return status; }
};
```

O método `atualizarValor` fica protegido. Assim, as classes derivadas podem usá-lo, mas o restante do programa não altera o valor sem passar pela validação.

---

## 4. Classes derivadas com faixas próprias

```cpp
class SensorTemperatura : public Sensor {
public:
    SensorTemperatura(string tag) : Sensor(tag, "C", -10.0, 80.0) {}

    void simularLeitura() override {
        atualizarValor(32.5);
    }

    string tipo() const override {
        return "temperatura";
    }
};

class SensorUmidade : public Sensor {
public:
    SensorUmidade(string tag) : Sensor(tag, "%", 0.0, 100.0) {}

    void simularLeitura() override {
        atualizarValor(61.0);
    }

    string tipo() const override {
        return "umidade";
    }
};

class SensorCorrente : public Sensor {
public:
    SensorCorrente(string tag) : Sensor(tag, "A", 0.0, 50.0) {}

    void simularLeitura() override {
        atualizarValor(8.4);
    }

    string tipo() const override {
        return "corrente";
    }
};
```

Cada sensor continua sendo tratado como `Sensor` na lista, mas cada classe possui seus próprios limites.

---

## 5. Percorrendo a lista com tratamento de erro

```cpp
vector<unique_ptr<Sensor>> sensores;

sensores.push_back(make_unique<SensorTemperatura>("TT-101"));
sensores.push_back(make_unique<SensorUmidade>("HT-201"));
sensores.push_back(make_unique<SensorCorrente>("IT-301"));

for (const auto& sensor : sensores) {
    try {
        sensor->simularLeitura();

        cout << sensor->getTag() << " | "
             << sensor->tipo() << " | "
             << sensor->getValorAtual() << " "
             << sensor->getUnidade() << " | "
             << statusParaTexto(sensor->getStatus()) << endl;
    } catch (const out_of_range& erro) {
        cout << sensor->getTag()
             << " | falha | " << erro.what() << endl;
    }
}
```

Esse laço pratica ao mesmo tempo:

- lista dinâmica;
- polimorfismo;
- validação;
- tratamento de erro;
- leitura uniforme de objetos diferentes.

---

## 6. Transformando objetos em dados para o supervisor

Depois de simular e validar, o dispositivo pode transformar cada objeto em uma estrutura textual ou JSON.

Exemplo conceitual de saída:

```json
[
  {
    "tag": "TT-101",
    "tipo": "temperatura",
    "valor": 32.5,
    "unidade": "C",
    "status": "operando"
  },
  {
    "tag": "HT-201",
    "tipo": "umidade",
    "valor": 61.0,
    "unidade": "%",
    "status": "operando"
  }
]
```

O supervisor não precisa conhecer a classe C++ concreta. Ele precisa receber campos consistentes.

---

## 7. Validação da lista em Python

No lado Python, a lista recebida também deve ser validada antes de virar tabela ou gráfico.

```python
STATUS_VALIDOS = {"operando", "alerta", "falha", "manutencao"}


def validar_leitura(leitura: dict) -> None:
    obrigatorios = {"tag", "tipo", "valor", "unidade", "status"}
    faltantes = obrigatorios - set(leitura)

    if faltantes:
        raise ValueError(f"Campos ausentes: {sorted(faltantes)}")

    if not isinstance(leitura["tag"], str):
        raise TypeError("tag deve ser texto")

    if leitura["valor"] is not None and not isinstance(leitura["valor"], int | float):
        raise TypeError("valor deve ser numérico ou nulo")

    if leitura["status"] not in STATUS_VALIDOS:
        raise ValueError(f"status inválido: {leitura['status']}")
```

### Separando válidas e inválidas

```python
leituras_validas = []
leituras_invalidas = []

for leitura in leituras_recebidas:
    try:
        validar_leitura(leitura)
        leituras_validas.append(leitura)
    except (TypeError, ValueError) as erro:
        leituras_invalidas.append({
            "tag": leitura.get("tag", "desconhecida"),
            "erro": str(erro),
        })
```

Esse padrão reforça a mesma ideia em Python: uma lista dinâmica pode crescer, mas cada item precisa ter formato confiável.

---

## 8. Objeto local no supervisor

O supervisor pode criar objetos locais para organizar os dados recebidos.

```python
class LeituraSupervisor:
    def __init__(self, dados: dict) -> None:
        self.tag = dados["tag"]
        self.tipo = dados["tipo"]
        self.valor = dados["valor"]
        self.unidade = dados["unidade"]
        self.status = dados["status"]

    def em_falha(self) -> bool:
        return self.status == "falha"

    def como_linha(self) -> dict:
        return {
            "tag": self.tag,
            "tipo": self.tipo,
            "valor": self.valor,
            "unidade": self.unidade,
            "status": self.status,
        }


leituras = [LeituraSupervisor(item) for item in leituras_validas]
linhas = [leitura.como_linha() for leitura in leituras]
```

O supervisor usa objetos para organizar a visualização, não para alterar o estado interno do dispositivo.

---

## 9. Perguntas de revisão rápida

1. Por que a validação deve ficar dentro do objeto em C++?
2. Por que a validação também é necessária no Python?
3. O que muda no laço quando um sensor lança uma exceção?
4. Como uma lista de dicionários pode virar uma lista de objetos no supervisor?

---

## Fontes de referência

- [cppreference - access specifiers](https://en.cppreference.com/w/cpp/language/access)
- [cppreference - exceptions](https://en.cppreference.com/w/cpp/language/exceptions)
- [Python Docs - Exceptions](https://docs.python.org/3/tutorial/errors.html)
- [Python Docs - Data Structures](https://docs.python.org/3/tutorial/datastructures.html)
