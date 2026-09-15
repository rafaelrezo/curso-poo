# Exceções e recursos: falhar, recuperar e continuar

## Objetivos de aprendizagem

- Distinguir uma atualização rejeitada de uma aquisição que não produz leitura.
- Acompanhar lançamento, propagação e captura de uma exceção.
- Explicar a limpeza de recursos e aplicar os conceitos dos capítulos 09 e 10 em uma atividade integrada.

**Tempo estimado:** 2h em sala, com aproximadamente 80 min de exposição dialogada e 40 min para iniciar a prática A; até 2h de trabalho orientado para concluí-la, incluindo preparação e revisão. Confira o [planejamento](../../index.md). O vídeo é complementar dentro desse tempo.

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

Programa independente: [exemplo_01_excecoes.cpp](exemplo_01_excecoes.cpp). Salve em uma pasta de demonstrações.

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

Execute:

```bash
g++ -std=c++17 -Wall -Wextra -Werror -pedantic exemplo_01_excecoes.cpp -o exemplo_01_excecoes
./exemplo_01_excecoes
```

Resultado:

```text
Leitura: 42.5 | sessoes: 0
Sem leitura | sessoes: 0
Leitura: 42.5 | sessoes: 0
```

No sucesso, sair de `adquirir` destrói a sessão local. Na propagação da falha até a captura, o mesmo destrutor é executado ao abandonar esse escopo. Esse uso de **RAII** associa a duração do recurso à vida do objeto. A cópia de `Sessao` é proibida para não criar duas liberações para uma abertura.

O resultado `{false, 0}` comunica ausência pelo campo `sucesso`; esse zero não é uma medição. `mostrar` verifica a condição antes de ler o número. O terceiro ciclo confirma que a falha anterior não deixou uma sessão aberta.

**Demonstração de defeito:** retire apenas o decremento no destrutor. Os contadores passam a 1, 2 e 3. Restaure e confira os zeros. A limpeza resolve o recurso; a captura resolve a resposta ao operador.

## 5. Python: a mesma obrigação com finally

Mantenha os três ciclos e acompanhe as duas responsabilidades: `except` trata uma falha prevista; `finally` fecha a sessão ao deixar a aquisição.

Programa independente: [exemplo_02_excecoes.py](exemplo_02_excecoes.py). Salve em uma pasta de demonstrações.

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

Execute:

```bash
python3 exemplo_02_excecoes.py
```

Resultado:

```text
Leitura: 42.5 | sessoes: 0
Sem leitura | sessoes: 0
Leitura: 42.5 | sessoes: 0
```

O fechamento acontece quando a operação retorna ou propaga uma exceção. Não retorne dentro de `finally`, pois isso pode suprimir um resultado ou uma falha. Para recursos como arquivos, `with` normalmente reúne aquisição e fechamento em uma estrutura própria; coleta de lixo não é uma política de fechamento oportuno.

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

## 7. Prática integrada A — consultar e adquirir com segurança

Esta é a **única entrega dos capítulos 09 e 10**. Trabalhe no fork de [rafaelrezo/poo-fundamentos-estacao](https://github.com/rafaelrezo/poo-fundamentos-estacao) iniciado no capítulo 07, com herança validada e integrada. O repositório do capítulo 08 permanece separado.

### 7.1 Retomar uma base executável

O starter atualizado fornece interface, fontes, troca de vínculo, bancada, sessão, falha de indisponibilidade e captura. Você implementará apenas a consulta do painel e a extensão de calibração, em C++ e Python. Leia os arquivos fornecidos: eles concretizam as demonstrações, mas conservam as assinaturas do projeto.

Se seu fork ainda usa o roteiro antigo, siga primeiro o [guia de atualização](https://github.com/rafaelrezo/poo-fundamentos-estacao/blob/main/ATUALIZACAO.md), que preserva o código já escrito. Não substitua o fork inteiro por um starter novo.

```bash
git switch main
git pull --ff-only origin main
git remote -v
git switch -c pratica/integrada-a
make test ETAPA=A
```

Se o guia de atualização já criou `pratica/integrada-a`, continue nela e não repita o bloco de criação acima; execute apenas o teste.

O remoto único deve ser `origin`, apontando para seu fork. O teste compila o programa e repete os contratos anteriores. No starter atualizado, a primeira falha desta prática pede a consulta ao sensor associado. Se a mensagem pedir a tag na classe-base, falta concluir o capítulo 07.

### 7.2 Incremento guiado — a leitura acompanha o objeto

Abra `include/relacoes.hpp` e `src/relacoes.py`. No método `PainelFixo.leitura`, aplique a consulta demonstrada na seção 2 do capítulo 09: use o sensor guardado pelo painel a cada chamada. A construção e a troca de vínculo já estão fornecidas. Não copie a classe reduzida da exposição sobre o sensor validado do starter.

Execute `make test ETAPA=A`. Os testes de vínculos e interfaces devem passar; a primeira pendência agora é a calibração. Faça um commit desse avanço, mantendo a branch aberta:

```bash
git add include/relacoes.hpp src/relacoes.py
git commit -m "consulta a leitura atual do sensor associado"
```

### 7.3 Extensão — identificar a falta de calibração

Em `include/excecoes.hpp` e `src/excecoes.py`, complete a condição pendente de `adquirir`. A classe `FalhaCalibracao` já existe. Decida onde lançar essa falha respeitando a ordem abaixo; preserve a abertura, a limpeza, o serviço e a captura fornecidos.

| Situação | Comportamento esperado |
|---|---|
| disponível e calibrada | devolver a leitura atual |
| indisponível | propagar `FalhaLeitura` |
| disponível, sem calibração | propagar `FalhaCalibracao` |
| indisponível e sem calibração | priorizar indisponibilidade |
| captura da falha prevista | resultado sem leitura e próxima consulta possível |
| defeito inesperado da fonte | propagar o defeito, com recurso já liberado |

Execute novamente **o mesmo comando**, `make test ETAPA=A`. O fechamento esperado é `OK pratica integrada A (C++ e Python)`. Os contratos incluem consulta compartilhada, propagação e contador de sessões zerado. A extensão exige adaptação: sua solução não está no starter.

### 7.4 Revisão e entrega única

Registre em `docs/decisoes.md` por que o painel não copia a leitura e onde a falha é recuperada. Em `AI_LOG.md`, registre pedidos à IA, aceites/rejeições e justificativa, ou declare ausência de IA.

```bash
git add include/excecoes.hpp src/excecoes.py docs/decisoes.md AI_LOG.md
git commit -m "trata calibracao preservando propagacao e limpeza"
git push -u origin pratica/integrada-a
```

Abra **uma PR da branch para a `main` do próprio fork**. Se fizer outros pushes durante o trabalho, a CI executará o mesmo `make test ETAPA=A`; uma falha durante a extensão é feedback de comportamento pendente. Integre somente após concluir a atividade, revisar e obter testes verdes.

- [ ] Consulta e aquisição observam o mesmo sensor atualizado.
- [ ] A falha de calibração tem o tipo e a prioridade corretos.
- [ ] O próximo ciclo funciona e a sessão termina liberada.
- [ ] A PR registra a saída local, o link da CI do commit e a explicação técnica.
- [ ] O diff preserva testes e automação, e a rastreabilidade de IA está registrada.

Se Actions estiver desativado no fork, habilite os workflows na aba Actions e envie o commit seguinte. A execução na `main` verifica apenas a baseline; a evidência funcional é a CI da branch/PR. Testes visíveis precisam ser complementados por revisão do diff e, em avaliação, defesa oral curta. Não abra PR contra o repositório-base.

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
