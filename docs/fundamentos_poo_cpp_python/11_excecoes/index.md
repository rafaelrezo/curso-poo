# Exceções e recursos: falhar, recuperar e continuar

## Objetivos de aprendizagem

- Distinguir uma atualização rejeitada de uma aquisição que não produz leitura.
- Acompanhar lançamento, propagação e captura de uma exceção.
- Explicar a limpeza de recursos e aplicar os conceitos dos capítulos 09 e 10 em uma atividade integrada.

**Tempo estimado:** 2h em sala, com aproximadamente 80 min de exposição dialogada e 40 min para iniciar a prática A; até 1h para a prática completa (40 min em sala e cerca de 20 min de conclusão orientada), incluindo preparação, validação e revisão. Confira o [planejamento](../../index.md). O vídeo é complementar dentro desse tempo.

## Vídeo de contexto

![type:video](https://www.youtube.com/embed/RHSxIKGCX7c)

Otávio Miranda — Try, Except, Tratando Exceções em Python. Procure onde o fluxo normal é interrompido e quem decide o que fazer depois.

---

## 1. A consulta pode não produzir um número

No capítulo 09, o cliente consultou uma fonte real e uma simulada. Agora a aquisição pode estar indisponível. Retornar zero seria ambíguo: zero também é uma leitura válida de nível.

Há dois contratos diferentes: `SensorNivel.atualizar` rejeita uma entrada inválida retornando falso e preservando o estado; `adquirir` precisa comunicar que não conseguiu obter uma leitura. Não vamos mudar silenciosamente a regra do sensor.

## 2. Interromper a operação e tratar a falha no cliente

A fonte constante continua produzindo 42,5. A disponibilidade é simulada por um booleano para observar o fluxo sem introduzir rede. Preveja os três ciclos: disponível, indisponível e disponível outra vez.

Programa independente: [exemplo_03_falha.cpp](exemplo_03_falha.cpp). Salve em uma pasta de demonstrações.

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

double adquirir(const FonteConstante& fonte, bool disponivel) {
    if (!disponivel) throw FalhaLeitura("fonte indisponivel");
    return fonte.valor();
}

int main() {
    FonteConstante fonte;
    for (bool disponivel : {true, false, true}) {
        try {
            const double valor = adquirir(fonte, disponivel);
            std::cout << "Leitura: " << valor << '\n';
        } catch (const FalhaLeitura& erro) {
            std::cout << "Sem leitura: " << erro.what() << '\n';
        }
    }
}
```

Execute:

```bash
g++ -std=c++17 -Wall -Wextra -Werror -pedantic exemplo_03_falha.cpp -o exemplo_03_falha
./exemplo_03_falha
```

Resultado:

```text
Leitura: 42.5
Sem leitura: fonte indisponivel
Leitura: 42.5
```

No segundo ciclo, `throw` interrompe `adquirir` antes de `return fonte.valor()`. A execução procura uma captura compatível; o `catch` no cliente explica a ausência. O laço continua no terceiro ciclo.

`FalhaLeitura` é um tipo próprio derivado de `std::runtime_error`; o `using` reutiliza seus construtores de mensagem. Capturar por referência constante evita copiar o objeto de exceção. O resultado só é apresentado depois que a aquisição termina com sucesso.

## 3. A falha atravessa quem não pode resolvê-la

Acrescentamos um serviço entre cliente e aquisição. Ele coordena a chamada, mas não sabe apresentar uma resposta ao operador. Por isso deixa a exceção propagar.

Programa independente: [exemplo_04_propagacao.cpp](exemplo_04_propagacao.cpp). Salve em uma pasta de demonstrações.

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

double adquirir(const FonteConstante& fonte, bool disponivel) {
    if (!disponivel) throw FalhaLeitura("fonte indisponivel");
    return fonte.valor();
}

double lerServico(const FonteConstante& fonte, bool disponivel) {
    return adquirir(fonte, disponivel);
}

int main() {
    FonteConstante fonte;
    for (bool disponivel : {true, false, true}) {
        try {
            const double valor = lerServico(fonte, disponivel);
            std::cout << "Leitura: " << valor << '\n';
        } catch (const FalhaLeitura& erro) {
            std::cout << "Sem leitura: " << erro.what() << '\n';
        }
    }
}
```

Execute:

```bash
g++ -std=c++17 -Wall -Wextra -Werror -pedantic exemplo_04_propagacao.cpp -o exemplo_04_propagacao
./exemplo_04_propagacao
```

Resultado:

```text
Leitura: 42.5
Sem leitura: fonte indisponivel
Leitura: 42.5
```

A saída permanece igual. No caminho normal, o número retorna por `lerServico`; na falha, nenhum desses retornos produz valor. A captura continua no cliente.

```text
main -> lerServico -> adquirir
  ^                      |
  +---- FalhaLeitura ----+
```

**Explique:** colocar um `catch` no serviço apenas para devolver zero esconderia qual informação? A fronteira de recuperação deve estar onde existe uma decisão útil; não é necessário capturar em toda função.

## 4. O recurso precisa ser liberado mesmo sem retorno normal

Imagine que a aquisição abra uma sessão. Primeiro surge a obrigação: cada abertura deve ter uma liberação, inclusive quando a chamada falhar. Vamos representar a sessão por um contador e conferir que ele volta a zero.

No programa completo, `executarCiclo` assume a captura antes feita pelo `main`. A classe `Sessao` controla o recurso. `lerServico` conserva seu papel de propagação.

**Antes de ler o código:** `std::runtime_error`, declarado em `<stdexcept>`, é um tipo de exceção da biblioteca padrão que guarda uma mensagem acessível por `what()`. `FalhaLeitura` deriva dele para dar um nome específico à falha da estação; `using` reaproveita o construtor que recebe a mensagem. Consulte o [manual de `std::runtime_error`](https://en.cppreference.com/cpp/error/runtime_error) para os membros e construtores. A mesma classe já apareceu nas seções 2 e 3; aqui ela permanece igual, e a novidade é o controle da sessão.

Leia primeiro `main -> executarCiclo -> lerServico -> adquirir`. Depois localize `Sessao sessao{abertas}` e acompanhe o contador nos caminhos de retorno e de exceção. Em `int& abertas`, o `&` declara uma **referência C++** ao `int` original: incrementar `abertas_` altera o contador criado no `main`, sem devolver outro valor nem fazer uma cópia. Isso é diferente do ponteiro de C e do operador `&` usado para obter endereço. A referência precisa ser inicializada ao criar o objeto e o `main` mantém o contador vivo durante todos os ciclos.

Programa independente: [exemplo_01_excecoes.cpp](exemplo_01_excecoes.cpp). Salve em uma pasta de demonstrações.

```cpp
#include <iostream>
#include <stdexcept>

class FonteConstante {
public:
    double valor() const { return 42.5; }
};

class FalhaLeitura : public std::runtime_error { // Tipo padrao de falha, em <stdexcept>.
public:
    using std::runtime_error::runtime_error; // Reutiliza o construtor com mensagem.
};

class Sessao {
    int& abertas_; // Referencia ao contador do main; nao cria uma copia.
public:
    // explicit impede conversao automatica de int para Sessao.
    // : abertas_(abertas) liga a referencia ao contador antes do corpo.
    explicit Sessao(int& abertas) : abertas_(abertas) { ++abertas_; }
    ~Sessao() { --abertas_; } // Ao sair do escopo, libera a sessao.
    Sessao(const Sessao&) = delete; // Proibe criar outra Sessao copiando esta.
    Sessao& operator=(const Sessao&) = delete; // Proibe atribuir uma Sessao a outra.
};

double adquirir(const FonteConstante& fonte, bool disponivel, int& abertas) {
    Sessao sessao{abertas}; // Abre agora; o destrutor fecha no retorno ou no throw.
    std::cout << "Durante aquisicao | sessoes: " << abertas << '\n';
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
    } catch (const FalhaLeitura&) { // Trata apenas a falha prevista.
        return {false, 0}; // O bool marca ausencia; zero nao e uma leitura.
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
    // auto deduz ResultadoLeitura, o tipo devolvido por executarCiclo.
    auto primeiro = executarCiclo(fonte, true, abertas);
    mostrar(primeiro, abertas);
    auto segundo = executarCiclo(fonte, false, abertas);
    mostrar(segundo, abertas);
    auto terceiro = executarCiclo(fonte, true, abertas);
    mostrar(terceiro, abertas);
}
```

Execute:

```bash
g++ -std=c++17 -Wall -Wextra -Werror -pedantic exemplo_01_excecoes.cpp -o exemplo_01_excecoes
./exemplo_01_excecoes
```

Resultado:

```text
Durante aquisicao | sessoes: 1
Leitura: 42.5 | sessoes: 0
Durante aquisicao | sessoes: 1
Sem leitura | sessoes: 0
Durante aquisicao | sessoes: 1
Leitura: 42.5 | sessoes: 0
```

Na linha do construtor, `explicit` impede uma conversão automática de `int` para `Sessao`. A parte `: abertas_(abertas)` é a **lista de inicialização**: liga o membro de referência ao contador recebido antes de executar `{ ++abertas_; }`. O prefixo `~` marca o destrutor, chamado quando `sessao` deixa o escopo. As duas linhas com `= delete` proíbem operações diferentes: `Sessao outra = sessao;` tentaria **criar** uma cópia; `outra = sessao;` tentaria **atribuir** o estado de uma sessão a outra já existente. Se uma cópia compartilhasse `abertas_`, seu destrutor também faria `--abertas_`. Uma única abertura poderia terminar com duas diminuições. Por isso esta classe representa uma sessão única e não pode ser copiada nem atribuída. Essas linhas são uma proteção do exemplo; para acompanhar o fluxo principal, basta saber que não criamos uma segunda `Sessao` a partir da primeira. Consulte também o [manual de referências](https://en.cppreference.com/cpp/language/reference) e o [manual de construtores e listas de inicialização](https://en.cppreference.com/cpp/language/constructor).

Em `auto primeiro`, o compilador deduz o tipo `ResultadoLeitura` a partir do retorno de `executarCiclo`; `segundo` e `terceiro` têm o mesmo tipo. A dedução acontece na compilação: `auto` não permite que a variável mude de tipo depois. Escrever `ResultadoLeitura primeiro = executarCiclo(...)` teria o mesmo tipo explícito.

No sucesso, sair de `adquirir` destrói a sessão local. Na propagação da falha até a captura, o mesmo destrutor é executado ao abandonar esse escopo. Esse uso de **RAII** associa a duração do recurso à vida do objeto. A cópia de `Sessao` é proibida para não criar duas liberações para uma abertura.

O resultado `{false, 0}` comunica ausência pelo campo `sucesso`; esse zero não é uma medição. `mostrar` verifica a condição antes de ler o número. O contador mostra `1` enquanto `adquirir` mantém a sessão aberta e `0` após cada ciclo. Mesmo no ciclo que lança a exceção, o destrutor reduz o contador antes de `mostrar` executar. O terceiro ciclo confirma que a falha anterior não deixou uma sessão aberta.

**Demonstração de defeito:** retire apenas o decremento no destrutor. As linhas após os ciclos passam a mostrar 1, 2 e 3, enquanto as linhas durante a aquisição mostram 1, 2 e 3. Restaure e confira os zeros. A limpeza resolve o recurso; a captura resolve a resposta ao operador.

## 5. Python: a mesma obrigação com finally

Mantenha os três ciclos. Há **dois blocos `try` em funções diferentes**: em `adquirir`, `finally` fecha a sessão sempre que o bloco termina, com `return` ou com `raise`; em `executar_ciclo`, `except FalhaLeitura` reconhece somente a falha prevista e decide devolver “sem leitura”. `finally` não captura nem transforma a exceção. Na falha, a ordem é `raise` em `adquirir` → fechamento no `finally` → propagação por `ler_servico` → tratamento no `except` do cliente. No sucesso, o `finally` também executa antes de o valor chegar ao cliente.

Programa independente: [exemplo_02_excecoes.py](exemplo_02_excecoes.py). Salve em uma pasta de demonstrações.

```python
class FonteConstante:
    def valor(self):
        return 42.5


class FalhaLeitura(Exception):  # Tipo proprio para uma falha prevista de aquisicao.
    pass  # A classe herda o comportamento de Exception; nao precisa de metodo novo.


class Sessao:
    def __init__(self):
        self.abertas = 0

    def abrir(self):
        self.abertas += 1

    def fechar(self):
        self.abertas -= 1


def adquirir(fonte, disponivel, sessao):
    sessao.abrir()  # A partir daqui ha uma sessao ativa.
    print(f"Durante aquisicao | sessoes: {sessao.abertas}")
    try:
        if not disponivel:
            raise FalhaLeitura("fonte indisponivel")  # Interrompe a aquisicao.
        return fonte.valor()
    finally:
        sessao.fechar()  # Executa no retorno normal e antes de propagar a falha.


def ler_servico(fonte, disponivel, sessao):
    return adquirir(fonte, disponivel, sessao)  # Nao captura: a falha sobe ao cliente.


def executar_ciclo(fonte, disponivel, sessao):
    try:
        return True, ler_servico(fonte, disponivel, sessao)
    except FalhaLeitura:  # So a falha prevista vira "sem leitura".
        return False, 0  # O False marca ausencia; zero nao e uma medicao.


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

Execute:

```bash
python3 exemplo_02_excecoes.py
```

Resultado:

```text
Durante aquisicao | sessoes: 1
Leitura: 42.5 | sessoes: 0
Durante aquisicao | sessoes: 1
Sem leitura | sessoes: 0
Durante aquisicao | sessoes: 1
Leitura: 42.5 | sessoes: 0
```

O `print` dentro de `adquirir` mostra `1` antes do retorno ou da falha; o `finally` fecha a sessão antes de `mostrar`, que então vê `0`. `FalhaLeitura(Exception)` nomeia a falha prevista; `pass` deixa a classe sem comportamento adicional. `raise` inicia a propagação. O `except` está no cliente porque é ali que existe uma resposta útil para o operador. Um erro de outro tipo continuaria a propagar, mas a sessão ainda seria fechada. O fechamento acontece quando a operação retorna ou propaga uma exceção. Não retorne dentro de `finally`, pois isso pode suprimir um resultado ou uma falha. Para recursos como arquivos, `with` normalmente reúne aquisição e fechamento em uma estrutura própria; coleta de lixo não é uma política de fechamento oportuno.

## 6. Qual falha o cliente pode recuperar?

Uma fonte pode estar indisponível ou precisar de calibração. Podemos especializar `FalhaLeitura` com `FalhaCalibracao`; capturar a família prevista continua permitindo que o próximo ciclo execute. A prática usará essa extensão, sem alterar o contrato de atualização do sensor.

| Técnica/Padrão | Melhor uso | Esforço | Entregável | Limitação |
|---|---|---|---|---|
| Retorno falso | rejeição prevista de uma atualização | baixo | estado preservado e resposta | o cliente precisa conferir o retorno |
| Exceção própria | aquisição que não consegue produzir leitura | médio | tipo de falha e propagação | captura ampla pode esconder defeitos |
| RAII em C++ | associar recurso ao tempo de vida do objeto | médio | liberação ao sair do escopo | limpeza não define recuperação |
| `finally` / `with` em Python | garantir a liberação na saída da operação | médio | fechamento no sucesso e na falha | não substitui o tratamento do problema |

Na estação, capture `FalhaLeitura` na fronteira. Um erro inesperado de implementação deve propagar depois da limpeza; convertê-lo em “sensor indisponível” dificultaria o diagnóstico.

---

## 7. Prática integrada A — adquirir e recuperar uma leitura

Esta é a **única entrega dos capítulos 09 e 10**, planejada para **1h de estudo** com o fluxo de entrega já conhecido. Use o [starter independente](https://github.com/rafaelrezo/poo-colaboracao-excecoes) e o [guia da prática](https://rafaelrezo.github.io/poo-colaboracao-excecoes/). O painel, as duas fontes, a interface e a limpeza de sessão estão fornecidos. O aluno implementa, em C++ e Python, duas decisões pequenas do capítulo 10: **lançar a falha específica** e **capturá-la na fronteira que responde ao operador**. Assim a prática exercita o mecanismo sem exigir reescrever o exemplo do capítulo 09 nem migrar um fork antigo.

### 7.1 Observar o contrato — cerca de 10 min

Faça fork do repositório-base público, clone **o seu fork** e mantenha apenas `origin` apontando para ele. Crie a branch:

```bash
git remote -v
git switch -c pratica/integrada-a
make run
make test ETAPA=A
```

O programa inicial compila. `make run` mostra `Painel: 20`, `Fonte simulada: 42.5 %` e `Sem calibracao: leitura indevida: 20`: colaboração e fonte substituível funcionam, mas ainda falta rejeitar a aquisição sem calibração. O primeiro teste aponta `GUIADO: falta de calibracao deve lancar FalhaCalibracao`. No C++, `ResultadoLeitura` guarda `sucesso`, `valor` e `motivo`; Python devolve as mesmas três informações em uma tupla. O motivo distingue falhas previstas sem tratar zero como medição.

### 7.2 Incremento guiado — lançar a falha, cerca de 15 min

Abra `include/estacao.hpp` e `src/estacao.py`. Em `adquirir`, complete a condição marcada: se a fonte estiver disponível, mas não calibrada, lance `FalhaCalibracao`. Preserve a verificação de indisponibilidade **antes** dessa condição. Não altere `Sessao`, `finally`, `lerServico` nem os testes.

Execute `make test ETAPA=A`. Agora a falha específica é lançada, mas a captura geral ainda informa `indisponivel`. Essa mensagem é o problema seguinte. Registre o primeiro avanço:

```bash
git add include/estacao.hpp src/estacao.py
git commit -m "lanca falha especifica para falta de calibracao"
```

### 7.3 Extensão — recuperar sem esconder a causa, cerca de 15 min

Em `executarCiclo` e `executar_ciclo`, acrescente uma captura de `FalhaCalibracao` **antes** da captura de `FalhaLeitura`. Devolva `sucesso` falso, valor zero sem significado de medição e motivo `calibracao`. A captura geral continua responsável por `indisponivel`. Como `FalhaCalibracao` deriva de `FalhaLeitura`, inverter a ordem impediria distinguir a causa.

| Situação | Resultado esperado |
|---|---|
| disponível e calibrada | leitura atual; sessão liberada |
| indisponível, com ou sem calibração | motivo `indisponivel`; sessão liberada |
| disponível, sem calibração | motivo `calibracao`; sessão liberada |
| defeito inesperado da fonte | propagação; sessão liberada |
| ciclo seguinte válido | nova leitura normal |

Execute `make run`: a linha deve ser `Sem calibracao: sem leitura (calibracao) | sessoes: 0`, seguida por `Ciclo seguinte: leitura disponivel: 20 | sessoes: 0`. Depois execute **o mesmo teste**, `make test ETAPA=A`, até obter `OK pratica integrada A` em C++ e Python.

### 7.4 Explicar e entregar — cerca de 20 min

Em `docs/decisoes.md`, siga o caminho `adquirir → lerServico → executarCiclo` em C++ e o caminho equivalente em Python. Explique por que o recurso é liberado antes da captura, por que a captura específica vem primeiro e como o mesmo contrato aceita as duas fontes. Em `AI_LOG.md`, registre pedidos à IA, aceites/rejeições e justificativa, ou declare ausência de IA.

```bash
git add include/estacao.hpp src/estacao.py docs/decisoes.md AI_LOG.md
git commit -m "distingue a falha recuperada no cliente"
git push -u origin pratica/integrada-a
```

Abra **uma PR da branch para a `main` do próprio fork**. A CI executa `make test ETAPA=A` após o push e na PR; inclua a saída local, o link da CI do commit e sua explicação. Integre após revisar o diff e obter testes verdes. Se Actions estiver desativado no fork, habilite os workflows na aba Actions e envie o commit seguinte. Testes visíveis são complementados por revisão do diff e, em avaliação, defesa oral curta. Não abra PR contra o repositório-base.

No [capítulo 11](../12_igualdade_identidade/index.md), passaremos da consulta individual ao cadastro de várias medições. O novo problema será decidir quando dois identificadores representam a mesma chave.

## Perguntas de revisão rápida

1. Por que zero não comunica, sozinho, que uma aquisição falhou?
2. Qual função lança, qual propaga e qual captura? Em que momento o recurso é liberado?
3. Por que a falha prevista pode ser tratada sem esconder um defeito inesperado da fonte?

## Fontes de referência

- [Python — erros, exceções e limpeza](https://docs.python.org/3/tutorial/errors.html).
- [C++ — tratamento de exceções](https://eel.is/c++draft/except).
- [C++ Core Guidelines — RAII](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rr-raii).
- [GitHub Docs — eventos de workflows](https://docs.github.com/en/actions/reference/workflows-and-actions/events-that-trigger-workflows).
- [GitHub Docs — sintaxe e permissões](https://docs.github.com/en/actions/reference/workflows-and-actions/workflow-syntax).
