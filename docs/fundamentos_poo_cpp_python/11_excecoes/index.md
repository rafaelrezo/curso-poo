# 10. Exceções: lançar, propagar, recuperar e liberar recursos

## Objetivos de aprendizagem

- Distinguir rejeição esperada de uma operação e falha excepcional de aquisição.
- Modelar exceções próprias e rastrear lançamento, propagação e captura em C++ e Python.
- Liberar recursos no sucesso e na falha, usando RAII em C++ e finally em Python.

**Tempo estimado:** 2h de estudo e prática. O vídeo é preparação prévia; confira a distribuição entre sala e prática no [roteiro da trilha](../../index.md).

## Vídeo de contexto

![type:video](https://www.youtube.com/embed/RHSxIKGCX7c)

Otávio Miranda — Try, Except, Tratando Exceções em Python. Observe onde o fluxo normal para e por que a limpeza precisa ocorrer mesmo sem resultado.

---

## 1. Mini-caso prático: a fonte pode ficar indisponível

A estação tem fontes consultáveis. Durante uma aquisição, a conexão simulada pode estar indisponível ou a calibração pode estar pendente. O operador precisa receber uma falha identificável e continuar no ciclo seguinte, sem deixar uma sessão aberta.

**O contrato antigo continua:** `SensorNivel.atualizar` retorna falso ao rejeitar uma leitura. Vamos criar uma operação diferente, `adquirir`, que pode lançar falhas de aquisição. Não mude silenciosamente a semântica do sensor já testado.

```text
executarCiclo -> lerServico -> adquirir -> consulta da fonte
      ^                           |
      +------ falha propagada ----+
```

O serviço intermediário não tem uma decisão útil de recuperação e deixa a falha subir. A fronteira decide devolver resultado sem leitura.

---

## 2. Retome o artefato e abra a branch

Esta é a aula 10; `ETAPA=11` e a branch existente são identificadores técnicos preservados do starter.

Use o próprio fork de [rafaelrezo/poo-fundamentos-estacao](https://github.com/rafaelrezo/poo-fundamentos-estacao), com **a etapa 10** concluída e integrada. O clone deve ter somente `origin`, apontando para o fork. Não copie arquivos dos repositórios das seções 01–06.

```bash
git switch main
git pull --ff-only origin main
git remote -v
git switch -c pratica/11-excecoes
make test ETAPA=11
```

O comando repete os contratos anteriores e inicialmente falha no comportamento ainda pendente desta seção. Leia a primeira mensagem; não altere testes ou automação para obter aprovação. Complete os incrementos abaixo e repita o mesmo comando.

---

## 3. O tipo de erro comunica a decisão possível

O esqueleto oferece `FalhaLeitura` e sua especialização `FalhaCalibracao`. Os programas completos a seguir começam pela falha geral; a calibração será acrescentada na prática de adaptação.

Uma falha de calibração também é uma falha de leitura. Por isso uma captura pela classe-base pode tratar ambas. A classe define o tipo de problema; as condições da operação definem quando lançá-lo.

| Condição | Resposta |
|---|---|
| disponível e calibrada | devolver valor consultado |
| indisponível | lançar `FalhaLeitura` |
| disponível, sem calibração | lançar `FalhaCalibracao` |
| ambas as condições falham | indisponibilidade tem prioridade |
| defeito inesperado na implementação | liberar recurso e propagar; não mascarar como falha prevista |

---

## 4. C++: acompanhar lançamento, propagação e limpeza até o main

O recurso é um contador de sessões, sem rede real. O resultado deve ser observável: depois de cada ciclo, o contador volta a zero. Leia primeiro os três ciclos do `main` (sucesso, indisponibilidade, sucesso); depois siga `executarCiclo`, `lerServico` e `adquirir`.

Este programa independente usa uma fonte fixa de 42,5 para concentrar o estudo no fluxo de falha. O starter continua recebendo `IFonteLeitura` e também o parâmetro `calibrado`; preserve essa interface ao adaptar o exemplo. A segunda condição de falha fica para a extensão.

Salve como `exemplo_01_excecoes.cpp` ou [baixe o programa completo](exemplo_01_excecoes.cpp).

```cpp
#include <iostream>
#include <stdexcept>

class FonteConstante {
public:
    double valor() const { return 42.5; }
};

class FalhaLeitura : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class Sessao {
    int& abertas_;
public:
    explicit Sessao(int& abertas) : abertas_(abertas) { ++abertas_; }
    ~Sessao() { --abertas_; }
    Sessao(const Sessao&) = delete;
    Sessao& operator=(const Sessao&) = delete;
};

double adquirir(const FonteConstante& fonte, bool disponivel, int& abertas) {
    Sessao sessao{abertas};
    if (!disponivel) throw FalhaLeitura("fonte indisponivel");
    return fonte.valor();
}

double lerServico(const FonteConstante& fonte, bool disponivel, int& abertas) {
    return adquirir(fonte, disponivel, abertas);
}

struct ResultadoLeitura {
    bool sucesso;
    double valor;
};

ResultadoLeitura executarCiclo(const FonteConstante& fonte, bool disponivel,
                              int& abertas) {
    try {
        return {true, lerServico(fonte, disponivel, abertas)};
    } catch (const FalhaLeitura&) {
        return {false, 0};
    }
}

void mostrar(const ResultadoLeitura& resultado, int abertas) {
    if (resultado.sucesso) std::cout << "Leitura: " << resultado.valor;
    else std::cout << "Sem leitura";
    std::cout << " | sessoes: " << abertas << '\n';
}

int main() {
    FonteConstante fonte;
    int abertas = 0;
    auto primeiro = executarCiclo(fonte, true, abertas);
    mostrar(primeiro, abertas);
    auto segundo = executarCiclo(fonte, false, abertas);
    mostrar(segundo, abertas);
    auto terceiro = executarCiclo(fonte, true, abertas);
    mostrar(terceiro, abertas);
}
```

```bash
g++ -std=c++17 -Wall -Wextra -Werror -pedantic exemplo_01_excecoes.cpp -o exemplo_01_excecoes
./exemplo_01_excecoes
```

Saída esperada:

```text
Leitura: 42.5 | sessoes: 0
Sem leitura | sessoes: 0
Leitura: 42.5 | sessoes: 0
```

### 4.1 Por que o segundo ciclo não apresenta um número?

1. `adquirir` cria `Sessao`, elevando o contador a 1.
2. A indisponibilidade provoca `throw FalhaLeitura`. O retorno da consulta não é executado.
3. Ao sair desse escopo durante a propagação, o destrutor de `Sessao` reduz o contador a 0.
4. `lerServico` não captura; a falha chega a `executarCiclo`, que captura a família prevista por referência constante.
5. `mostrar` verifica `sucesso` e escreve `Sem leitura`. O terceiro ciclo comprova que o programa pode continuar.

O zero no resultado de falha não é uma medição: o cliente deve verificar `sucesso` antes de usar `valor`. Nunca atualize o sensor com esse zero. Em integração, o contrato poderá representar ausência com valor nulo.

**RAII** vincula o recurso à vida do objeto: abre no construtor e libera no destrutor, tanto no retorno normal como na propagação tratada aqui. A cópia de `Sessao` é proibida para evitar duas liberações para uma abertura. C++ padrão não tem `finally`.

**Aplique no fork:** em `include/excecoes.hpp`, complete a liberação no destrutor, a verificação de disponibilidade em `adquirir` e a captura em `executarCiclo`, seguindo o fluxo completo. Mantenha `lerServico` sem captura e preserve seus parâmetros, inclusive `calibrado`. O `using` na exceção reaproveita os construtores de mensagem de `std::runtime_error`.

---

## 5. Python: o mesmo fluxo com finally

A decisão de negócio permanece igual. O que muda é onde a limpeza está escrita: `finally` pertence à operação de aquisição e executa mesmo quando ela retorna ou propaga uma exceção.

Salve como `exemplo_02_excecoes.py` ou [baixe o programa completo](exemplo_02_excecoes.py).

```python
class FonteConstante:
    def valor(self):
        return 42.5


class FalhaLeitura(Exception):
    pass


class Sessao:
    def __init__(self):
        self.abertas = 0

    def abrir(self):
        self.abertas += 1

    def fechar(self):
        self.abertas -= 1


def adquirir(fonte, disponivel, sessao):
    sessao.abrir()
    try:
        if not disponivel:
            raise FalhaLeitura("fonte indisponivel")
        return fonte.valor()
    finally:
        sessao.fechar()


def ler_servico(fonte, disponivel, sessao):
    return adquirir(fonte, disponivel, sessao)


def executar_ciclo(fonte, disponivel, sessao):
    try:
        return True, ler_servico(fonte, disponivel, sessao)
    except FalhaLeitura:
        return False, 0


def mostrar(resultado, sessao):
    sucesso, valor = resultado
    mensagem = f"Leitura: {valor}" if sucesso else "Sem leitura"
    print(f"{mensagem} | sessoes: {sessao.abertas}")


def main():
    fonte = FonteConstante()
    sessao = Sessao()
    primeiro = executar_ciclo(fonte, True, sessao)
    mostrar(primeiro, sessao)
    segundo = executar_ciclo(fonte, False, sessao)
    mostrar(segundo, sessao)
    terceiro = executar_ciclo(fonte, True, sessao)
    mostrar(terceiro, sessao)


if __name__ == "__main__":
    main()
```

```bash
python3 exemplo_02_excecoes.py
```

Saída esperada:

```text
Leitura: 42.5 | sessoes: 0
Sem leitura | sessoes: 0
Leitura: 42.5 | sessoes: 0
```

**Localize os dois papéis:** `except FalhaLeitura` escolhe uma resposta para a falha prevista; `finally` fecha a sessão independentemente dessa resposta. As classes de exceção podem ter corpo `pass`: herdam o comportamento da base e introduzem um tipo próprio; isso não é uma implementação omitida.

**Experimente nos dois programas:** retire temporariamente a limpeza (o decremento do destrutor C++ ou a chamada `sessao.fechar()` Python, mantendo `pass` no bloco `finally`). Os contadores passam a 1, 2 e 3. Restaure a limpeza e confira os três zeros antes de adaptar ao fork.

Em Python, não retorne de dentro de `finally`: isso pode esconder um resultado ou uma exceção. Para arquivos, `with` normalmente expressa a mesma obrigação de fechamento. Coleta de lixo não substitui uma política de liberação de recursos externos no momento adequado. [Python — limpeza com finally](https://docs.python.org/3/tutorial/errors.html#defining-clean-up-actions).

**Aplique no fork:** em `src/excecoes.py`, mantenha abertura antes de `try`, condição e consulta dentro dele e fechamento no `finally`. Complete `executar_ciclo` com a captura da família prevista; preserve `ler_servico`, as assinaturas e o parâmetro `calibrado`. O teste cumulativo ainda aponta a calibração pendente até a extensão seguinte.

---

## 6. Prática de adaptação: calibração e falha inesperada

Adicione a falha específica de calibração, depois da verificação de disponibilidade. Preserve as assinaturas e a chamada intermediária do serviço, que também é utilizada na verificação dos testes autorais da seção 01 da Parte 2.

`make test ETAPA=11` deve confirmar sucesso, dois tipos de falha, captura na fronteira, propagação pelo serviço e contador voltando a zero em todos os caminhos. A suíte inclui uma fonte que lança um defeito fora da família `FalhaLeitura`: esse erro deve escapar da captura, com a sessão já liberada.

| Técnica/Padrão | Melhor uso | Esforço | Entregável | Limitação |
|---|---|---|---|---|
| Retorno falso | rejeição prevista de atualização | baixo | estado preservado e resposta explícita | cliente precisa verificar o retorno |
| Exceção própria | operação não consegue produzir resultado | médio | tipo e contexto de falha | captura genérica pode esconder defeitos |
| RAII / `finally` / `with` | obrigação de liberar recurso | médio | fechamento no sucesso e na falha | limpeza não significa recuperação do negócio |

Se o contador cresce, revise a limpeza. Se o teste espera `FalhaCalibracao` e recebe sucesso, a nova condição não foi implementada. Se o ciclo seguinte falha, procure estado residual. A próxima seção separará também identidade do objeto e igualdade dos seus valores.

## 7. Validação e entrega

```bash
make test ETAPA=11
git add include/excecoes.hpp src/excecoes.py docs/decisoes.md docs/diagrama.md AI_LOG.md
git commit -m "conclui excecoes com contratos cumulativos"
git push -u origin pratica/11-excecoes
```

Faça um commit do incremento guiado e outro da extensão quando ambos forem verificáveis. Abra PR da branch para a `main` **do próprio fork**; confira a execução de `make test ETAPA=11` na CI correspondente ao commit. Integre após testes verdes e revisão. Não abra PR contra o repositório-base.

- [ ] O comando local repete e preserva as etapas anteriores deste starter.
- [ ] A extensão foi adaptada e os casos de fronteira foram explicados.
- [ ] `docs/decisoes.md` relaciona conceito, implementação e evidência.
- [ ] O diagrama corresponde ao estado atual do código.
- [ ] O PR inclui saída local e link da CI do commit.
- [ ] `AI_LOG.md` registra pedido, aceites/rejeições e justificativa, ou declara ausência de IA.

Na main, a CI verifica apenas a baseline executável do starter. A entrega precisa da evidência funcional da branch/PR. Testes visíveis não comprovam entendimento: o docente revisa o diff e, em avaliação, exige defesa oral curta.

Na [seção 11](../12_igualdade_identidade/index.md), definiremos como reconhecer identificadores equivalentes sem confundir dois objetos com uma única instância.

## Perguntas de revisão rápida

1. Por que uma atualização rejeitada pode retornar falso enquanto a aquisição indisponível lança uma exceção?
2. Qual caminho a falha percorre entre adquirir, serviço e ciclo? Onde ocorre a limpeza?
3. Por que capturar a falha prevista não deve esconder um defeito inesperado?

## Fontes de referência

- [Python — erros, exceções e limpeza](https://docs.python.org/3/tutorial/errors.html)
- [C++ — tratamento de exceções](https://eel.is/c++draft/except)
- [C++ Core Guidelines — RAII](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rr-raii)
