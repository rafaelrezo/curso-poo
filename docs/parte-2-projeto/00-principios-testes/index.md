# 01. Princípios de projeto e testes de objetos: justificar e verificar a colaboração

## Objetivos de aprendizagem

- Avaliar coesão, acoplamento e responsabilidade em um modelo pequeno.
- Refatorar um cliente para depender de abstrações e delegar sua política.
- Escrever testes de estado, fronteira e exceção que detectem defeitos reais.

**Tempo estimado:** 2h de estudo e prática. O vídeo é preparação prévia; confira a distribuição entre sala e prática no [roteiro da trilha](../../index.md).

## Vídeo de contexto

![type:video](https://www.youtube.com/embed/IV0Co31JYD8)

Testes unitários em Python com unittest. Observe a preparação do objeto, a ação e a verificação. Nesta prática, começaremos com asserts pequenos em C++ e Python; o formato unittest pode organizar a extensão.

---

## 1. Mini-caso prático: um controlador faz coisas demais

Um painel que lê o sensor, decide a regra de alerta, formata texto e salva dados mistura razões diferentes para mudar. Trocar a fonte ou o limite pode obrigar a editar o mesmo bloco.

Nesta aula, o problema é menor e verificável: `ControladorConsulta` consulta uma `IFonteLeitura` e delega a decisão a `PoliticaAlarme`. A política é estrita: alerta quando `valor > limite`. Igualdade ao limite não alerta.

Antes de implementar, registre como mudariam os arquivos se fosse necessário trocar apenas a fonte, apenas o limite ou apenas a apresentação. Use isso como evidência de acoplamento, não apenas a quantidade de classes.

---

## 2. Retome o artefato e abra a branch

Use o próprio fork de [rafaelrezo/poo-fundamentos-estacao](https://github.com/rafaelrezo/poo-fundamentos-estacao), iniciado no capítulo 07. A prática de programação do capítulo 11 agora tem [repositório independente](https://github.com/rafaelrezo/poo-identidade-colecoes); o capítulo 12 trabalha somente modelagem.

**Ponte entre os repositórios:** se seu fork de fundamentos ainda tem os `TODO B` no catálogo, reaproveite os corpos de `inserir`, `buscar` e `remover` que você já concluiu no 11, nas classes `Catalogo` de `include/colecoes.hpp` e `src/colecoes.py`. Preserve o restante desses arquivos: o starter de fundamentos também contém operações com fontes que não existem na prática independente. Não substitua os arquivos inteiros nem copie o histórico. As assinaturas e a política de duplicatas são compatíveis.

Faça essa adaptação na branch `projeto/00-testes`, valide com `make test ETAPA=B` e registre um commit de preparação antes de implementar o controlador. `B` permanece como identificador técnico da base de testes do starter antigo; **não representa uma nova entrega nem a antiga prática integrada 11+12**. Se o catálogo já está implementado, basta conferir esse teste. Reveja seu diagrama do capítulo 12 para apoiar a leitura das relações.

**Fork antigo com `TODO A`:** a base publicada de fundamentos pode ainda ter pendências de painel e calibração, mesmo que você tenha concluído a prática A em seu repositório independente. Em `include/relacoes.hpp` e `src/relacoes.py`, faça `PainelFixo.leitura` consultar o sensor associado (`sensor_->valor()` / `self._sensor.valor()`). Em `adquirir`, nos arquivos de exceções, preserve a rejeição de indisponibilidade e lance `FalhaCalibracao` quando faltar calibração, como no capítulo 10. Preserve a interface e o retorno antigos de `executarCiclo`/`executar_ciclo`; não copie a classe inteira do outro starter. Isso recompõe a infraestrutura já estudada, sem nova atividade. Falhas da etapa 07 indicam que a implementação anterior de sensores também precisa estar concluída.

O clone mantém somente `origin` apontando para seu fork. Não copie arquivos dos repositórios das seções 01–06.

```bash
git switch main
git pull --ff-only origin main
git remote -v
git switch -c projeto/00-testes
# Se necessário, adapte os três métodos do catálogo antes de testar.
make test ETAPA=B
make test-projeto
```

O alvo `test-projeto` executa a etapa técnica 14 (seu identificador foi preservado), repetindo os contratos anteriores e inicialmente falha no comportamento ainda pendente desta seção. Leia a primeira mensagem; não altere testes ou automação para obter aprovação. Complete os incrementos abaixo e repita o mesmo comando.

---

## 3. Critérios para avaliar o projeto

| Técnica/Padrão | Melhor uso | Esforço | Entregável | Limitação |
|---|---|---|---|---|
| Responsabilidade definida | separar motivos de mudança | médio | classe com propósito claro | dividir em classes demais também dificulta leitura |
| Alta coesão | aproximar operações que trabalham no mesmo propósito | médio | estado e comportamento relacionados | nome de classe não comprova coesão |
| Baixo acoplamento | conter o impacto das mudanças | médio | dependências pequenas e explícitas | abstrações excessivas também têm custo |
| Composição | coordenar colaboradores | médio | política e fonte separadas | requer definir posse e tempo de vida |
| Herança substituível | implementar um contrato de tipo | médio | implementação utilizável pelo cliente | não serve para qualquer reúso |

O controlador deve **ter** uma política e conhecer uma fonte, sem herdar de sensor. O teste deve poder fornecer uma fonte pequena e controlada, sem rede, arquivo ou equipamento.

---

## 4. Incremento guiado: delegar e escrever um teste

O programa abaixo reúne uma fonte controlada, uma política, o controlador e o primeiro teste no main. Leia preparar→agir→verificar. O recorte independente usa apenas `valor`; no fork, a interface também exige `unidade`, que deve continuar implementada pelo dublê.

[Baixe `exemplo_01_teste_objeto.cpp`](exemplo_01_teste_objeto.cpp) ou salve o programa completo:

```cpp
#include <cassert>
#include <iostream>

class IFonteLeitura {
public:
    virtual ~IFonteLeitura() = default;
    virtual double valor() const = 0;
};

class FonteTeste : public IFonteLeitura {
public:
    double valor() const override { return 10; }
};

class PoliticaAlarme {
    double limite_;
public:
    explicit PoliticaAlarme(double limite) : limite_(limite) {}
    bool aciona(double valor) const { return valor > limite_; }
};

class ControladorConsulta {
    const IFonteLeitura& fonte_;
    const PoliticaAlarme& politica_;
public:
    ControladorConsulta(const IFonteLeitura& fonte, const PoliticaAlarme& politica)
        : fonte_(fonte), politica_(politica) {}
    bool avaliar() const { return politica_.aciona(fonte_.valor()); }
};

int main() {
    FonteTeste fonte;
    PoliticaAlarme politica{10};
    ControladorConsulta controlador{fonte, politica};
    const bool alerta = controlador.avaliar();
    assert(!alerta);
    std::cout << "OK: igualdade ao limite nao alerta\n";
}
```

```bash
g++ -std=c++17 -Wall -Wextra -Werror -pedantic exemplo_01_teste_objeto.cpp -o exemplo
./exemplo
```

Saída esperada:

```text
OK: igualdade ao limite nao alerta
```

[Baixe `exemplo_02_teste_objeto.py`](exemplo_02_teste_objeto.py) ou salve o programa completo:

```python
from abc import ABC, abstractmethod


class IFonteLeitura(ABC):
    @abstractmethod
    def valor(self):
        raise NotImplementedError


class FonteTeste(IFonteLeitura):
    def valor(self):
        return 10


class PoliticaAlarme:
    def __init__(self, limite):
        self._limite = limite

    def aciona(self, valor):
        return valor > self._limite


class ControladorConsulta:
    def __init__(self, fonte, politica):
        self._fonte = fonte
        self._politica = politica

    def avaliar(self):
        return self._politica.aciona(self._fonte.valor())


def main():
    fonte = FonteTeste()
    politica = PoliticaAlarme(10)
    controlador = ControladorConsulta(fonte, politica)
    alerta = controlador.avaliar()
    assert not alerta
    print("OK: igualdade ao limite nao alerta")


if __name__ == "__main__":
    main()
```

```bash
python3 exemplo_02_teste_objeto.py
```

Saída esperada:

```text
OK: igualdade ao limite nao alerta
```

**Aplique no fork:** complete `avaliar` em `include/projeto.hpp` e `src/projeto.py` com a delegação mostrada. Em `tests/aluno.cpp` e `tests/aluno.py`, use os imports/includes das classes do projeto, crie seu dublê implementando as duas operações da interface e adapte o teste de fronteira. Não copie redefinições de classes já existentes para os testes.

Execute `make test-aluno`. Esse primeiro teste passa, mas sozinho não comprova comportamento acima do limite nem propagação de falhas. A próxima prática amplia a evidência.

---

## 5. Prática de adaptação: completar a evidência

Acrescente testes próprios nas duas linguagens:

1. abaixo, exatamente no limite e acima dele;
2. controlador recebe uma fonte de teste, observa mudança da leitura e reavalia;
3. aquisição indisponível propaga `FalhaLeitura` através do serviço;
4. sessão está fechada após a falha;
5. um ciclo válido funciona depois da falha anterior.

Para o segundo caso, defina uma implementação pequena de `IFonteLeitura` cujo valor você controla. É um dublê de teste: substitui a dependência para tornar o cenário reproduzível. Não use relógio, rede ou aleatoriedade nesse teste de unidade.

Na exceção C++, use uma flag inicializada como falsa, capture o tipo esperado e verifique a flag após a chamada. Se nenhuma exceção ocorrer, o teste deve falhar. Em Python, use `try/except/else` ou um teste `unittest` com `assertRaises`. Apenas capturar uma exceção sem verificar que ela era obrigatória cria um teste que também aceita o sucesso indevido.

Cada teste deve explicar uma regra. Não leia o texto do fonte para procurar `if`, `assert` ou nomes de métodos: isso não verifica o comportamento.

---

## 6. Como saber se meus testes detectam defeitos?

```bash
make test-aluno
make test-projeto
```

O segundo comando repete todos os contratos e executa seus testes. Depois cria cópias temporárias e introduz seis defeitos, três por linguagem: fronteira `>` alterada para `>=`, alarme que nunca dispara e serviço que deixa de propagar a falha de aquisição.

O fonte original não é modificado. A ferramenta espera que seus testes passem na implementação normal e falhem em cada cópia defeituosa. Se um mutante C++ não compilar, isso é tratado como erro de validação, não como prova de um bom teste.

Uma mensagem como `seus testes nao detectaram fronteira Python` indica o caso que falta. Ao final, devem aparecer confirmações de detecção dos seis defeitos. Não altere a política, o serviço intermediário ou a ferramenta para contornar essa verificação; esses pontos documentados do esqueleto são usados nas mutações.

Mutações são evidência adicional, não garantia de cobertura total. Um revisor ainda precisa conferir que a falha veio do comportamento e que os testes são legíveis. Em `docs/decisoes.md`, registre também uma decisão de coesão e uma de acoplamento, com o antes/depois do seu código.

## 7. Validação e entrega

```bash
make test-projeto
git add include/projeto.hpp src/projeto.py tests/aluno.cpp tests/aluno.py docs/decisoes.md docs/diagrama.md AI_LOG.md
git commit -m "conclui testes com contratos cumulativos"
git push -u origin projeto/00-testes
```

Faça um commit do incremento guiado e outro da extensão quando ambos forem verificáveis. Abra PR da branch para a `main` **do próprio fork**; confira a execução de `make test-projeto` na CI correspondente ao commit. Integre após testes verdes e revisão. Não abra PR contra o repositório-base.

- [ ] O comando local repete e preserva as etapas anteriores deste starter.
- [ ] A extensão foi adaptada e os casos de fronteira foram explicados.
- [ ] `docs/decisoes.md` relaciona conceito, implementação e evidência.
- [ ] O diagrama corresponde ao estado atual do código.
- [ ] O PR inclui saída local e link da CI do commit.
- [ ] `AI_LOG.md` registra pedido, aceites/rejeições e justificativa, ou declara ausência de IA.

Na main, a CI verifica apenas a baseline executável do starter. A entrega precisa da evidência funcional da branch/PR. Testes visíveis não comprovam entendimento: o docente revisa o diff e, em avaliação, exige defesa oral curta.

A UML da Parte 1 é o modelo de partida. Atualize-a ao introduzir a política e o controlador. Em seguida, avance para [Cenário e Arquitetura](../01-cenario-e-modelagem.md), mantendo estes testes nas próximas integrações.

## Perguntas de revisão rápida

1. Que mudança de requisito deveria afetar a política sem alterar a fonte?
2. Por que um teste só na fronteira não detecta uma implementação que nunca alerta?
3. Como um teste pode passar mesmo que a exceção esperada nunca tenha sido lançada?

## Fontes de referência

- [Python — unittest](https://docs.python.org/3/library/unittest.html)
- [C++ Core Guidelines — interfaces](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-interfaces)
- [Python — testes e assertRaises](https://docs.python.org/3/library/unittest.html#unittest.TestCase.assertRaises)
