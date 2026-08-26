# Protegendo o estado: acesso, atualização e primeira invariante

## Objetivos de aprendizagem

- Distinguir interface pública e estado interno em C++ e Python.
- Preservar uma leitura entre `0%` e `100%` por meio de uma operação controlada.
- Interpretar testes com asserções e validar a implementação localmente e na CI.

**Tempo estimado:** 2h, em um encontro.

## Vídeo da aula

![type:video](https://www.youtube.com/embed/0kN_DS3kcqQ)

O vídeo **Encapsulamento de Classes em C++ — Parte 1** apresenta atributos privados e operações públicas. Observe como o código externo usa a classe sem alterar diretamente seus dados internos.

---

## 1. O problema: um objeto aceita qualquer estado

Considere esta versão completa e simplificada de `SensorNivel`:

```cpp
#include <iostream>
#include <string>

class SensorNivel {
public:
    std::string tag;
    double valor;
    std::string unidade;

    SensorNivel(
        std::string tagInicial,
        double valorInicial,
        std::string unidadeInicial
    )
        : tag(tagInicial),
          valor(valorInicial),
          unidade(unidadeInicial) {
    }

    void exibirResumo() const {
        std::cout << tag << ": " << valor << " " << unidade << '\n';
    }
};

int main() {
    SensorNivel sensor{"LT-101", 42.5, "%"};

    sensor.exibirResumo();

    sensor.valor = -30.0;

    sensor.exibirResumo();
    return 0;
}
```

Leia a execução na ordem em que ela acontece:

1. `sensor` é criado com `valor` igual a `42.5`;
2. a primeira chamada de `exibirResumo()` mostra o estado inicial;
3. `sensor.valor = -30.0` acessa o atributo público e altera o objeto;
4. a segunda chamada mostra o novo estado.

Saída:

```text
LT-101: 42.5 %
LT-101: -30 %
```

Observe no programa completo que a atribuição `sensor.valor = -30.0` ocorre na `main`, fora da definição da classe. Ela compila porque `valor` está depois de `public:`. O objeto aceita a alteração mesmo que `-30%` não represente um nível percentual possível. Precisamos controlar quem pode alterar o estado e quais valores são aceitos.

### Roteiro do encontro

| Tempo | Bloco | Evidência |
|---:|---|---|
| 30 min | conceito e exemplo mínimo | explicar acesso e invariante |
| 70 min | prática no repositório-base | implementação C++ e Python |
| 15 min | testes, push e CI | validação verde |
| 5 min | fechamento | responder às perguntas de revisão |

---

## 2. Conceitos antes da prática

### 2.1 Interface pública e estado privado em C++

No exemplo anterior, `valor` pertence à interface pública: outros trechos podem ler e escrever diretamente. A **interface pública** reúne aquilo que o código cliente pode usar. O **estado privado** guarda detalhes acessíveis diretamente somente pela própria classe.

O programa completo abaixo protege `nivel_` e oferece apenas uma consulta pública:

```cpp
#include <iostream>

class Reservatorio {
private:
    double nivel_;

public:
    explicit Reservatorio(double nivelInicial)
        : nivel_(nivelInicial) {
    }

    double nivel() const {
        return nivel_;
    }
};

int main() {
    const Reservatorio reservatorio{42.5};
    std::cout << "Nivel: " << reservatorio.nivel() << "%\n";
    return 0;
}
```

Saída:

```text
Nivel: 42.5%
```

O método público `nivel()` permite consultar o valor. O `const` depois dos parênteses informa que a consulta não altera o objeto. Se uma linha como `reservatorio.nivel_ = -10.0` fosse adicionada à `main`, a compilação falharia porque `nivel_` está em `private`.

### 2.2 `private` não valida valores

O próximo programa já possui estado privado, mas a operação pública `atualizar` ainda aceita qualquer número:

```cpp
#include <iostream>

class Reservatorio {
private:
    double nivel_;

public:
    explicit Reservatorio(double nivelInicial)
        : nivel_(nivelInicial) {
    }

    double nivel() const {
        return nivel_;
    }

    void atualizar(double novoNivel) {
        nivel_ = novoNivel;
    }
};

int main() {
    Reservatorio reservatorio{42.5};
    reservatorio.atualizar(-10.0);
    std::cout << "Nivel: " << reservatorio.nivel() << "%\n";
    return 0;
}
```

Saída:

```text
Nivel: -10%
```

Para um nível percentual, queremos preservar:

```text
0.0 <= nível <= 100.0
```

Uma condição que deve permanecer verdadeira durante a vida do objeto é uma **invariante**. A versão completa corrigida valida antes de alterar:

```cpp
#include <iostream>

class Reservatorio {
private:
    double nivel_;

public:
    explicit Reservatorio(double nivelInicial)
        : nivel_(nivelInicial) {
    }

    double nivel() const {
        return nivel_;
    }

    bool atualizar(double novoNivel) {
        if (novoNivel < 0.0 || novoNivel > 100.0) {
            return false;
        }

        nivel_ = novoNivel;
        return true;
    }
};

int main() {
    Reservatorio reservatorio{42.5};
    const bool aceitou = reservatorio.atualizar(-10.0);

    std::cout << "Aceitou: " << std::boolalpha << aceitou << '\n';
    std::cout << "Nivel preservado: " << reservatorio.nivel() << "%\n";
    return 0;
}
```

Saída:

```text
Aceitou: false
Nivel preservado: 42.5%
```

| Retorno | Significado | Estado final |
|---|---|---|
| `true` | atualização aceita | guarda o novo valor |
| `false` | atualização rejeitada | preserva o valor anterior |

`private` controla o acesso; `atualizar` preserva a regra do domínio.

### 2.3 Python: aplicar a mesma invariante

O exemplo C++ anterior estabeleceu duas regras: aceitar apenas níveis entre `0` e `100` e preservar o valor anterior quando uma atualização for rejeitada. Agora aplicaremos essas mesmas regras em Python.

Python não possui blocos `public` e `private` como C++. A intenção é comunicada principalmente pelo nome:

| Nome | Intenção | O acesso externo é impedido? |
|---|---|---|
| `nivel`, `atualizar()` | parte pública da classe | não |
| `_nivel`, `_validar()` | detalhe interno; clientes não devem usar | não; é convenção |
| `__nivel` | evita colisões em subclasses por *name mangling* | não completamente |
| `__init__`, `__str__` | método especial definido pela linguagem | não se aplica |

Um `_` marca atributos e métodos não públicos. Dois sublinhados no início fazem Python transformar o nome internamente; isso evita colisões em subclasses, mas não cria uma barreira de segurança.

O programa Python completo aplica essa convenção e oferece uma propriedade para leitura:

```python
class Reservatorio:
    def __init__(self, nivel_inicial: float):
        self._nivel = nivel_inicial

    @property
    def nivel(self) -> float:
        return self._nivel

    def atualizar(self, novo_nivel: float) -> bool:
        if not 0.0 <= novo_nivel <= 100.0:
            return False

        self._nivel = novo_nivel
        return True


reservatorio = Reservatorio(42.5)
aceitou = reservatorio.atualizar(-10.0)

print(f"Aceitou: {aceitou}")
print(f"Nível preservado: {reservatorio.nivel:g}%")
```

Saída:

```text
Aceitou: False
Nível preservado: 42.5%
```

Em Python, o encapsulamento depende da interface documentada, da convenção e dos testes. `_nivel` continua tecnicamente acessível, mas não faz parte do contrato oferecido aos clientes. Como a propriedade não possui setter, `reservatorio.nivel = -10.0` produziria `AttributeError`.

Neste ponto, as duas implementações preservam a mesma faixa. A próxima subseção não introduz outra regra: ela compara, de forma separada, como C++ e Python verificam esse mesmo comportamento.

### 2.4 Comparar a verificação em C++ e Python

A regra verificada continua sendo `0 <= nível <= 100`. Ao chamar `atualizar(-0.1)`, os testes das duas linguagens devem confirmar duas consequências:

1. a operação informou que rejeitou o valor;
2. o nível válido anterior continuou armazenado.

#### Verificação manual em C++

O programa abaixo contém a classe C++, a execução e as comparações manuais:

```cpp
#include <iostream>

class Reservatorio {
private:
    double nivel_;

public:
    explicit Reservatorio(double nivelInicial)
        : nivel_(nivelInicial) {
    }

    double nivel() const {
        return nivel_;
    }

    bool atualizar(double novoNivel) {
        if (novoNivel < 0.0 || novoNivel > 100.0) {
            return false;
        }
        nivel_ = novoNivel;
        return true;
    }
};

int main() {
    Reservatorio reservatorio{42.5};

    const bool aceitou = reservatorio.atualizar(-0.1);

    if (aceitou != false) {
        std::cout << "FALHA: a entrada invalida foi aceita\n";
        return 1;
    }

    if (reservatorio.nivel() != 42.5) {
        std::cout << "FALHA: a rejeicao alterou o estado\n";
        return 1;
    }

    std::cout << "OK: entrada rejeitada e estado preservado\n";
    return 0;
}
```

Cada `if` compara um resultado **observado** com um resultado **esperado**. Um teste automatizado repete essas comparações sempre que o código muda, evitando que uma correção apague um comportamento que já funcionava.

#### Asserções em C++

Uma **asserção** expressa uma dessas expectativas de forma mais curta: “esta condição precisa ser verdadeira neste ponto do teste”. O programa completo equivalente fica assim:

```cpp
#include <cassert>
#include <iostream>

class Reservatorio {
private:
    double nivel_;

public:
    explicit Reservatorio(double nivelInicial)
        : nivel_(nivelInicial) {
    }

    double nivel() const {
        return nivel_;
    }

    bool atualizar(double novoNivel) {
        if (novoNivel < 0.0 || novoNivel > 100.0) {
            return false;
        }
        nivel_ = novoNivel;
        return true;
    }
};

int main() {
    Reservatorio reservatorio{42.5};
    const bool aceitou = reservatorio.atualizar(-0.1);

    assert(aceitou == false);
    assert(reservatorio.nivel() == 42.5);

    std::cout << "OK: entrada rejeitada e estado preservado\n";
    return 0;
}
```

Leia `assert(aceitou == false)` de dentro para fora:

1. `aceitou` é o resultado observado;
2. `false` é o resultado esperado;
3. `==` produz uma condição verdadeira ou falsa;
4. `assert` permite continuar se a condição for verdadeira;
5. se for falsa, interrompe o teste e informa a expressão, o arquivo e a linha.

O sucesso normalmente é silencioso. Por isso, depois de todas as asserções, o teste C++ imprime uma mensagem `OK`.

#### Asserções em Python

No Python, `unittest` verifica as mesmas duas consequências. O programa completo contém a classe e o teste:

```python
import unittest


class Reservatorio:
    def __init__(self, nivel_inicial: float):
        self._nivel = nivel_inicial

    @property
    def nivel(self) -> float:
        return self._nivel

    def atualizar(self, novo_nivel: float) -> bool:
        if not 0.0 <= novo_nivel <= 100.0:
            return False
        self._nivel = novo_nivel
        return True


class TestReservatorio(unittest.TestCase):
    def test_rejeita_e_preserva_o_nivel(self) -> None:
        reservatorio = Reservatorio(42.5)

        aceitou = reservatorio.atualizar(-0.1)

        self.assertFalse(aceitou)
        self.assertEqual(reservatorio.nivel, 42.5)


if __name__ == "__main__":
    unittest.main()
```

`assertFalse` confirma a rejeição. `assertEqual` confirma que o último nível válido foi preservado.

!!! warning "Teste e validação têm papéis diferentes"
    As asserções verificam se o programa cumpriu o contrato. O `if` dentro de `atualizar` é que protege o objeto durante a execução.

---

## 3. Prática — estender `SensorNivel`

### 3.1 Preparar o repositório-base

O starter contém `SensorNivel` com `tag`, `valor`, `unidade` e `resumo()`. A atividade acrescentará estado operacional, operações e contagem de leituras.

Acesse o repositório-base: [github.com/rafaelrezo/poo-objetos-validos](https://github.com/rafaelrezo/poo-objetos-validos).

Depois de abrir o repositório-base:

1. faça o fork;
2. habilite os workflows no fork, se solicitado;
3. clone substituindo `SEU_USUARIO`;
4. configure `upstream` e confira os remotos.

```bash
git clone https://github.com/SEU_USUARIO/poo-objetos-validos.git
cd poo-objetos-validos
git remote add upstream https://github.com/rafaelrezo/poo-objetos-validos.git
git remote -v
```

Execute o código inicial:

```bash
make build
make run
```

Saída inicial:

```text
LT-101: 42.5 %
TODO: adicionar estado operacional e registro de leituras
LT-101: 42.5 %
TODO: adicionar estado operacional e registro de leituras
```

As duas primeiras linhas são do C++; as duas últimas, do Python. O marcador indica a extensão ainda não implementada.

Crie uma única branch para a aula:

```bash
git switch -c pratica/01-estender-sensor
```

### 3.2 Decidir a interface

O sensor deverá saber se está ativo, permitir ativação e desativação, registrar leituras válidas somente quando ativo e contar as leituras aceitas.

Antes de programar, copie esta tabela para o pull request e complete as duas últimas colunas:

| Elemento | Público ou interno? | Justificativa |
|---|---|---|
| estado ativo/inativo | | |
| contador de leituras | | |
| `ativar` e `desativar` | | |
| consulta do estado | | |
| `registrarLeitura` | | |
| verificação da faixa | | |

Os testes definem as assinaturas usadas pelo cliente. A representação dos dados e uma possível função auxiliar de validação são decisões de implementação. Em C++, detalhes internos ficam em `private`; em Python, recebem nomes iniciados por `_`.

### 3.3 Checkpoint 01 — estado operacional

Estenda o cabeçalho C++ sem remover a estrutura existente:

```cpp
#ifndef SENSOR_NIVEL_HPP
#define SENSOR_NIVEL_HPP

#include <string>

class SensorNivel {
private:
    // TODO: declarar os dados internos existentes.
    // TODO: acrescentar o estado ativo/inativo, inicialmente falso.

public:
    SensorNivel(
        std::string tagInicial,
        double valorInicial,
        std::string unidadeInicial = ""
    );

    // TODO: declarar tag(), valor() e unidade() como consultas.
    // TODO: declarar estaAtivo(), ativar() e desativar().
    // Checkpoint 02: totalLeituras() e registrarLeitura().
    std::string resumo() const;
};

#endif
```

No Python, faça a mesma decisão no contexto da classe inteira:

```python
class SensorNivel:
    def __init__(
        self,
        tag: str,
        valor: float,
        unidade: str = "",
    ):
        # TODO: guardar os dados existentes como internos.
        # TODO: acrescentar o estado ativo/inativo, inicialmente falso.

    # TODO: criar properties para tag, valor, unidade e ativo.

    # TODO: implementar ativar() e desativar().
    # Checkpoint 02: total_leituras e registrar_leitura().

    def resumo(self) -> str:
        sufixo = f" {self._unidade}" if self._unidade else ""
        return f"{self._tag}: {self._valor:g}{sufixo}"
```

Valide o primeiro incremento:

```bash
make test ETAPA=01
```

O teste compila e executa clientes que consultam os dados, consultam o estado e chamam `ativar` e `desativar`. Em Python, também confirma que `ativo` não aceita atribuição direta.

### 3.4 Checkpoint 02 — registrar e contar leituras

Acrescente estas operações à interface pública:

| C++ | Python | Resultado |
|---|---|---|
| `bool registrarLeitura(double)` | `registrar_leitura(valor) -> bool` | aceita ou rejeita uma leitura |
| `int totalLeituras() const` | property `total_leituras` | informa quantas leituras foram aceitas |

`registrarLeitura` deve verificar, nesta ordem:

1. sensor ativo;
2. valor entre `0` e `100`;
3. se falhar, preservar valor e contador;
4. se aceitar, guardar valor, incrementar contador e retornar sucesso.

Atualize os dois programas principais para produzir:

```text
LT-101: 42.5 % | inativo | leituras: 0
Leitura aceita: true
LT-101: 55 % | ativo | leituras: 1
LT-101: 42.5 % | inativo | leituras: 0
Leitura aceita: True
LT-101: 55 % | ativo | leituras: 1
```

As três primeiras linhas são do C++; as demais, do Python.

### 3.5 Validar o contrato completo

O teste verifica esta sequência nas duas linguagens:

| Operação | Retorno esperado | Valor depois |
|---|---|---:|
| estado inicial | — | `42.5` |
| registrar enquanto inativo | falha | `42.5`, contador `0` |
| ativar e registrar `55.0` | sucesso | `55.0`, contador `1` |
| registrar `-0.1` | falha | `55.0`, contador `1` |
| registrar `100.1` | falha | `55.0`, contador `1` |
| desativar e registrar `60.0` | falha | `55.0`, contador `1` |

Execute um único comando:

```bash
make test ETAPA=02
```

Procure estas evidências:

```text
OK checkpoint 01 C++: interface e estado operacional confirmados
OK checkpoint 02 C++: registro e preservacao confirmados
OK saida: programas C++ e Python produziram o contrato esperado
OK
```

Se houver falha, leia primeiro o arquivo, a linha e a comparação apresentada. Não altere `tests/`, `Makefile` ou `.github/`.

---

## 4. Diagnóstico de erros comuns

| Sintoma | Causa provável | Ação |
|---|---|---|
| `private within this context` | o cliente acessa estado interno | use uma operação pública |
| `invalid use of member function` | usou `sensor.valor` no C++ | use `sensor.valor()` |
| `no declaration matches` | cabeçalho e `.cpp` divergem | compare nomes e tipos |
| leitura inativa altera o sensor | estado operacional não foi verificado primeiro | rejeite antes de alterar |
| contador aumenta após falha | incremento ocorreu antes de todas as verificações | incremente somente após aceitar |
| `TypeError: 'float' object is not callable` | chamou property Python como método | use `sensor.valor` |
| recursão em uma property | `valor` devolve `self.valor` | devolva `self._valor` |

---

## 5. Entrega profissional

Depois dos testes locais verdes:

```bash
git add include/sensor_nivel.hpp src/sensor_nivel.cpp src/sensor_nivel.py \
    src/main.cpp src/main.py AI_LOG.md
git commit -m "Estende sensor com estado e registro de leituras"
git push -u origin pratica/01-estender-sensor
```

Confira **Actions → Validação do capítulo 04** e abra o pull request somente depois da CI verde. Inclua:

- saída de `make run` com as transições esperadas;
- resultado de `make test ETAPA=02`;
- link da execução remota;
- tabela justificando membros públicos e internos;
- evidência de que leituras rejeitadas preservaram valor e contador;
- rastreabilidade do uso de IA, quando houver.

---

## 6. Resultado da atividade e ligação com o próximo capítulo

Ao final, o sensor produzido possui identidade, unidade, leitura atual, estado operacional e contador. Sua interface permite ativar, desativar, consultar e registrar leituras; sua representação interna não é alterada diretamente pelo programa principal.

Esse objeto ainda cuida apenas de si. No capítulo seguinte, uma `Bomba` terá seu próprio estado e um `Controlador` coordenará os dois objetos por composição.

## Perguntas de revisão rápida

1. Por que `ativo` e `totalLeituras` não devem aceitar atribuição direta pelo cliente?
2. Quais operações pertencem à interface pública e qual delas preserva mais de uma regra?
3. Por que o contador deve ser incrementado somente depois de todas as verificações?

## Evidências de conclusão

- [ ] a teoria foi discutida antes de abrir o fork;
- [ ] o código inicial compilou e exibiu os marcadores da extensão;
- [ ] `make test ETAPA=01` confirmou o estado operacional;
- [ ] `make test ETAPA=02` repetiu o checkpoint anterior e confirmou o registro;
- [ ] `make run` produziu a saída esperada nas duas linguagens;
- [ ] a única branch da aula teve CI verde;
- [ ] o PR justifica a visibilidade escolhida e rastreia o uso de IA.

## Fontes de referência

- [C++ Core Guidelines — interfaces de classes](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-class)
- [cppreference — controle de acesso](https://en.cppreference.com/w/cpp/language/access)
- [cppreference — `assert`](https://en.cppreference.com/w/cpp/error/assert)
- [Python Docs — classes e variáveis privadas](https://docs.python.org/pt-br/3/tutorial/classes.html#private-variables)
- [Python Docs — `property`](https://docs.python.org/pt-br/3/library/functions.html#property)
- [Python Docs — `unittest`](https://docs.python.org/pt-br/3/library/unittest.html)
- [PEP 8 — atributos e métodos públicos e não públicos](https://peps.python.org/pep-0008/#designing-for-inheritance)
- [GitHub Docs — sintaxe de workflows](https://docs.github.com/pt/actions/reference/workflows-and-actions/workflow-syntax)
