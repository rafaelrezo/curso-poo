# Identidade, igualdade e coleções: quando um cadastro é repetido?

## Objetivos de aprendizagem

- Distinguir identidade de instância e igualdade definida pelo domínio.
- Relacionar chaves estáveis, comparação e escolha de coleção.
- Explicar a inserção, a duplicata e a ausência em um catálogo de medições.

**Tempo estimado:** 2h de exposição dialogada e demonstrações. A prática dos capítulos 11 e 12 será uma única atividade no capítulo 12. Esta aula não exige entrega própria. O vídeo é complementar, dentro do tempo de estudo do bloco.

## Vídeo de contexto

![type:video](https://www.youtube.com/embed/ZWj8o692qGY)

Vídeo de apoio à comparação de objetos. Durante a leitura, distinga “mesma instância” de “mesmo valor”.

---

## 1. Dois cadastros podem representar o mesmo identificador

A estação agora registra medições de vários sensores. Duas operações criam objetos com a tag `LT-101`. São duas instâncias, mas o catálogo deve reconhecer uma única chave de cadastro.

A leitura muda ao longo do tempo; a identificação permanece. Por isso vamos separar o valor `IdSensor` da medição. A comparação de identificadores não deve depender da leitura atual.

## 2. Mesmo objeto e mesmo valor são perguntas diferentes

No programa, `alias` é outro nome para A. B é construído separadamente, com a mesma tag. Preveja os três resultados antes de executar.

Programa independente: [exemplo_05_igualdade.cpp](exemplo_05_igualdade.cpp).

```cpp
#include <iostream>
#include <stdexcept>
#include <string>

class IdSensor {
    std::string valor_;
public:
    explicit IdSensor(std::string valor) : valor_(valor) {
        if (valor.empty()) throw std::invalid_argument("tag vazia");
    }
    const std::string& valor() const { return valor_; }
    bool operator==(const IdSensor& outro) const { return valor_ == outro.valor_; }
};

int main() {
    IdSensor a{"LT-101"};
    IdSensor b{"LT-101"};
    const IdSensor& alias = a;
    std::cout << std::boolalpha;
    std::cout << "Alias: " << (&alias == &a) << '\n';
    std::cout << "Mesma instancia: " << (&a == &b) << '\n';
    std::cout << "Mesmo identificador: " << (a == b) << '\n';
}
```

Execute:

```bash
g++ -std=c++17 -Wall -Wextra -Werror -pedantic exemplo_05_igualdade.cpp -o exemplo_05_igualdade
./exemplo_05_igualdade
```

Resultado:

```text
Alias: true
Mesma instancia: false
Mesmo identificador: true
```

Endereços iguais identificam o mesmo objeto vivo. `operator==` define outra pergunta: os identificadores têm o mesmo valor? A igualdade é uma decisão do domínio, aqui baseada na tag exata, sem normalização.

### A mesma pergunta em Python

`is` verifica identidade. `==` usa o comportamento de igualdade da classe. Observe a construção dos dois objetos e a atribuição de `alias`.

Programa independente: [exemplo_06_igualdade.py](exemplo_06_igualdade.py).

```python
class IdSensor:
    def __init__(self, valor):
        if not valor:
            raise ValueError("tag vazia")
        self._valor = valor

    def __eq__(self, outro):
        if not isinstance(outro, IdSensor):
            return NotImplemented
        return self._valor == outro._valor


def main():
    a = IdSensor("LT-101")
    b = IdSensor("LT-101")
    alias = a
    print(f"Alias: {alias is a}")
    print(f"Mesma instancia: {a is b}")
    print(f"Mesmo identificador: {a == b}")


if __name__ == "__main__":
    main()
```

Execute:

```bash
python3 exemplo_06_igualdade.py
```

Resultado:

```text
Alias: True
Mesma instancia: False
Mesmo identificador: True
```

`NotImplemented` informa que a comparação com outro tipo não foi definida, permitindo que Python siga seu protocolo de comparação. Não é uma exceção lançada. Nesta primeira versão, estamos apenas comparando objetos; ainda não precisamos transformá-los em chaves de dicionário.

## 3. Uma sequência não elimina cadastros repetidos

Guardar os objetos em uma coleção não resolve automaticamente a regra de negócio. O vetor abaixo conserva cada inserção, mesmo quando dois identificadores são iguais.

Programa independente: [exemplo_07_sequencia.cpp](exemplo_07_sequencia.cpp).

```cpp
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class IdSensor {
    std::string valor_;
public:
    explicit IdSensor(std::string valor) : valor_(valor) {
        if (valor.empty()) throw std::invalid_argument("tag vazia");
    }
    const std::string& valor() const { return valor_; }
    bool operator==(const IdSensor& outro) const { return valor_ == outro.valor_; }
};

int main() {
    std::vector<IdSensor> ids{IdSensor{"LT-101"}, IdSensor{"LT-101"}, IdSensor{"LT-102"}};
    std::cout << "Quantidade: " << ids.size() << '\n';
    for (const auto& id : ids) std::cout << id.valor() << '\n';
}
```

Execute:

```bash
g++ -std=c++17 -Wall -Wextra -Werror -pedantic exemplo_07_sequencia.cpp -o exemplo_07_sequencia
./exemplo_07_sequencia
```

Resultado:

```text
Quantidade: 3
LT-101
LT-101
LT-102
```

O vetor está funcionando: sequências admitem repetição. Seria adequado para um histórico com várias ocorrências da mesma tag. Para o catálogo atual, queremos localizar uma medição pela chave e recusar um segundo cadastro equivalente.

| Técnica/Padrão | Melhor uso | Esforço | Entregável | Limitação |
|---|---|---|---|---|
| Sequência: `vector` / `list` | histórico e percurso na ordem dos elementos | baixo | registros com repetição | não impõe unicidade |
| Conjunto: `set` | saber se um identificador pertence ao grupo | médio | chaves sem duplicatas | não associa sozinho uma medição à chave |
| Mapa: `map` / `dict` | consultar um item pela identificação | médio | catálogo por chave | a política de duplicata precisa ser escolhida |

Escolha a sequência para ocorrências históricas, conjunto para pertencimento e mapa para este catálogo. A multiplicidade “muitos” descreve o domínio, mas não escolhe a coleção.

## 4. O catálogo precisa de uma política de duplicata

Nossa regra será: inserir uma tag existente devolve falso e preserva o registro anterior. Buscar uma tag ausente comunica ausência. Começamos com um catálogo específico de `Medicao`, sem exigir genericidade para entender o comportamento.

Em C++, `std::map` organiza as chaves usando uma comparação de ordem. Acrescentamos `operator<` pela tag; duas tags equivalentes não são menores uma que a outra. A ordenação precisa concordar com o significado de chave usado pelo domínio.

Programa independente: [exemplo_08_catalogo_medicoes.cpp](exemplo_08_catalogo_medicoes.cpp).

```cpp
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

class IdSensor {
    std::string valor_;
public:
    explicit IdSensor(std::string valor) : valor_(valor) {
        if (valor.empty()) throw std::invalid_argument("tag vazia");
    }
    const std::string& valor() const { return valor_; }
    bool operator<(const IdSensor& outro) const { return valor_ < outro.valor_; }
    bool operator==(const IdSensor& outro) const { return valor_ == outro.valor_; }
};

struct Medicao {
    double valor;
    std::string unidade;
};

class Catalogo {
    std::map<IdSensor, Medicao> itens_;
public:
    bool inserir(const IdSensor& id, const Medicao& item) {
        return itens_.emplace(id, item).second;
    }
    const Medicao* buscar(const IdSensor& id) const {
        auto it = itens_.find(id);
        return it == itens_.end() ? nullptr : &it->second;
    }
    std::size_t quantidade() const { return itens_.size(); }
};

int main() {
    Catalogo catalogo;
    std::cout << std::boolalpha;
    std::cout << "Primeira: " << catalogo.inserir(IdSensor{"LT-101"}, {12, "%"}) << '\n';
    std::cout << "Duplicada: " << catalogo.inserir(IdSensor{"LT-101"}, {99, "%"}) << '\n';
    const auto* item = catalogo.buscar(IdSensor{"LT-101"});
    if (item) std::cout << "Preservada: " << item->valor << ' ' << item->unidade << '\n';
    std::cout << "Ausente: " << (catalogo.buscar(IdSensor{"LT-999"}) == nullptr) << '\n';
    std::cout << "Quantidade: " << catalogo.quantidade() << '\n';
}
```

Execute:

```bash
g++ -std=c++17 -Wall -Wextra -Werror -pedantic exemplo_08_catalogo_medicoes.cpp -o exemplo_08_catalogo_medicoes
./exemplo_08_catalogo_medicoes
```

Resultado:

```text
Primeira: true
Duplicada: false
Preservada: 12 %
Ausente: true
Quantidade: 1
```

`emplace` devolve um resultado cujo campo `second` informa se a inserção ocorreu. `find` procura pela chave; `end` significa que ela não foi encontrada. Por isso a busca retorna `nullptr` quando não existe registro.

O ponteiro retornado permite ler um item do catálogo; não transfere posse. Não o utilize depois de remover esse item ou destruir o catálogo. A tentativa de registrar 99 preservou 12: inserir e atualizar são operações diferentes neste contrato.

**Preveja uma variação:** trocar somente a segunda tag por `LT-102` fará a quantidade passar a dois. A decisão depende da chave, não do endereço do objeto nem do número da medição.

## 5. Python: a chave precisa cooperar com o dicionário

Python `dict` utiliza hash e igualdade. Objetos iguais precisam ter o mesmo hash; hashes iguais não provam igualdade, porque colisões são possíveis. A chave precisa permanecer estável enquanto estiver no dicionário.

Agora `@dataclass(frozen=True)` gera comparação e hash coerentes para `IdSensor`, com base em seu campo textual, e impede atribuições usuais aos campos. O programa continua verificando a tag vazia. A medição é um registro imutável neste recorte.

Programa independente: [exemplo_09_catalogo_medicoes.py](exemplo_09_catalogo_medicoes.py).

```python
from dataclasses import dataclass


@dataclass(frozen=True)
class IdSensor:
    valor: str

    def __post_init__(self):
        if not self.valor:
            raise ValueError("tag vazia")


@dataclass(frozen=True)
class Medicao:
    valor: float
    unidade: str


class Catalogo:
    def __init__(self):
        self._itens = {}

    def inserir(self, id, item):
        if id in self._itens:
            return False
        self._itens[id] = item
        return True

    def buscar(self, id):
        return self._itens.get(id)

    def quantidade(self):
        return len(self._itens)


def main():
    catalogo = Catalogo()
    print(f"Primeira: {catalogo.inserir(IdSensor('LT-101'), Medicao(12, '%'))}")
    print(f"Duplicada: {catalogo.inserir(IdSensor('LT-101'), Medicao(99, '%'))}")
    item = catalogo.buscar(IdSensor("LT-101"))
    if item is not None:
        print(f"Preservada: {item.valor} {item.unidade}")
    print(f"Ausente: {catalogo.buscar(IdSensor('LT-999')) is None}")
    print(f"Quantidade: {catalogo.quantidade()}")


if __name__ == "__main__":
    main()
```

Execute:

```bash
python3 exemplo_09_catalogo_medicoes.py
```

Resultado:

```text
Primeira: True
Duplicada: False
Preservada: 12 %
Ausente: True
Quantidade: 1
```

A verificação `id in self._itens` aplica a regra de rejeitar duplicata antes da atribuição. `get` retorna `None` na ausência; este catálogo não aceita `None` como item de domínio.

O C++ armazena medições por valor. O Python guarda referências a registros imutáveis: remover uma entrada não elimina referências externas a esse mesmo registro. Nos dois casos, o catálogo gerencia cadastros do software, não a existência dos sensores físicos.

`std::map` percorre chaves em ordem definida pelo comparador; `dict` preserva a ordem de inserção. O contrato comum de catálogo não deve depender de confundir essas duas ordens. Não use o número do hash como identificador persistente.

## 6. O que muda quando o catálogo precisa crescer?

Primeiro estabilizamos a regra. Depois podemos generalizar o tipo armazenado: o starter fornece `Catalogo<T>` em C++ e `Catalogo[T]` em Python. A tarefa do estudante continuará sendo implementar as operações do catálogo; não será necessário construir uma biblioteca genérica.

Para aprofundamento, os programas completos [catálogo genérico C++](exemplo_03_catalogo.cpp) e [catálogo genérico Python](exemplo_04_catalogo.py) mostram a mesma inserção e busca com outro tipo de item. Compare o lugar ocupado pelo tipo, mantendo o comportamento do cliente.

Outra necessidade seria percorrer várias fontes pelo contrato `IFonteLeitura`. O starter também fornece essa operação: em C++, um vetor de `unique_ptr` gerencia as fontes com posse exclusiva; em Python, uma lista guarda suas referências. Genericidade varia um tipo de armazenamento; polimorfismo permite comportamentos concretos distintos sob uma interface. São decisões diferentes, e nenhuma exige uma entrega adicional nesta aula.

**O próximo problema:** uma equipe precisa entender o cadastro e decidir o que pode ser removido. No [capítulo 12](../../modelagem_analise_codigo/index.md), vamos modelar esse mesmo sistema e completar a prática integrada B, incluindo remoção e justificativa das relações.

## Perguntas de revisão rápida

1. Como duas instâncias distintas podem representar a mesma chave? Por que a leitura não participa dessa igualdade?
2. Por que um vetor pode conter duas tags iguais? Qual estrutura atende à busca de uma medição por tag?
3. O que deve acontecer ao inserir uma duplicata ou buscar uma tag ausente? Como explicar esses resultados nas duas linguagens?

## Fontes de referência

- [Python — identidade, igualdade e hash](https://docs.python.org/3/reference/datamodel.html#object.__hash__).
- [Python — dataclasses](https://docs.python.org/3/library/dataclasses.html).
- [Python — estruturas de dados](https://docs.python.org/3/tutorial/datastructures.html).
- [C++ — map](https://en.cppreference.com/w/cpp/container/map).
- [C++ — map::emplace](https://en.cppreference.com/w/cpp/container/map/emplace).
- [C++ Core Guidelines — gerenciamento de recursos](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-resource).
