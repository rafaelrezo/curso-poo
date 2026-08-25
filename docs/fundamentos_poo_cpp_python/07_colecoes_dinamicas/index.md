# Coleções dinâmicas de objetos e memória segura

## Objetivos de aprendizagem

- Relacionar a lista encadeada estudada em C com `std::vector` em C++ e `list` em Python.
- Evoluir de uma coleção simples para uma coleção polimórfica com posse explícita.
- Percorrer zero, um ou vários dispositivos sem duplicar o algoritmo de aquisição.

**Tempo estimado:** 4h, em dois encontros de 2h.

## Vídeo da aula

![type:video](https://www.youtube.com/embed/RBSGKlAvoiM)

---

## 1. De onde partimos?

Na revisão de C, a turma implementou nós com `malloc`, ponteiros e `free`. Essa experiência mostrou três responsabilidades:

- reservar memória;
- conectar e percorrer elementos;
- liberar cada região reservada.

C++ e Python oferecem coleções prontas. A estrutura interna deixa de ser o problema da aplicação, mas ainda precisamos entender crescimento, referências e responsabilidade sobre os objetos.

---

## 2. Primeiro passo: `vector` de valores

Antes de combinar herança e ponteiros, confirme a coleção isoladamente:

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<double> leituras;
    leituras.push_back(42.5);
    leituras.push_back(43.0);

    for (double valor : leituras) {
        std::cout << valor << '\n';
    }
}
```

| Necessidade | Lista encadeada em C | C++ | Python |
|---|---|---|---|
| coleção que cresce | criar nó com `malloc` | `std::vector<T>` | `list` |
| inserir | ajustar ponteiros | `push_back` | `append` |
| percorrer | `while` e `proximo` | range-based `for` | `for` |
| liberar estrutura | `free` por nó | automático pelo contêiner | gerenciado pelo runtime |

---

## 3. Segundo passo: tipos diferentes na mesma coleção

O problema agora é guardar `SensorNivel` e `SensorPressao` como objetos que cumprem `Sensor`.

```cpp
#include <memory>
#include <vector>

std::vector<std::unique_ptr<Sensor>> sensores;
sensores.push_back(std::make_unique<SensorNivel>());
sensores.push_back(std::make_unique<SensorPressao>());

for (const auto& sensor : sensores) {
    sensor->adquirir();
    std::cout << sensor->tipo() << ": " << sensor->valor() << '\n';
}
```

Leia a declaração de dentro para fora:

| Trecho | Significado |
|---|---|
| `Sensor` | contrato comum |
| `unique_ptr<Sensor>` | um proprietário para cada objeto |
| `vector<...>` | coleção que cresce |
| `const auto&` | percorre sem copiar nem trocar o ponteiro armazenado |

`vector<Sensor>` não serve: `Sensor` é abstrata e copiar derivados como objetos-base causaria *slicing*. `unique_ptr` expressa posse única e libera os objetos automaticamente quando a coleção termina.

---

## 4. Ponte C++ -> Python

```python
sensores = [SensorNivel(), SensorPressao()]

for sensor in sensores:
    sensor.adquirir()
    print(f"{sensor.tipo}: {sensor.valor}")
```

Python torna a coleção mais curta porque a lista guarda referências e o runtime gerencia memória. O conceito comum continua sendo: uma coleção percorre objetos diferentes por uma operação comum.

---

## 5. Casos de teste cumulativos

| Caso | Resultado observável |
|---|---|
| coleção vazia | ciclo termina sem erro |
| um sensor | uma aquisição e uma saída |
| dois tipos | duas saídas com tipos distintos |
| inserção adicional | testes anteriores continuam passando |

Evite testes que somente procurem `vector` ou `unique_ptr` no arquivo. Valide quantidade de aquisições, tipos e valores produzidos.

---

## 6. Prática profissional

1. Crie `cap07-colecao-dispositivos`.
2. Faça primeiro um `vector<double>` funcionar.
3. Faça commit: `Adiciona coleção simples de leituras`.
4. Substitua a coleção fixa de sensores pela coleção polimórfica.
5. Faça commit: `Gerencia sensores com posse única`.
6. Execute testes cumulativos e envie a branch.
7. Explique no PR quem é responsável por liberar cada sensor.

### Erros comuns

| Erro | Diagnóstico |
|---|---|
| tentativa de copiar `unique_ptr` | percorra por referência e mova somente ao transferir posse |
| método da derivada não executa | confirme `virtual`, assinatura e `override` |
| vazamento com `new` manual | prefira `make_unique` |
| coleção vazia causa acesso inválido | não suponha que existe posição `0` |

---

## 7. Mini-caso prático

O controlador agora adquire uma quantidade variável de dispositivos. As leituras ainda vivem apenas na memória e desaparecem ao fim do processo. O próximo capítulo criará um contrato de telemetria para que outro programa possa consumi-las.

---

## Perguntas de revisão rápida

1. O que `vector` abstrai em comparação com a lista encadeada manual de C?
2. Por que a coleção polimórfica usa `unique_ptr<Sensor>`?
3. Quem libera os sensores quando o `vector` deixa de existir?

## Fontes de referência

- [cppreference — `std::vector`](https://en.cppreference.com/w/cpp/container/vector)
- [cppreference — `std::unique_ptr`](https://en.cppreference.com/w/cpp/memory/unique_ptr)
- [C++ Core Guidelines — resource management](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-resource)
- [Python Docs — listas](https://docs.python.org/pt-br/3/tutorial/introduction.html#listas)
- [W3Schools — C++ Vectors](https://www.w3schools.com/cpp/cpp_vectors.asp)
- [W3Schools — Python Lists](https://www.w3schools.com/python/python_lists.asp)
