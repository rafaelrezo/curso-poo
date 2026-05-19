# Polimorfismo em C++: uma interface, respostas diferentes na automação

## Objetivos de aprendizagem

- Entender polimorfismo como a capacidade de tratar objetos relacionados por uma interface comum, mantendo comportamentos diferentes.
- Reconhecer por que `virtual`, `override` e classes abstratas são decisivos para despacho dinâmico em C++.
- Aplicar o conceito em cenários de engenharia de controle e automação, comparando a implementação em C++ e Python.

**Tempo estimado:** 4h

## Vídeo da aula

![type:video](https://www.youtube.com/embed/wX2gozSqHfU)

---

## 1. Problema de modelagem: o supervisório quer comandar equipamentos diferentes sem espalhar `if` por todo o sistema

Em uma estação de tratamento, em uma linha de envase ou em uma celula robotica, o software de supervisão costuma enxergar um conjunto heterogeneo de ativos:

- um inversor que converte referência percentual em frequência;
- uma válvula proporcional que converte referência percentual em abertura;
- uma bomba dosadora que converte referência percentual em vazão.

O operador e o sistema de controle muitas vezes querem emitir um comando unificado, por exemplo:

```cpp
aplicarReferenciaPercentual(62.5);
```

O problema é que cada equipamento responde de um jeito:

| Ativo | Comando comum | Resposta concreta | Sem polimorfismo |
|---|---|---|---|
| Inversor da bomba | `aplicarReferenciaPercentual(62.5)` | converte para `37.5 Hz` | `if` por tipo e logica espalhada |
| Valvula proporcional | `aplicarReferenciaPercentual(62.5)` | ajusta `62.5%` de abertura | interface inconsistente entre classes |
| Dosador quimico | `aplicarReferenciaPercentual(62.5)` | converte para vazão de dosagem | acoplamento forte com implementações concretas |

Sem polimorfismo, o código tende a virar uma cascata de `if`, `switch` e conversoes manuais.

Com polimorfismo, o sistema chama uma interface comum, e cada objeto concreto executa seu próprio comportamento.

**Ideia central desta aula:** polimorfismo não e "escrever o mesmo nome em várias classes". Polimorfismo e permitir que uma mesma operação seja enviada a tipos diferentes sem perder o comportamento específico de cada um.

---

## 2. Roteiro base com W3Schools

A W3Schools abre bem a intuicao do tema, e a página de `virtual functions` fecha a parte operacional que interessa mais em C++.

| Secao | Leitura base | Link direto para testar | O que observar |
|---|---|---|---|
| Polimorfismo | [Polymorphism](https://www.w3schools.com/cpp/cpp_polymorphism.asp) | [Try: sem `virtual`](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_virtual) | a mesma ação pode produzir efeitos diferentes em classes derivadas |
| Funcoes virtuais | [Virtual Functions](https://www.w3schools.com/cpp/cpp_virtual_functions.asp) | [Try: com `virtual`](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_virtual2) | em C++, o despacho dinâmico depende de `virtual` quando a chamada passa por ponteiro ou referência da base |
| Referência oficial C++ | [cppreference: `virtual`](https://en.cppreference.com/w/cpp/language/virtual) | [cppreference: `override`](https://en.cppreference.com/w/cpp/language/override) | `virtual` habilita despacho dinâmico; `override` reduz erro de assinatura |

### Como usar esta trilha

1. Rode primeiro o exemplo sem `virtual`.
2. Observe que o ponteiro da base não chama automaticamente a versão da classe derivada.
3. Rode depois o exemplo com `virtual` e compare a saída.
4. Reescreva o exemplo genérico `Animal` para um domínio de automação.

---

## 3. O que e polimorfismo, afinal?

Na formulacao da W3Schools, polimorfismo significa "muitas formas" e aparece quando várias classes relacionadas por herança respondem a uma mesma operação.

Para um curso de engenharia, vale traduzir isso de forma mais prática:

- conceito: vários tipos concretos compartilham uma interface comum;
- impacto: o software supervisor, o simulador ou a camada de controle não precisam conhecer todos os detalhes de cada equipamento;
- ação recomendada: modele uma base comum apenas quando houver comportamento comum real e variação legitima entre derivados.

### O que esta aula chama de polimorfismo

Nesta unidade, o foco principal e **polimorfismo dinâmico**, também chamado de polimorfismo por subtipo:

- existe uma classe-base ou interface comum;
- classes derivadas sobrescrevem a operação;
- o programa decide em tempo de execução qual implementação chamar.

### O que pode confundir o aluno

Em muitos materiais, a palavra polimorfismo aparece junto com sobrecarga.

Isso merece um corte claro:

- `sobrecarga` escolhe entre funções pela assinatura, em tempo de compilação;
- `sobrescrita` redefine o comportamento de uma operação herdada;
- `polimorfismo dinâmico` acontece quando a chamada vai pela base, mas o comportamento executado vem do tipo concreto do objeto.

### Regra de bolso

Se você precisa que o sistema trate vários objetos por um contrato comum e, ainda assim, cada um reaja de maneira diferente, então polimorfismo provavelmente faz sentido.

Se a diferença está apenas na lista de parâmetros, talvez o problema seja sobrecarga, não polimorfismo dinâmico.

---

## 4. Como C++ suporta isso formalmente

Segundo a W3Schools e o `cppreference`, o ponto decisivo em C++ e simples:

- sem `virtual`, a chamada por ponteiro ou referência da base segue o tipo da base;
- com `virtual`, a chamada considera o tipo real do objeto;
- `override` e opcional na sintaxe, mas recomendado para detectar erro de assinatura;
- uma função virtual pura (`= 0`) transforma a base em classe abstrata;
- um destrutor virtual e a forma segura de projetar uma base polimórfica.

### Exemplo curto de contrato polimórfico

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

O ganho não está em escrever a classe derivada. O ganho aparece quando o programa usa a base:

```cpp
InversorBomba bomba("P-201", "Recalque");
AtuadorMalha* atuador = &bomba;
atuador->aplicarReferenciaPercentual(60.0);
```

Se `aplicarReferenciaPercentual()` for virtual, a implementação de `InversorBomba` será usada.

Se não for virtual, o compilador considerara a versão associada ao tipo `AtuadorMalha*`.

### Ponto técnico importante

O `cppreference` destaca que uma chamada virtual preserva o comportamento sobrescrito mesmo quando o programa so enxerga ponteiro ou referência da base. Isso e exatamente o que permite escrever software menos acoplado para famílias de sensores, atuadores e módulos de simulação.

### Erros comuns nesta etapa

- achar que herança sozinha já entrega polimorfismo dinâmico;
- esquecer `virtual` na base;
- esquecer `override` na derivada e errar a assinatura sem perceber;
- criar hierarquia so porque várias classes "parecem parentes", sem contrato comum bem definido.

---

## 5. Comparação de abordagens para variação de comportamento

| Técnica/Padrão | Melhor uso | Esforço | Entregável | Limitação |
|---|---|---|---|---|
| Sobrecarga | mesmas operações com assinaturas diferentes | baixo | flexibilidade estatica de chamada | não resolve despacho via base |
| Herança + sobrescrita com `virtual` | família de tipos com contrato comum e respostas diferentes | médio | despacho dinâmico e código cliente mais simples | hierarquia ruim aumenta acoplamento |
| Classe abstrata | definir operações obrigatórias para toda a família | médio | interface clara para o domínio | abstrai cedo demais quando existe so um tipo concreto |
| Composição + estratégia | variar comportamento sem aprofundar herança | médio | sistema mais desacoplado | exige mais objetos e montagem explícita |

### Recomendação prática por cenário

- use sobrecarga quando a variação estiver na assinatura;
- use polimorfismo com `virtual` quando o código cliente precisa tratar vários equipamentos pelo mesmo contrato;
- use composição quando a variação estiver em algoritmos trocaveis e não em identidades de tipo.

---

## 6. Exemplo aplicado: supervisório de uma estação de tratamento

No repositório, o exemplo compilável desta aula está em:

- [exemplo_estacao_tratamento_polimorfica.cpp](./exemplo_estacao_tratamento_polimorfica.cpp)

### O que esse exemplo mostra

- `AtuadorMalha` como base abstrata;
- `InversorBomba`, `ValvulaProporcional` e `DosadorQuimico` sobrescrevendo a mesma operação;
- um `vector<AtuadorMalha*>` permitindo que o supervisório trate equipamentos distintos por uma interface única;
- a mesma referência percentual produzindo respostas fisicas diferentes em cada ativo.

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

- aproxima o conceito de software de controle e supervisão real;
- deixa claro que o sistema chama a mesma operação para todos;
- mostra que o objeto concreto decide como transformar a referência em ação física;
- evita a falsa ideia de que polimorfismo e apenas "copiar métodos com o mesmo nome".

### Leitura guiada da saída

Ao executar, discuta com a turma:

1. qual equipamento transformou percentual em frequência;
2. qual equipamento manteve o percentual como abertura;
3. qual equipamento converteu percentual em vazão;
4. por que o laço principal não precisou descobrir o tipo concreto com `if`.

### Como compilar em sala

```bash
g++ -std=c++17 docs/fundamentos_poo_cpp_python/05_polimorfismo/exemplo_estacao_tratamento_polimorfica.cpp -o exemplo_polimorfismo
./exemplo_polimorfismo
```

---

## 7. Ponte C++ -> Python

A ideia de polimorfismo existe nas duas linguagens, mas a forma de declarar o contrato não e a mesma.

### Em C++

- o uso de `virtual` explícita a intencao polimórfica;
- `override` ajuda a validar a sobrescrita;
- classes abstratas com `= 0` deixam claro que a base define contrato, não implementação completa;
- ponteiros e referências para a base são a forma tipica de acionar despacho dinâmico.

### Em Python

- métodos são despachados dinamicamente por padrão;
- o tutorial oficial observa que, para programadores de C++, os métodos em Python são efetivamente `virtual`;
- `ABC` e `@abstractmethod` ajudam a explicitar contrato quando isso melhora a legibilidade;
- listas comuns de objetos já permitem variação de comportamento sem a mesma cerimonia sintatica de C++.

### Espelho em Python no repositório

- [exemplo_python_polimorfismo.py](./exemplo_python_polimorfismo.py)

```python
class AtuadorMalha(ABC):
    @abstractmethod
    def aplicar_referencia_percentual(self, percentual):
        pass
```

### Comparação rápida

| Aspecto | C++ | Python | Impacto didatico |
|---|---|---|---|
| Ativacao do polimorfismo | `virtual` na base | despacho dinâmico por padrão | C++ exige declaracao mais explícita |
| Validação de sobrescrita | `override` | convenção, testes e revisão | C++ protege melhor contra erro de assinatura |
| Contrato abstrato | função virtual pura | `ABC` e `@abstractmethod` | os dois conseguem comunicar interface |
| Uso em coleções | ponteiro/referência da base | lista de objetos | Python fica mais direto para demonstração |

### Recomendação prática por cenário

- em C++, ensine polimorfismo junto com `virtual`, `override` e base abstrata;
- em Python, use `ABC` quando o curso precisar evidenciar contrato, não apenas comportamento;
- nas duas linguagens, comece pelo problema de modelagem antes da sintaxe.

---

## 8. Mini-caso prático: malha de dosagem em uma ETA

Imagine uma pequena ETA com três equipamentos controlados por software:

- uma bomba de recalque comandada por inversor;
- uma válvula proporcional na linha de mistura rápida;
- um dosador quimico na etapa de cloracao.

O supervisório calcula uma referência unificada de `62.5%` para a operação atual.

### Situacao 1. Mesmo comando para tipos diferentes

O software envia a mesma chamada para todos:

```cpp
atuador->aplicarReferenciaPercentual(62.5);
```

### Situacao 2. Respostas fisicas diferentes

- a bomba converte em frequência;
- a válvula converte em abertura;
- o dosador converte em vazão.

### Situacao 3. Menos acoplamento no código cliente

O laço do supervisório não precisa saber se está falando com inversor, válvula ou dosador. Ele depende da interface da base.

**Licao didatica:** polimorfismo não elimina as diferenças entre os equipamentos. Ele desloca essas diferenças para o lugar correto: a implementação de cada tipo concreto.

---

## 9. Exercícios para casa: atividade guiada de polimorfismo

No repositório, ha um arquivo-base para a atividade:

- [projeto_linha_envase_base.cpp](./projeto_linha_envase_base.cpp)

Se quiser manter uma referência pronta para revisão posterior, use também:

- [projeto_linha_envase.cpp](./projeto_linha_envase.cpp)

### Enunciado

Modele uma pequena linha de envase com:

- `AtuadorLinha` como classe-base abstrata;
- `MotorEsteira` como classe derivada;
- `ValvulaDosagem` como classe derivada.

### Regras da atividade

- a base deve expor a operação `aplicarReferenciaPercentual()`;
- cada derivada deve interpretar a referência de forma coerente com o ativo;
- o `main` deve usar um `vector<AtuadorLinha*>`;
- a execução final deve imprimir um resumo dos equipamentos apos o mesmo comando.

### Tarefas

1. Implementar os construtores das duas derivadas.
2. Sobrescrever `aplicarReferenciaPercentual()` em cada classe.
3. Sobrescrever `exibirResumo()` com saída legivel.
4. Criar um vetor polimórfico e percorrer os atuadores.
5. Explicar, em comentario curto, por que a operação da base foi declarada `virtual`.

### Checklist de entrega

- o código compila;
- a base está abstrata;
- as derivadas usam `override`;
- o `main` percorre os objetos pela interface da base;
- a saída mostra respostas diferentes para o mesmo comando.

### Comando sugerido para o aluno testar

```bash
g++ -std=c++17 docs/fundamentos_poo_cpp_python/05_polimorfismo/projeto_linha_envase.cpp -o projeto_polimorfismo
./projeto_polimorfismo
```

---

## 10. Videos e materiais complementares

### Vídeo principal desta unidade

- [Curso de C++ #66 - POO, Polimorfismo (sobrecarga de métodos) - P8 (CFB Cursos)](https://www.youtube.com/watch?v=wX2gozSqHfU)

### Laboratorio complementar

- [W3Schools - Polymorphism](https://www.w3schools.com/cpp/cpp_polymorphism.asp)
- [W3Schools - Virtual Functions](https://www.w3schools.com/cpp/cpp_virtual_functions.asp)
- [Try: sem `virtual`](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_virtual)
- [Try: com `virtual`](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_virtual2)
- [cppreference - `virtual`](https://en.cppreference.com/w/cpp/language/virtual)
- [Python Docs - Classes](https://docs.python.org/3/tutorial/classes.html)

### Observação didatica

O vídeo principal ajuda a iniciar o tema, mas a diferença entre "mesmo nome" e "despacho dinâmico" fica realmente clara quando o aluno executa os dois exemplos da W3Schools e adapta o caso `Animal` para uma malha de controle.

---

## Perguntas de revisão rápida

1. Por que herança, sozinha, não garante polimorfismo dinâmico em C++?
2. O que muda na prática quando uma operação da base e declarada `virtual`?
3. Em um sistema de automação, que vantagem existe em percorrer vários equipamentos por um `vector` da classe-base?

## Fontes de referência

- https://www.w3schools.com/cpp/cpp_polymorphism.asp
- https://www.w3schools.com/cpp/cpp_virtual_functions.asp
- https://www.w3schools.com/cpp/trycpp.asp?filename=demo_virtual
- https://www.w3schools.com/cpp/trycpp.asp?filename=demo_virtual2
- https://en.cppreference.com/w/cpp/language/virtual
- https://en.cppreference.com/w/cpp/language/override
- https://docs.python.org/3/tutorial/classes.html
- https://docs.python.org/3/library/abc.html
