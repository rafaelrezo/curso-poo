# Especificadores de acesso, encapsulamento, friend e heranca em C++: protegendo ativos industriais

## Objetivos de aprendizagem

- Diferenciar `public`, `private` e `protected` como decisoes de modelagem, e nao apenas de sintaxe.
- Entender como o encapsulamento protege regras do objeto e por que `friend` deve ser tratado como excecao controlada.
- Reconhecer quando heranca publica representa uma relacao `is-a` valida em C++ e como esse raciocinio aparece em Python.

**Tempo estimado:** 4h

## Video da aula

![type:video](https://www.youtube.com/embed/hYp2Ki1u3ms)

---

## 1. Problema de modelagem: quando tudo e publico, qualquer parte do sistema pode quebrar o objeto

Na aula 2, atributos publicos ajudaram a visualizar o que uma classe guarda. Para aprender o conceito, isso funciona. Para manter software real, isso vira risco rapidamente.

Imagine um `SensorPressao` em uma estacao de bombeamento:

- se `valorAtual` for publico, qualquer trecho do programa pode escrever `-50.0`;
- se `limiteAlarme` for publico, alguem pode zerar o limite sem criterio;
- se o estado de bloqueio for publico, o objeto deixa de proteger a propria operacao.

O problema nao e "esconder por esconder". O problema e garantir que o objeto continue coerente ao longo da vida do programa.

| Situacao | Se tudo for publico | Impacto pratico |
|---|---|---|
| Atualizar leitura do sensor | qualquer funcao escreve qualquer valor | leitura invalida entra no sistema |
| Alterar limite de alarme | qualquer modulo muda o limite | alarmes falsos ou alarmes omitidos |
| Mudar area do equipamento | qualquer trecho reclassifica o ativo | rastreabilidade e manutencao ficam confusas |

**Ideia central desta aula:** especificador de acesso nao e decoracao. Ele define quem pode tocar em cada parte do modelo e em quais condicoes.

---

## 2. Roteiro base com W3Schools

Os quatro links abaixo formam o trilho principal desta unidade. Eles organizam a progressao de um jeito didatico:

| Secao | Leitura base | Link direto para testar | O que observar |
|---|---|---|---|
| Especificadores de acesso | [Access Specifiers](https://www.w3schools.com/cpp/cpp_access_specifiers.asp) | [Try: acesso publico](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_access_public) | `public`, `private` e `protected` controlam quem pode tocar no estado |
| Encapsulamento | [Encapsulation](https://www.w3schools.com/cpp/cpp_encapsulation.asp) | [Try: encapsulamento com getter/setter](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_encapsulation) | esconder dados sensiveis e expor uma API publica coerente |
| `friend` | [Friend Functions](https://www.w3schools.com/cpp/cpp_friend_function.asp) | [Try: friend function](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_friend_function) | `friend` abre uma excecao pontual ao encapsulamento |
| Heranca | [Inheritance](https://www.w3schools.com/cpp/cpp_inheritance.asp) | [Try: heranca simples](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_inheritance) | a classe derivada reaproveita comportamento da base quando existe relacao `is-a` |

### Como usar esta trilha

1. Rode cada exemplo exatamente como esta.
2. Identifique quem pode acessar cada membro e por qual caminho.
3. Troque o exemplo generico por um ativo de engenharia, automacao ou servicos.
4. Pergunte: "estou protegendo uma regra de negocio ou so escondendo dado sem criterio?"

---

## 3. Especificadores de acesso e encapsulamento: quem pode fazer o que com o objeto?

Na W3Schools, a primeira mensagem importante e direta: C++ trabalha com tres especificadores de acesso.

### Leitura rapida dos tres niveis

| Especificador | Quem acessa | Melhor uso | Risco se abusar |
|---|---|---|---|
| `public` | qualquer codigo que tenha acesso ao objeto | operacoes que fazem parte do contrato publico da classe | expor demais e perder controle do estado |
| `private` | apenas a propria classe | dados internos e regras que nao devem ser manipuladas por fora | criar getters/setters cosmeticos sem criterio |
| `protected` | a propria classe e classes derivadas | pontos de extensao pensados para heranca | virar um "atalho" para fugir de encapsulamento |

### O que isso significa na pratica

- `public` deve expor comportamento observavel: ligar bomba, atualizar leitura, consultar estado.
- `private` deve guardar o que sustenta o contrato do objeto: limites, flags, valores internos, detalhes de calibracao.
- `protected` deve ser usado com parcimonia quando a classe-base precisa permitir extensao por subclasses.

### Ponto tecnico importante

Segundo a documentacao do `cppreference`, classes declaradas com `class` possuem membros `private` por padrao, enquanto `struct` usa `public` por padrao. Isso e detalhe de linguagem, mas ajuda a lembrar que C++ trata encapsulamento como parte do modelo, nao como detalhe opcional.

### Entao o que e encapsulamento?

Na formulacao da W3Schools, encapsulamento significa esconder dados sensiveis e expor formas controladas de leitura e alteracao.

Em sala, vale traduzir isso para uma regra mais operacional:

**Conceito -> impacto -> acao recomendada**

- conceito: nem todo atributo deve ficar disponivel para escrita externa;
- impacto: o objeto pode garantir estado coerente e reduzir efeitos colaterais;
- acao recomendada: exponha metodos publicos que representem intencoes do dominio, nao apenas campos abertos.

### Exemplo curto em C++

```cpp
class ValvulaControle {
private:
    double abertura;

public:
    ValvulaControle() : abertura(0.0) {}

    void definirAbertura(double novaAbertura) {
        if (novaAbertura < 0.0) {
            abertura = 0.0;
            return;
        }

        if (novaAbertura > 100.0) {
            abertura = 100.0;
            return;
        }

        abertura = novaAbertura;
    }

    double getAbertura() const {
        return abertura;
    }
};
```

Aqui, a classe nao expoe o atributo diretamente. Ela expoe uma acao publica que preserva a faixa de operacao.

### Erros comuns nesta virada de nivel

- transformar todo atributo em `public` por comodidade;
- criar `get` e `set` para tudo, sem regra alguma;
- usar `protected` em qualquer campo porque "talvez uma subclasse precise";
- esquecer que o objetivo nao e esconder nomes, e sim proteger invariantes.

---

## 4. `friend`: excecao controlada, nao padrao de modelagem

Depois de estudar encapsulamento, o aluno costuma perguntar: "entao nunca mais posso acessar dado privado de fora?"

Em C++, a resposta e: **normalmente nao, mas existe a palavra-chave `friend` para casos bem especificos**.

### O que `friend` faz

- uma funcao `friend` nao e metodo da classe;
- mesmo assim, ela pode acessar membros `private` e `protected`;
- esse acesso precisa ser declarado explicitamente pela classe.

### Quando faz sentido em exemplos didaticos e projetos reais

- funcoes de exibicao fortemente acopladas ao objeto, como `operator<<`;
- adaptadores de integracao que precisam montar uma visao interna controlada;
- relatorios de diagnostico ou manutencao que nao justificam abrir toda a API publica.

### Quando comeca a ficar ruim

- quando varias funcoes externas so funcionam porque viraram `friend`;
- quando `friend` substitui uma API publica mal pensada;
- quando a classe precisa de "amigos demais" para ser usada.

### Regra de bolso

Se `friend` aparece como recurso excepcional, pode ser justificavel.

Se `friend` virou estrategia padrao de acesso, o encapsulamento provavelmente esta fraco.

### Exemplo de ideia

```cpp
class SensorPressao {
private:
    double valorAtual;

public:
    explicit SensorPressao(double valor) : valorAtual(valor) {}

    friend void auditar(const SensorPressao& sensor);
};

void auditar(const SensorPressao& sensor) {
    cout << sensor.valorAtual << "\n";
}
```

O ponto didatico nao e "como burlar o `private`". O ponto e entender que a propria classe escolhe conceder um acesso excepcional e delimitado.

---

## 5. Heranca: reutilizar comportamento sem copiar classe inteira

Na W3Schools, heranca aparece como reuso de atributos e metodos entre uma classe-base e uma classe derivada. Isso esta correto, mas ainda incompleto para um curso de engenharia.

### A pergunta certa nao e "da para herdar?"

A pergunta certa e:

**"a classe derivada realmente e um tipo mais especifico da classe-base?"**

Se a resposta for sim, heranca publica pode fazer sentido.

Se a resposta for nao, provavelmente voce precisa de composicao, e nao de heranca.

### Leitura pratica da heranca publica

O `cppreference` resume a ideia central assim: em heranca publica, o objeto derivado deve se comportar como um objeto da base. Em linguagem de modelagem, a relacao e `is-a`.

Exemplos que costumam funcionar:

- `SensorPressao` e um `AtivoIndustrial`;
- `BombaRecalque` e um `AtivoIndustrial`;
- `ValvulaControle` e um `AtivoIndustrial`.

Exemplo que costuma dar problema:

- `SalaDeControle` herdar de `Sensor`, so porque ambos tem `tag`.

### O que a classe derivada herda, e o que ela nao herda

- membros `public` da base continuam acessiveis como `public` na derivada, se a heranca for publica;
- membros `protected` continuam acessiveis para a derivada;
- membros `private` da base nao ficam acessiveis diretamente na derivada.

Isso e importante: a subclasse reaproveita a base, mas nao ganha direito automatico de mexer em todos os detalhes internos.

### Comparacao de tecnicas de reutilizacao e controle

| Tecnica/Padrao | Melhor uso | Esforco | Entregavel | Limitacao |
|---|---|---|---|---|
| Encapsulamento com `private` + API publica | proteger estado e validar regras do objeto | medio | classe mais segura e legivel | adiciona metodos e exige intencao de projeto |
| `protected` como ponto de extensao | classe-base feita para derivacao | medio | reaproveitamento com algum controle | aumenta acoplamento entre base e derivadas |
| `friend` controlado | diagnostico, impressao ou integracao muito proxima da classe | medio | acesso excepcional sem abrir tudo | pode corroer encapsulamento se usado em excesso |
| Heranca publica | relacao `is-a` real entre base e derivada | medio | subtipo reutilizavel e coerente | hierarquia ruim espalha erro por varias classes |
| Composicao | relacao `has-a` ou colaboracao entre objetos | medio | modelo mais desacoplado | exige delegacao explicita e menos "atalho" sintatico |

### Foco didatico desta aula

Para a trilha inicial do curso, a recomendacao pratica e:

- priorize heranca publica quando houver `is-a`;
- use `protected` com moderacao;
- evite `private inheritance` e `protected inheritance` como primeira escolha em exemplos iniciais;
- compare sempre com a alternativa de composicao.

---

## 6. Exemplo aplicado: estacao de bombeamento com sensor, bomba e auditoria de calibracao

No repositorio, o exemplo compilavel desta aula esta em:

- [exemplo_estacao_bombeamento.cpp](./exemplo_estacao_bombeamento.cpp)

### O que esse exemplo mostra

- `AtivoIndustrial` encapsula `tag` e estado de bloqueio;
- `SensorPressao` e `BombaRecalque` herdam publicamente da classe-base;
- a area operacional fica disponivel para extensao controlada na hierarquia;
- uma funcao `friend` gera relatorio de calibracao sem abrir todo o estado interno.

### Trecho central em C++

```cpp
class AtivoIndustrial {
private:
    string tag;
    bool bloqueado;

protected:
    string area;

    void redefinirArea(string novaArea) {
        area = novaArea;
    }
};

class SensorPressao : public AtivoIndustrial {
private:
    double valorAtual;
    double limiteAlarme;
    double offsetCalibracao;

public:
    friend void emitirRelatorioCalibracao(const SensorPressao& sensor);
};
```

### Por que esse exemplo e didatico

- `tag` nao pode ser alterada livremente por qualquer codigo;
- a subclasse usa o que a base oferece sem violar o `private`;
- `friend` aparece como excecao localizada, e nao como porta geral de acesso;
- o dominio continua proximo de sistemas industriais reais.

### Leitura guiada da saida

Ao executar, discuta com a turma:

1. quais dados ficaram protegidos na base;
2. quais operacoes a subclasse reaproveitou;
3. por que o relatorio de calibracao nao foi implementado como acesso publico geral;
4. em que ponto heranca economizou duplicacao de codigo.

### Como compilar em sala

```bash
g++ -std=c++17 docs/fundamentos_poo_cpp_python/04_encapsulamento_heranca/exemplo_estacao_bombeamento.cpp -o exemplo_encapsulamento
./exemplo_encapsulamento
```

---

## 7. Ponte C++ -> Python

O conceito de encapsulamento e heranca existe nas duas linguagens, mas o mecanismo nao e identico.

### Em C++

- `private`, `protected` e `public` sao parte formal da linguagem;
- `friend` permite uma excecao declarada explicitamente;
- heranca publica comunica relacao `is-a` e preserva acesso conforme a base.

### Em Python

- nao existe "privado real" no mesmo sentido de C++;
- o tutorial oficial recomenda tratar nomes com `_` como nao publicos por convencao;
- heranca usa `class Derivada(Base):` e `super()`;
- nao existe equivalente direto a `friend`, entao a saida usual e expor um metodo/propriedade publica bem pensada.

### Espelho em Python no repositorio

- [exemplo_python_encapsulamento_heranca.py](./exemplo_python_encapsulamento_heranca.py)

```python
class AtivoIndustrial:
    def __init__(self, tag, area):
        self._tag = tag
        self._area = area
        self._bloqueado = False

    @property
    def tag(self):
        return self._tag
```

### Comparacao rapida

| Aspecto | C++ | Python | Impacto didatico |
|---|---|---|---|
| Controle de acesso | formal na linguagem | convencional na maior parte dos casos | C++ torna a fronteira mais explicita |
| Encapsulamento | `private` + API publica | `_atributo`, `@property` e disciplina de projeto | Python depende mais de convencao |
| Heranca | `class Derivada : public Base` | `class Derivada(Base)` | a ideia de especializacao continua |
| Sobrescrita | precisa respeitar a interface da base | o tutorial oficial lembra que metodos sao efetivamente `virtual` | Python torna polimorfismo mais flexivel |
| `friend` | existe | nao ha equivalente direto | em Python, prefira API publica clara |

### Recomendacao pratica por cenario

- em C++, use encapsulamento para defender invariantes e reduzir mutacao acidental;
- em Python, use convencoes e `@property` quando a leitura/escrita precisar de regra;
- nas duas linguagens, so use heranca quando a especializacao fizer sentido no dominio.

---

## 8. Mini-caso pratico: monitoramento de uma estacao de tratamento

Imagine uma estacao com estes ativos:

- um `SensorPressao` na linha de recalque;
- uma `BombaRecalque` que envia agua ao reservatorio elevado;
- um servico de manutencao que precisa auditar calibracao.

### Situacao 1. Leitura operacional

O sistema atualiza a pressao por um metodo controlado:

```cpp
sensor.atualizarValor(6.8);
```

Em vez de escrever no atributo diretamente, a classe decide como aceitar esse valor.

### Situacao 2. Especializacao

`SensorPressao` e `BombaRecalque` compartilham identidade, area e estado de bloqueio porque ambos sao `AtivoIndustrial`.

### Situacao 3. Diagnostico excepcional

A manutencao precisa enxergar um `offsetCalibracao` interno.

Em vez de abrir esse campo para todo o sistema, a classe libera acesso pontual a uma funcao `friend`.

**Licao didatica:** encapsular nao significa impedir trabalho. Significa decidir com clareza quais portas existem, quem pode usa-las e por que elas existem.

---

## 9. Exercicios para casa: atividade guiada de encapsulamento e heranca

No repositorio, ha um arquivo-base para a atividade:

- [projeto_estacao_bombeamento_base.cpp](./projeto_estacao_bombeamento_base.cpp)

Se quiser manter uma referencia pronta para revisao posterior, use tambem:

- [projeto_estacao_bombeamento.cpp](./projeto_estacao_bombeamento.cpp)

### Enunciado

Modele uma pequena estacao com:

- `AtivoCampo` como classe-base;
- `SensorNivel` como classe derivada;
- `BombaDosadora` como classe derivada.

### Regras da atividade

- a base deve encapsular `tag` e estado de bloqueio;
- as derivadas nao devem acessar diretamente membros `private` da base;
- `protected` deve aparecer apenas onde realmente ajuda a extensao;
- o `friend` de `SensorNivel` deve ser usado apenas para inspecao tecnica.

### Tarefas

1. Implementar os construtores das derivadas reaproveitando a classe-base.
2. Criar metodos publicos que validem alteracoes de nivel e vazao.
3. Declarar e definir a funcao `friend` de inspecao do sensor.
4. Exibir no terminal um resumo dos objetos criados.
5. Justificar, em comentario curto, por que cada membro ficou `public`, `private` ou `protected`.

### Checklist de entrega

- o codigo compila;
- o estado importante ficou protegido;
- as derivadas reutilizam a base sem duplicacao desnecessaria;
- a funcao `friend` ficou localizada e justificavel;
- o terminal mostra um resumo legivel dos ativos.

### Comando sugerido para o aluno testar

```bash
g++ -std=c++17 docs/fundamentos_poo_cpp_python/04_encapsulamento_heranca/projeto_estacao_bombeamento.cpp -o projeto_bombeamento
./projeto_bombeamento
```

---

## 10. Videos e materiais complementares

### Video principal desta unidade

- [Curso de C++ #65 - POO, Encapsulamento - P7 (CFB Cursos)](https://www.youtube.com/watch?v=hYp2Ki1u3ms)

### Video complementar para heranca

- [Curso de C++ #47 - POO, Classes, Heranca - P4 (CFB Cursos)](https://www.youtube.com/watch?v=TLQzg0b-AY4)

### Laboratorio complementar

- [W3Schools - Access Specifiers](https://www.w3schools.com/cpp/cpp_access_specifiers.asp)
- [W3Schools - Encapsulation](https://www.w3schools.com/cpp/cpp_encapsulation.asp)
- [W3Schools - Friend Functions](https://www.w3schools.com/cpp/cpp_friend_function.asp)
- [W3Schools - Inheritance](https://www.w3schools.com/cpp/cpp_inheritance.asp)

### Observacao didatica

O video principal ajuda a abrir o assunto. A compreensao melhora quando o aluno alterna entre leitura, execucao no navegador e adaptacao do exemplo para um dominio de engenharia.

---

## Perguntas de revisao rapida

1. Qual a diferenca pratica entre `private` e `protected` em uma hierarquia de classes?
2. Em que situacoes `friend` pode ser justificavel sem enfraquecer demais o encapsulamento?
3. Como decidir se uma relacao deve usar heranca publica ou composicao?

## Fontes de referencia

- https://www.w3schools.com/cpp/cpp_access_specifiers.asp
- https://www.w3schools.com/cpp/cpp_encapsulation.asp
- https://www.w3schools.com/cpp/cpp_friend_function.asp
- https://www.w3schools.com/cpp/cpp_inheritance.asp
- https://www.w3schools.com/cpp/trycpp.asp?filename=demo_access_public
- https://www.w3schools.com/cpp/trycpp.asp?filename=demo_encapsulation
- https://www.w3schools.com/cpp/trycpp.asp?filename=demo_friend_function
- https://www.w3schools.com/cpp/trycpp.asp?filename=demo_inheritance
- https://en.cppreference.com/w/cpp/language/access
- https://en.cppreference.com/w/cpp/language/friend
- https://en.cppreference.com/w/cpp/language/derived_class
- https://docs.python.org/3/tutorial/classes.html
