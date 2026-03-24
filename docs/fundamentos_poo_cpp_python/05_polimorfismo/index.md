# Polimorfismo em C++: uma interface, respostas diferentes na automacao

## Objetivos de aprendizagem

- Entender polimorfismo como a capacidade de tratar objetos relacionados por uma interface comum, mantendo comportamentos diferentes.
- Reconhecer por que `virtual`, `override` e classes abstratas sao decisivos para despacho dinamico em C++.
- Aplicar o conceito em cenarios de engenharia de controle e automacao, comparando a implementacao em C++ e Python.

**Tempo estimado:** 4h

## Video da aula

![type:video](https://www.youtube.com/embed/wX2gozSqHfU)

---

## 1. Problema de modelagem: o supervisorio quer comandar equipamentos diferentes sem espalhar `if` por todo o sistema

Em uma estacao de tratamento, em uma linha de envase ou em uma celula robotica, o software de supervisao costuma enxergar um conjunto heterogeneo de ativos:

- um inversor que converte referencia percentual em frequencia;
- uma valvula proporcional que converte referencia percentual em abertura;
- uma bomba dosadora que converte referencia percentual em vazao.

O operador e o sistema de controle muitas vezes querem emitir um comando unificado, por exemplo:

```cpp
aplicarReferenciaPercentual(62.5);
```

O problema e que cada equipamento responde de um jeito:

| Ativo | Comando comum | Resposta concreta | Sem polimorfismo |
|---|---|---|---|
| Inversor da bomba | `aplicarReferenciaPercentual(62.5)` | converte para `37.5 Hz` | `if` por tipo e logica espalhada |
| Valvula proporcional | `aplicarReferenciaPercentual(62.5)` | ajusta `62.5%` de abertura | interface inconsistente entre classes |
| Dosador quimico | `aplicarReferenciaPercentual(62.5)` | converte para vazao de dosagem | acoplamento forte com implementacoes concretas |

Sem polimorfismo, o codigo tende a virar uma cascata de `if`, `switch` e conversoes manuais.

Com polimorfismo, o sistema chama uma interface comum, e cada objeto concreto executa seu proprio comportamento.

**Ideia central desta aula:** polimorfismo nao e "escrever o mesmo nome em varias classes". Polimorfismo e permitir que uma mesma operacao seja enviada a tipos diferentes sem perder o comportamento especifico de cada um.

---

## 2. Roteiro base com W3Schools

A W3Schools abre bem a intuicao do tema, e a pagina de `virtual functions` fecha a parte operacional que interessa mais em C++.

| Secao | Leitura base | Link direto para testar | O que observar |
|---|---|---|---|
| Polimorfismo | [Polymorphism](https://www.w3schools.com/cpp/cpp_polymorphism.asp) | [Try: sem `virtual`](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_virtual) | a mesma acao pode produzir efeitos diferentes em classes derivadas |
| Funcoes virtuais | [Virtual Functions](https://www.w3schools.com/cpp/cpp_virtual_functions.asp) | [Try: com `virtual`](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_virtual2) | em C++, o despacho dinamico depende de `virtual` quando a chamada passa por ponteiro ou referencia da base |
| Referencia oficial C++ | [cppreference: `virtual`](https://en.cppreference.com/w/cpp/language/virtual) | [cppreference: `override`](https://en.cppreference.com/w/cpp/language/override) | `virtual` habilita despacho dinamico; `override` reduz erro de assinatura |

### Como usar esta trilha

1. Rode primeiro o exemplo sem `virtual`.
2. Observe que o ponteiro da base nao chama automaticamente a versao da classe derivada.
3. Rode depois o exemplo com `virtual` e compare a saida.
4. Reescreva o exemplo generico `Animal` para um dominio de automacao.

---

## 3. O que e polimorfismo, afinal?

Na formulacao da W3Schools, polimorfismo significa "muitas formas" e aparece quando varias classes relacionadas por heranca respondem a uma mesma operacao.

Para um curso de engenharia, vale traduzir isso de forma mais pratica:

- conceito: varios tipos concretos compartilham uma interface comum;
- impacto: o software supervisor, o simulador ou a camada de controle nao precisam conhecer todos os detalhes de cada equipamento;
- acao recomendada: modele uma base comum apenas quando houver comportamento comum real e variacao legitima entre derivados.

### O que esta aula chama de polimorfismo

Nesta unidade, o foco principal e **polimorfismo dinamico**, tambem chamado de polimorfismo por subtipo:

- existe uma classe-base ou interface comum;
- classes derivadas sobrescrevem a operacao;
- o programa decide em tempo de execucao qual implementacao chamar.

### O que pode confundir o aluno

Em muitos materiais, a palavra polimorfismo aparece junto com sobrecarga.

Isso merece um corte claro:

- `sobrecarga` escolhe entre funcoes pela assinatura, em tempo de compilacao;
- `sobrescrita` redefine o comportamento de uma operacao herdada;
- `polimorfismo dinamico` acontece quando a chamada vai pela base, mas o comportamento executado vem do tipo concreto do objeto.

### Regra de bolso

Se voce precisa que o sistema trate varios objetos por um contrato comum e, ainda assim, cada um reaja de maneira diferente, entao polimorfismo provavelmente faz sentido.

Se a diferenca esta apenas na lista de parametros, talvez o problema seja sobrecarga, nao polimorfismo dinamico.

---

## 4. Como C++ suporta isso formalmente

Segundo a W3Schools e o `cppreference`, o ponto decisivo em C++ e simples:

- sem `virtual`, a chamada por ponteiro ou referencia da base segue o tipo da base;
- com `virtual`, a chamada considera o tipo real do objeto;
- `override` e opcional na sintaxe, mas recomendado para detectar erro de assinatura;
- uma funcao virtual pura (`= 0`) transforma a base em classe abstrata;
- um destrutor virtual e a forma segura de projetar uma base polimorfica.

### Exemplo curto de contrato polimorfico

```cpp
class AtuadorMalha {
public:
    virtual ~AtuadorMalha() = default;
    virtual void aplicarReferenciaPercentual(double percentual) = 0;
    virtual void exibirResumo() const = 0;
};

class InversorBomba : public AtuadorMalha {
public:
    void aplicarReferenciaPercentual(double percentual) override {
        // converte percentual em Hz
    }

    void exibirResumo() const override {
        // mostra o estado do inversor
    }
};
```

### Onde o polimorfismo realmente aparece

O ganho nao esta em escrever a classe derivada. O ganho aparece quando o programa usa a base:

```cpp
InversorBomba bomba("P-201", "Recalque");
AtuadorMalha* atuador = &bomba;
atuador->aplicarReferenciaPercentual(60.0);
```

Se `aplicarReferenciaPercentual()` for virtual, a implementacao de `InversorBomba` sera usada.

Se nao for virtual, o compilador considerara a versao associada ao tipo `AtuadorMalha*`.

### Ponto tecnico importante

O `cppreference` destaca que uma chamada virtual preserva o comportamento sobrescrito mesmo quando o programa so enxerga ponteiro ou referencia da base. Isso e exatamente o que permite escrever software menos acoplado para familias de sensores, atuadores e modulos de simulacao.

### Erros comuns nesta etapa

- achar que heranca sozinha ja entrega polimorfismo dinamico;
- esquecer `virtual` na base;
- esquecer `override` na derivada e errar a assinatura sem perceber;
- criar hierarquia so porque varias classes "parecem parentes", sem contrato comum bem definido.

---

## 5. Comparacao de abordagens para variacao de comportamento

| Tecnica/Padrao | Melhor uso | Esforco | Entregavel | Limitacao |
|---|---|---|---|---|
| Sobrecarga | mesmas operacoes com assinaturas diferentes | baixo | flexibilidade estatica de chamada | nao resolve despacho via base |
| Heranca + sobrescrita com `virtual` | familia de tipos com contrato comum e respostas diferentes | medio | despacho dinamico e codigo cliente mais simples | hierarquia ruim aumenta acoplamento |
| Classe abstrata | definir operacoes obrigatorias para toda a familia | medio | interface clara para o dominio | abstrai cedo demais quando existe so um tipo concreto |
| Composicao + estrategia | variar comportamento sem aprofundar heranca | medio | sistema mais desacoplado | exige mais objetos e montagem explicita |

### Recomendacao pratica por cenario

- use sobrecarga quando a variacao estiver na assinatura;
- use polimorfismo com `virtual` quando o codigo cliente precisa tratar varios equipamentos pelo mesmo contrato;
- use composicao quando a variacao estiver em algoritmos trocaveis e nao em identidades de tipo.

---

## 6. Exemplo aplicado: supervisorio de uma estacao de tratamento

No repositorio, o exemplo compilavel desta aula esta em:

- [exemplo_estacao_tratamento_polimorfica.cpp](./exemplo_estacao_tratamento_polimorfica.cpp)

### O que esse exemplo mostra

- `AtuadorMalha` como base abstrata;
- `InversorBomba`, `ValvulaProporcional` e `DosadorQuimico` sobrescrevendo a mesma operacao;
- um `vector<AtuadorMalha*>` permitindo que o supervisorio trate equipamentos distintos por uma interface unica;
- a mesma referencia percentual produzindo respostas fisicas diferentes em cada ativo.

### Trecho central em C++

```cpp
vector<AtuadorMalha*> atuadores = {
    &bombaRecalque,
    &valvulaCoagulante,
    &dosadorCloro
};

for (AtuadorMalha* atuador : atuadores) {
    atuador->aplicarReferenciaPercentual(referenciaSupervisoria);
}
```

### Por que esse exemplo e didatico

- aproxima o conceito de software de controle e supervisao real;
- deixa claro que o sistema chama a mesma operacao para todos;
- mostra que o objeto concreto decide como transformar a referencia em acao fisica;
- evita a falsa ideia de que polimorfismo e apenas "copiar metodos com o mesmo nome".

### Leitura guiada da saida

Ao executar, discuta com a turma:

1. qual equipamento transformou percentual em frequencia;
2. qual equipamento manteve o percentual como abertura;
3. qual equipamento converteu percentual em vazao;
4. por que o laço principal nao precisou descobrir o tipo concreto com `if`.

### Como compilar em sala

```bash
g++ -std=c++17 docs/fundamentos_poo_cpp_python/05_polimorfismo/exemplo_estacao_tratamento_polimorfica.cpp -o exemplo_polimorfismo
./exemplo_polimorfismo
```

---

## 7. Ponte C++ -> Python

A ideia de polimorfismo existe nas duas linguagens, mas a forma de declarar o contrato nao e a mesma.

### Em C++

- o uso de `virtual` explicita a intencao polimorfica;
- `override` ajuda a validar a sobrescrita;
- classes abstratas com `= 0` deixam claro que a base define contrato, nao implementacao completa;
- ponteiros e referencias para a base sao a forma tipica de acionar despacho dinamico.

### Em Python

- metodos sao despachados dinamicamente por padrao;
- o tutorial oficial observa que, para programadores de C++, os metodos em Python sao efetivamente `virtual`;
- `ABC` e `@abstractmethod` ajudam a explicitar contrato quando isso melhora a legibilidade;
- listas comuns de objetos ja permitem variacao de comportamento sem a mesma cerimonia sintatica de C++.

### Espelho em Python no repositorio

- [exemplo_python_polimorfismo.py](./exemplo_python_polimorfismo.py)

```python
class AtuadorMalha(ABC):
    @abstractmethod
    def aplicar_referencia_percentual(self, percentual):
        pass
```

### Comparacao rapida

| Aspecto | C++ | Python | Impacto didatico |
|---|---|---|---|
| Ativacao do polimorfismo | `virtual` na base | despacho dinamico por padrao | C++ exige declaracao mais explicita |
| Validacao de sobrescrita | `override` | convencao, testes e revisao | C++ protege melhor contra erro de assinatura |
| Contrato abstrato | funcao virtual pura | `ABC` e `@abstractmethod` | os dois conseguem comunicar interface |
| Uso em colecoes | ponteiro/referencia da base | lista de objetos | Python fica mais direto para demonstracao |

### Recomendacao pratica por cenario

- em C++, ensine polimorfismo junto com `virtual`, `override` e base abstrata;
- em Python, use `ABC` quando o curso precisar evidenciar contrato, nao apenas comportamento;
- nas duas linguagens, comece pelo problema de modelagem antes da sintaxe.

---

## 8. Mini-caso pratico: malha de dosagem em uma ETA

Imagine uma pequena ETA com tres equipamentos controlados por software:

- uma bomba de recalque comandada por inversor;
- uma valvula proporcional na linha de mistura rapida;
- um dosador quimico na etapa de cloracao.

O supervisorio calcula uma referencia unificada de `62.5%` para a operacao atual.

### Situacao 1. Mesmo comando para tipos diferentes

O software envia a mesma chamada para todos:

```cpp
atuador->aplicarReferenciaPercentual(62.5);
```

### Situacao 2. Respostas fisicas diferentes

- a bomba converte em frequencia;
- a valvula converte em abertura;
- o dosador converte em vazao.

### Situacao 3. Menos acoplamento no codigo cliente

O laço do supervisorio nao precisa saber se esta falando com inversor, valvula ou dosador. Ele depende da interface da base.

**Licao didatica:** polimorfismo nao elimina as diferencas entre os equipamentos. Ele desloca essas diferencas para o lugar correto: a implementacao de cada tipo concreto.

---

## 9. Exercicios para casa: atividade guiada de polimorfismo

No repositorio, ha um arquivo-base para a atividade:

- [projeto_linha_envase_base.cpp](./projeto_linha_envase_base.cpp)

Se quiser manter uma referencia pronta para revisao posterior, use tambem:

- [projeto_linha_envase.cpp](./projeto_linha_envase.cpp)

### Enunciado

Modele uma pequena linha de envase com:

- `AtuadorLinha` como classe-base abstrata;
- `MotorEsteira` como classe derivada;
- `ValvulaDosagem` como classe derivada.

### Regras da atividade

- a base deve expor a operacao `aplicarReferenciaPercentual()`;
- cada derivada deve interpretar a referencia de forma coerente com o ativo;
- o `main` deve usar um `vector<AtuadorLinha*>`;
- a execucao final deve imprimir um resumo dos equipamentos apos o mesmo comando.

### Tarefas

1. Implementar os construtores das duas derivadas.
2. Sobrescrever `aplicarReferenciaPercentual()` em cada classe.
3. Sobrescrever `exibirResumo()` com saida legivel.
4. Criar um vetor polimorfico e percorrer os atuadores.
5. Explicar, em comentario curto, por que a operacao da base foi declarada `virtual`.

### Checklist de entrega

- o codigo compila;
- a base esta abstrata;
- as derivadas usam `override`;
- o `main` percorre os objetos pela interface da base;
- a saida mostra respostas diferentes para o mesmo comando.

### Comando sugerido para o aluno testar

```bash
g++ -std=c++17 docs/fundamentos_poo_cpp_python/05_polimorfismo/projeto_linha_envase.cpp -o projeto_polimorfismo
./projeto_polimorfismo
```

---

## 10. Videos e materiais complementares

### Video principal desta unidade

- [Curso de C++ #66 - POO, Polimorfismo (sobrecarga de metodos) - P8 (CFB Cursos)](https://www.youtube.com/watch?v=wX2gozSqHfU)

### Laboratorio complementar

- [W3Schools - Polymorphism](https://www.w3schools.com/cpp/cpp_polymorphism.asp)
- [W3Schools - Virtual Functions](https://www.w3schools.com/cpp/cpp_virtual_functions.asp)
- [Try: sem `virtual`](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_virtual)
- [Try: com `virtual`](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_virtual2)
- [cppreference - `virtual`](https://en.cppreference.com/w/cpp/language/virtual)
- [Python Docs - Classes](https://docs.python.org/3/tutorial/classes.html)

### Observacao didatica

O video principal ajuda a iniciar o tema, mas a diferenca entre "mesmo nome" e "despacho dinamico" fica realmente clara quando o aluno executa os dois exemplos da W3Schools e adapta o caso `Animal` para uma malha de controle.

---

## Perguntas de revisao rapida

1. Por que heranca, sozinha, nao garante polimorfismo dinamico em C++?
2. O que muda na pratica quando uma operacao da base e declarada `virtual`?
3. Em um sistema de automacao, que vantagem existe em percorrer varios equipamentos por um `vector` da classe-base?

## Fontes de referencia

- https://www.w3schools.com/cpp/cpp_polymorphism.asp
- https://www.w3schools.com/cpp/cpp_virtual_functions.asp
- https://www.w3schools.com/cpp/trycpp.asp?filename=demo_virtual
- https://www.w3schools.com/cpp/trycpp.asp?filename=demo_virtual2
- https://en.cppreference.com/w/cpp/language/virtual
- https://en.cppreference.com/w/cpp/language/override
- https://docs.python.org/3/tutorial/classes.html
- https://docs.python.org/3/library/abc.html
