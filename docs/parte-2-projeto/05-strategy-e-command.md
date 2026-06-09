# Aula 7 - Padrões de Projeto I: Strategy, Command e Repository

## Objetivos de aprendizagem

- Entender padrões de projeto como soluções de organização, não apenas como trechos de código.
- Relacionar `Strategy`, `Command` e `Repository` com arquivos e responsabilidades do projeto.
- Usar um repositório de apoio para estudar como a separação em pastas facilita manutenção, testes e defesa técnica.

**Tempo estimado:** 2h

## Vídeo da aula

![type:video](https://www.youtube.com/embed/Y3fe-9DA7Lg)

---

## 1. O que é um padrão de projeto?

Um padrão de projeto é uma forma conhecida de organizar uma solução para um problema que aparece com frequência em software.

Ele não é uma biblioteca pronta. Também não é uma obrigação. Um padrão só faz sentido quando ajuda a responder perguntas como:

- onde coloco esta regra?
- quem executa esta ação?
- quem acessa o banco?
- como eu testo isso sem misturar tudo?
- se eu precisar mudar uma regra, quantos arquivos preciso alterar?

Nesta aula, três padrões serão usados no contexto da estação de bombeamento:

| Problema no projeto | Padrão | Ideia principal |
|---|---|---|
| trocar regras de controle | `Strategy` | cada regra fica em uma classe própria |
| representar ações de atuação | `Command` | cada comando vira um objeto com execução padronizada |
| isolar acesso a dados | `Repository` | banco, CSV ou memória ficam atrás de uma interface |

Regra prática:

```text
Padrão de projeto deve deixar o código mais fácil de entender, testar ou mudar.
Se ele só aumenta o número de classes sem clareza, foi usado cedo demais.
```

---

## 2. Por que mostrar arquivos separados?

Quando todo o exemplo aparece em um único bloco, fica fácil copiar e executar. Mas isso pode esconder uma parte importante dos padrões: eles também ajudam a organizar o projeto em arquivos menores.

No projeto final, a equipe precisa explicar onde estão:

- sensores;
- regras;
- comandos;
- eventos;
- persistência;
- supervisor;
- testes.

Se tudo estiver em um único `main.cpp` ou em um único `app.py`, a defesa técnica fica mais difícil. O aluno precisa procurar a regra, o comando e o acesso ao banco no mesmo arquivo.

Por isso, esta aula usa um repositório de apoio:

- [Repositório `curso-poo-exemplos`](https://github.com/rafaelrezo/curso-poo-exemplos)
- [Exemplo `padroes_i_estacao`](https://github.com/rafaelrezo/curso-poo-exemplos/tree/main/padroes_i_estacao)

Esse repositório deve ser lido como referência de organização. Ele não é uma entrega pronta do projeto final.

---

## 3. Problema base da estação

A estação recebe leituras de nível e pressão. Com base nelas, o sistema precisa:

1. decidir se a bomba deve ligar, desligar ou bloquear;
2. transformar essa decisão em uma ação;
3. executar a ação na bomba;
4. registrar dados no supervisor.

Uma solução direta poderia colocar tudo no controlador:

```cpp
string avaliar(double nivel, double pressao) {
    if (pressao > 6.0) {
        return "BLOQUEAR_PARTIDA";
    }
    if (nivel < 30.0) {
        return "LIGAR_BOMBA";
    }
    if (nivel > 80.0) {
        return "DESLIGAR_BOMBA";
    }
    return "NENHUM";
}
```

Essa solução resolve o início, mas cria problemas quando aparecem:

- uma regra econômica;
- uma regra de emergência;
- uma regra própria da dupla;
- comandos com histórico;
- persistência em SQLite.

O sinal de alerta é:

```text
toda mudança importante obriga a editar o mesmo arquivo.
```

Contraexemplos no repositório de apoio:

- [C++ sem `Strategy` e sem `Command`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/padroes_i_estacao/sem_padroes/dispositivo_cpp/main.cpp)
- [Python sem `Repository`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/padroes_i_estacao/sem_padroes/supervisor_python/app_sem_repository.py)

Use esses arquivos como comparação. Eles funcionam, mas concentram responsabilidades que depois serão separadas.

---

## 4. Estrutura do exemplo de apoio

Abra o exemplo:

- [https://github.com/rafaelrezo/curso-poo-exemplos/tree/main/padroes_i_estacao](https://github.com/rafaelrezo/curso-poo-exemplos/tree/main/padroes_i_estacao)

A estrutura principal é:

```text
padroes_i_estacao/
  sem_padroes/
    dispositivo_cpp/
      main.cpp
    supervisor_python/
      app_sem_repository.py
  dispositivo_cpp/
    include/
      tipo_comando.hpp
      bomba.hpp
      estrategia_controle.hpp
      controlador_estacao.hpp
      comando.hpp
    src/
      main.cpp
  supervisor_python/
    modelos.py
    repositorios.py
    app_demo.py
```

Leitura da arquitetura:

| Arquivo | Papel |
|---|---|
| `sem_padroes/` | contraexemplos para comparar com a versão organizada |
| `tipo_comando.hpp` | define o conjunto de comandos possíveis |
| `bomba.hpp` | representa o atuador físico simulado |
| `estrategia_controle.hpp` | contém regras de controle (`Strategy`) |
| `controlador_estacao.hpp` | usa uma estratégia sem conhecer seus detalhes |
| `comando.hpp` | contém comandos de atuação (`Command`) |
| `main.cpp` | monta o fluxo C++ completo |
| `modelos.py` | define dados usados pelo supervisor |
| `repositorios.py` | isola acesso a dados (`Repository`) |
| `app_demo.py` | mostra o fluxo Python usando o repositório |

Essa separação já é parte do aprendizado de padrões de projeto.

Leitura recomendada:

1. Comece por `sem_padroes/`.
2. Observe quais responsabilidades estão misturadas.
3. Depois leia `include/` e `supervisor_python/`.
4. Compare como cada padrão desloca uma responsabilidade para um arquivo próprio.

---

## 5. Strategy: regra de controle em arquivo próprio

`Strategy` aparece quando a regra pode variar.

No exemplo, a regra está em:

- [Strategy - `estrategia_controle.hpp`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/padroes_i_estacao/dispositivo_cpp/include/estrategia_controle.hpp)

Trecho central:

```cpp
class EstrategiaControle {
public:
    virtual ~EstrategiaControle() = default;

    virtual TipoComando decidir(double nivel, double pressao) const = 0;
    virtual std::string nome() const = 0;
};
```

O arquivo também contém estratégias concretas, por exemplo:

```cpp
class ControleNormal : public EstrategiaControle {
public:
    TipoComando decidir(double nivel, double pressao) const override {
        if (pressao > 6.0) {
            return TipoComando::BloquearPartida;
        }
        if (nivel < 30.0) {
            return TipoComando::LigarBomba;
        }
        if (nivel > 80.0) {
            return TipoComando::DesligarBomba;
        }
        return TipoComando::Nenhum;
    }
};
```

O ganho não é apenas trocar `if` por classe. O ganho é organizacional:

| Antes | Depois |
|---|---|
| regra dentro do controlador | regra em `estrategia_controle.hpp` |
| controlador cresce a cada regra nova | nova regra vira nova classe |
| difícil testar uma regra isolada | regra pode ser testada separadamente |
| decisão misturada com execução | decisão fica separada da atuação |

O controlador usa a estratégia aqui:

- [Controlador - `controlador_estacao.hpp`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/padroes_i_estacao/dispositivo_cpp/include/controlador_estacao.hpp)

Trecho:

```cpp
class ControladorEstacao {
private:
    const EstrategiaControle& estrategia;

public:
    explicit ControladorEstacao(const EstrategiaControle& regra)
        : estrategia(regra) {}

    TipoComando avaliar(double nivel, double pressao) const {
        return estrategia.decidir(nivel, pressao);
    }
};
```

Leitura: o controlador não sabe se está usando `ControleNormal`, `ControleEconomico` ou outra regra futura. Ele conhece apenas a interface.

---

## 6. Command: atuação em arquivo próprio

`Command` aparece quando uma ação precisa ser executada de forma padronizada.

No exemplo, os comandos estão em:

- [Command - `comando.hpp`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/padroes_i_estacao/dispositivo_cpp/include/comando.hpp)

Trecho central:

```cpp
class Comando {
public:
    virtual ~Comando() = default;

    virtual void executar(Bomba& bomba) const = 0;
    virtual std::string nome() const = 0;
};
```

Exemplo de comando concreto:

```cpp
class LigarBomba : public Comando {
public:
    void executar(Bomba& bomba) const override {
        bomba.ligar();
    }

    std::string nome() const override {
        return "LIGAR_BOMBA";
    }
};
```

O ganho organizacional:

| Antes | Depois |
|---|---|
| strings como `"LIGAR_BOMBA"` espalhadas | comandos concentrados em `comando.hpp` |
| muitos `if` para executar ações | cada comando sabe executar a própria ação |
| histórico difícil de padronizar | todo comando tem `nome()` |
| ação misturada com decisão | `Strategy` decide, `Command` executa |

O arquivo também possui uma função de criação:

```cpp
inline std::unique_ptr<Comando> criarComando(TipoComando tipo) {
    if (tipo == TipoComando::LigarBomba) {
        return std::make_unique<LigarBomba>();
    }
    if (tipo == TipoComando::DesligarBomba) {
        return std::make_unique<DesligarBomba>();
    }
    if (tipo == TipoComando::BloquearPartida) {
        return std::make_unique<BloquearPartida>();
    }
    return nullptr;
}
```

Essa função liga a decisão da estratégia ao comando concreto.

---

## 7. Fluxo C++ completo

O fluxo completo está em:

- [C++ principal - `main.cpp`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/padroes_i_estacao/dispositivo_cpp/src/main.cpp)

O arquivo `main.cpp` faz a montagem:

```cpp
ControleNormal regra;
ControladorEstacao controlador(regra);
Bomba bomba;

TipoComando tipo = controlador.avaliar(nivel, pressao);
std::unique_ptr<Comando> comando = criarComando(tipo);

if (comando) {
    comando->executar(bomba);
}
```

Leitura do fluxo:

```text
leitura de nível/pressão
        |
        v
Strategy decide o TipoComando
        |
        v
criarComando cria o Command correto
        |
        v
Command executa ação na Bomba
```

Esse é o ponto principal da aula: o projeto fica mais compreensível porque cada arquivo tem um papel.

---

## 8. Ponte C++ -> Python: tipos padronizados

Em C++, o exemplo usa `enum class` em:

- [Tipo de comando - `tipo_comando.hpp`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/padroes_i_estacao/dispositivo_cpp/include/tipo_comando.hpp)

Em Python, a ideia equivalente aparece em:

- [Modelos Python - `modelos.py`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/padroes_i_estacao/supervisor_python/modelos.py)

Trecho:

```python
from enum import Enum


class TipoComando(Enum):
    NENHUM = "NENHUM"
    LIGAR_BOMBA = "LIGAR_BOMBA"
    DESLIGAR_BOMBA = "DESLIGAR_BOMBA"
    BLOQUEAR_PARTIDA = "BLOQUEAR_PARTIDA"
```

Isso evita retornar strings soltas em vários pontos do código. Quando for necessário gerar JSON, log ou texto para tela, usa-se o valor textual do enum.

---

## 9. Repository: persistência em arquivo próprio

`Repository` aparece quando o acesso aos dados não deve ficar misturado com a tela ou com a regra.

No exemplo, o repositório está em:

- [Repository - `repositorios.py`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/padroes_i_estacao/supervisor_python/repositorios.py)

Compare com o contraexemplo:

- [Python sem `Repository` - `app_sem_repository.py`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/padroes_i_estacao/sem_padroes/supervisor_python/app_sem_repository.py)

Trecho central:

```python
class RepositorioLeituras(Protocol):
    def salvar(self, leitura: Leitura) -> None:
        ...

    def listar_todas(self) -> list[Leitura]:
        ...
```

O mesmo contrato pode ter uma versão em memória:

```python
class RepositorioLeiturasMemoria:
    def __init__(self) -> None:
        self._leituras: list[Leitura] = []

    def salvar(self, leitura: Leitura) -> None:
        self._leituras.append(leitura)

    def listar_todas(self) -> list[Leitura]:
        return self._leituras
```

E uma versão SQLite:

```python
class RepositorioLeiturasSqlite:
    def salvar(self, leitura: Leitura) -> None:
        ...

    def listar_todas(self) -> list[Leitura]:
        ...
```

Ganho organizacional:

| Antes | Depois |
|---|---|
| SQL direto na tela | SQL dentro de `repositorios.py` |
| difícil testar sem banco | repositório em memória |
| troca de tecnologia espalhada | troca concentrada no repositório |
| tela sabe demais | tela apenas pede dados |

O fluxo Python de demonstração está em:

- [Supervisor demo - `app_demo.py`](https://github.com/rafaelrezo/curso-poo-exemplos/blob/main/padroes_i_estacao/supervisor_python/app_demo.py)

---

## 10. Como executar o exemplo

Clone o repositório de exemplos:

```bash
git clone https://github.com/rafaelrezo/curso-poo-exemplos.git
cd curso-poo-exemplos/padroes_i_estacao
```

Executar C++:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic \
  -I dispositivo_cpp/include \
  dispositivo_cpp/src/main.cpp \
  -o /tmp/padroes_i_estacao_cpp

/tmp/padroes_i_estacao_cpp
```

Executar Python:

```bash
python3 supervisor_python/app_demo.py
```

---

## 11. Mini-caso prático

Uma equipe quer adicionar uma regra própria:

```text
Se a pressão passar de 5.8 bar e o nível estiver abaixo de 35%,
bloquear a partida por risco de operação instável.
```

Com a organização do exemplo, a mudança deve seguir este raciocínio:

1. Criar uma nova classe de regra em `estrategia_controle.hpp`.
2. Testar se a nova regra retorna o `TipoComando` correto.
3. Reaproveitar `comando.hpp` para executar a ação.
4. Registrar no README onde a regra aparece.
5. Se houver persistência no supervisor, salvar a leitura via `repositorios.py`.

O objetivo é evitar alterar vários pontos do projeto sem necessidade.

---

## 12. Exercícios práticos

1. Abrir o repositório `curso-poo-exemplos`.
2. Executar primeiro o contraexemplo em `sem_padroes/`.
3. Localizar onde está o `Strategy`.
4. Localizar onde está o `Command`.
5. Localizar onde está o `Repository`.
6. Explicar, em três frases, por que `main.cpp` ficou mais curto na versão com padrões.
7. Criar uma regra de controle nova no exemplo local.
8. Documentar no README do projeto final quais arquivos representam cada padrão.

---

## 13. Checklist de entrega

- [ ] O projeto possui regra de controle separada do controlador principal.
- [ ] Os comandos de atuação estão padronizados.
- [ ] O supervisor possui módulo ou classe de acesso a dados.
- [ ] A organização de pastas ajuda a localizar cada responsabilidade.
- [ ] O README explica onde estão `Strategy`, `Command` e `Repository`.

---

## 14. Perguntas de revisão rápida

1. Por que `Strategy` deve ficar separado do controlador?
2. Por que `Command` ajuda a registrar e testar atuações?
3. Por que `Repository` melhora a separação entre tela e banco?

---

## Fontes de referência

- [Repositório de exemplos do curso](https://github.com/rafaelrezo/curso-poo-exemplos)
- [Refactoring.Guru - Padrões de Projeto em português](https://refactoring.guru/pt-br/design-patterns)
- [C++ Core Guidelines - Interfaces](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Ri-abstract)
- [Python Docs - sqlite3](https://docs.python.org/3/library/sqlite3.html)
- [Python Docs - dataclasses](https://docs.python.org/3/library/dataclasses.html)
- [Martin Fowler - Repository](https://martinfowler.com/eaaCatalog/repository.html)
- [Refactoring Guru - Strategy](https://refactoring.guru/design-patterns/strategy)
- [Refactoring Guru - Command](https://refactoring.guru/design-patterns/command)
