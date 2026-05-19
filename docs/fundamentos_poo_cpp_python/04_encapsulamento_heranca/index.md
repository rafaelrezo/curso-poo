# Especificadores de acesso, encapsulamento, friend e herança em C++: protegendo ativos industriais

## Objetivos de aprendizagem

- Diferenciar `public`, `private` e `protected` como decisões de modelagem, e não apenas de sintaxe.
- Entender como o encapsulamento protege regras do objeto e por que `friend` deve ser tratado como exceção controlada.
- Reconhecer quando herança pública representa uma relacao `is-a` válida em C++ e como esse raciocinio aparece em Python.

**Tempo estimado:** 4h

## Vídeo da aula

![type:video](https://www.youtube.com/embed/hYp2Ki1u3ms)

---

## 1. Problema de modelagem: quando tudo e publico, qualquer parte do sistema pode quebrar o objeto

Na aula 2, atributos públicos ajudaram a visualizar o que uma classe guarda. Para aprender o conceito, isso funciona. Para manter software real, isso vira risco rapidamente.

Imagine um `SensorPressao` em uma estação de bombeamento:

- se `valorAtual` for publico, qualquer trecho do programa pode escrever `-50.0`;
- se `limiteAlarme` for publico, alguem pode zerar o limite sem critério;
- se o estado de bloqueio for publico, o objeto deixa de proteger a própria operação.

O problema não e "esconder por esconder". O problema é garantir que o objeto continue coerente ao longo da vida do programa.

| Situacao | Se tudo for publico | Impacto prático |
|---|---|---|
| Atualizar leitura do sensor | qualquer função escreve qualquer valor | leitura inválida entra no sistema |
| Alterár limite de alarme | qualquer modulo muda o limite | alarmes falsos ou alarmes omitidos |
| Mudar area do equipamento | qualquer trecho reclassifica o ativo | rastreabilidade e manutenção ficam confusas |

**Ideia central desta aula:** especificador de acesso não e decoracao. Ele define quem pode tocar em cada parte do modelo e em quais condicoes.

---

## 2. Roteiro base com W3Schools

Os quatro links abaixo formam o trilho principal desta unidade. Eles organizam a progressao de um jeito didatico:

| Secao | Leitura base | Link direto para testar | O que observar |
|---|---|---|---|
| Especificadores de acesso | [Access Specifiers](https://www.w3schools.com/cpp/cpp_access_specifiers.asp) | [Try: acesso publico](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_access_public) | `public`, `private` e `protected` controlam quem pode tocar no estado |
| Encapsulamento | [Encapsulation](https://www.w3schools.com/cpp/cpp_encapsulation.asp) | [Try: encapsulamento com getter/setter](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_encapsulation) | esconder dados sensiveis e expor uma API pública coerente |
| `friend` | [Friend Functions](https://www.w3schools.com/cpp/cpp_friend_function.asp) | [Try: friend function](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_friend_function) | `friend` abre uma exceção pontual ao encapsulamento |
| Herança | [Inheritance](https://www.w3schools.com/cpp/cpp_inheritance.asp) | [Try: herança simples](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_inheritance) | a classe derivada reaproveita comportamento da base quando existe relacao `is-a` |

### Como usar esta trilha

1. Rode cada exemplo exatamente como está.
2. Identifique quem pode acessar cada membro e por qual caminho.
3. Troque o exemplo genérico por um ativo de engenharia, automação ou servicos.
4. Pergunte: "estou protegendo uma regra de negócio ou so escondendo dado sem critério?"

---

## 3. Especificadores de acesso e encapsulamento: quem pode fazer o que com o objeto?

Na W3Schools, a primeira mensagem importante e direta: C++ trabalha com três especificadores de acesso.

### Leitura rápida dos três níveis

| Especificador | Quem acessa | Melhor uso | Risco se abusar |
|---|---|---|---|
| `public` | qualquer código que tenha acesso ao objeto | operações que fazem parte do contrato publico da classe | expor demais e perder controle do estado |
| `private` | apenas a própria classe | dados internos e regras que não devem ser manipuladas por fora | criar getters/setters cosmeticos sem critério |
| `protected` | a própria classe e classes derivadas | pontos de extensão pensados para herança | virar um "atalho" para fugir de encapsulamento |

### O que isso significa na prática

- `public` deve expor comportamento observavel: ligar bomba, atualizar leitura, consultar estado.
- `private` deve guardar o que sustenta o contrato do objeto: limites, flags, valores internos, detalhes de calibracao.
- `protected` deve ser usado com parcimonia quando a classe-base precisa permitir extensão por subclasses.

### Ponto técnico importante

Segundo a documentação do `cppreference`, classes declaradas com `class` possuem membros `private` por padrão, enquanto `struct` usa `public` por padrão. Isso e detalhe de linguagem, mas ajuda a lembrar que C++ trata encapsulamento como parte do modelo, não como detalhe opcional.

### Entao o que e encapsulamento?

Na formulacao da W3Schools, encapsulamento significa esconder dados sensiveis e expor formas controladas de leitura é alteração.

Em sala, vale traduzir isso para uma regra mais operacional:

**Conceito -> impacto -> ação recomendada**

- conceito: nem todo atributo deve ficar disponivel para escrita externa;
- impacto: o objeto pode garantir estado coerente e reduzir efeitos colateráis;
- ação recomendada: exponha métodos públicos que representem intenções do domínio, não apenas campos abertos.

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

Aqui, a classe não expoe o atributo diretamente. Ela expoe uma ação pública que preserva a faixa de operação.

### Erros comuns nesta virada de nível

- transformar todo atributo em `public` por comodidade;
- criar `get` e `set` para tudo, sem regra alguma;
- usar `protected` em qualquer campo porque "talvez uma subclasse precise";
- esquecer que o objetivo não e esconder nomes, e sim proteger invariantes.

---

## 4. `friend`: exceção controlada, não padrão de modelagem

Depois de estudar encapsulamento, o aluno costuma perguntar: "então nunca mais posso acessar dado privado de fora?"

Em C++, a resposta e: **normalmente não, mas existe a palavra-chave `friend` para casos bem específicos**.

### O que `friend` faz

- uma função `friend` não e método da classe;
- mesmo assim, ela pode acessar membros `private` e `protected`;
- esse acesso precisa ser declarado explicitamente pela classe.

### Quando faz sentido em exemplos didaticos e projetos reais

- funções de exibicao fortemente acopladas ao objeto, como `operator<<`;
- adaptadores de integração que precisam montar uma visão interna controlada;
- relatorios de diagnóstico ou manutenção que não justificam abrir toda a API pública.

### Quando comeca a ficar ruim

- quando várias funções externas so funcionam porque viraram `friend`;
- quando `friend` substitui uma API pública mal pensada;
- quando a classe precisa de "amigos demais" para ser usada.

### Regra de bolso

Se `friend` aparece como recurso excepcional, pode ser justificavel.

Se `friend` virou estratégia padrão de acesso, o encapsulamento provavelmente está fraco.

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

O ponto didatico não e "como burlar o `private`". O ponto e entender que a própria classe escolhe conceder um acesso excepcional e delimitado.

---

## 5. Herança: reutilizar comportamento sem copiar classe inteira

Na W3Schools, herança aparece como reuso de atributos e métodos entre uma classe-base e uma classe derivada. Isso está correto, mas ainda incompleto para um curso de engenharia.

### A pergunta certa não e "da para herdar?"

A pergunta certa e:

**"a classe derivada realmente e um tipo mais específico da classe-base?"**

Se a resposta for sim, herança pública pode fazer sentido.

Se a resposta for não, provavelmente você precisa de composição, e não de herança.

### Leitura prática da herança pública

O `cppreference` resume a ideia central assim: em herança pública, o objeto derivado deve se comportar como um objeto da base. Em linguagem de modelagem, a relacao e `is-a`.

Exemplos que costumam funcionar:

- `SensorPressao` e um `AtivoIndustrial`;
- `BombaRecalque` e um `AtivoIndustrial`;
- `ValvulaControle` e um `AtivoIndustrial`.

Exemplo que costuma dar problema:

- `SalaDeControle` herdar de `Sensor`, so porque ambos tem `tag`.

### O que a classe derivada herda, e o que ela não herda

- membros `public` da base continuam acessiveis como `public` na derivada, se a herança for pública;
- membros `protected` continuam acessiveis para a derivada;
- membros `private` da base não ficam acessiveis diretamente na derivada.

Isso e importante: a subclasse reaproveita a base, mas não ganha direito automático de mexer em todos os detalhes internos.

### Comparação de técnicas de reutilizacao e controle

| Técnica/Padrão | Melhor uso | Esforço | Entregável | Limitação |
|---|---|---|---|---|
| Encapsulamento com `private` + API pública | proteger estado e validar regras do objeto | médio | classe mais segura e legivel | adiciona métodos e exige intencao de projeto |
| `protected` como ponto de extensão | classe-base feita para derivacao | médio | reaproveitamento com algum controle | aumenta acoplamento entre base e derivadas |
| `friend` controlado | diagnóstico, impressao ou integração muito próxima da classe | médio | acesso excepcional sem abrir tudo | pode corroer encapsulamento se usado em excesso |
| Herança pública | relacao `is-a` real entre base e derivada | médio | subtipo reutilizavel e coerente | hierarquia ruim espalha erro por várias classes |
| Composição | relacao `has-a` ou colaboracao entre objetos | médio | modelo mais desacoplado | exige delegacao explícita e menos "atalho" sintatico |

### Foco didatico desta aula

Para a trilha inicial do curso, a recomendacao prática e:

- priorize herança pública quando houver `is-a`;
- use `protected` com moderacao;
- evite `private inheritance` e `protected inheritance` como primeira escolha em exemplos iniciais;
- compare sempre com a alternativa de composição.

---

## 6. Exemplo aplicado: estação de bombeamento com sensor, bomba e auditoria de calibracao

No repositório, o exemplo compilável desta aula está em:

- [exemplo_estacao_bombeamento.cpp](./exemplo_estacao_bombeamento.cpp)

### O que esse exemplo mostra

- `AtivoIndustrial` encapsula `tag` e estado de bloqueio;
- `SensorPressao` e `BombaRecalque` herdam publicamente da classe-base;
- a area operacional fica disponivel para extensão controlada na hierarquia;
- uma função `friend` gera relatório de calibracao sem abrir todo o estado interno.

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

- `tag` não pode ser alteráda livremente por qualquer código;
- a subclasse usa o que a base oferece sem violar o `private`;
- `friend` aparece como exceção localizada, e não como porta geral de acesso;
- o domínio contínua próximo de sistemas industriais reais.

### Leitura guiada da saída

Ao executar, discuta com a turma:

1. quais dados ficaram protegidos na base;
2. quais operações a subclasse reaproveitou;
3. por que o relatório de calibracao não foi implementado como acesso publico geral;
4. em que ponto herança economizou duplicacao de código.

### Como compilar em sala

```bash
g++ -std=c++17 docs/fundamentos_poo_cpp_python/04_encapsulamento_heranca/exemplo_estacao_bombeamento.cpp -o exemplo_encapsulamento
./exemplo_encapsulamento
```

---

## 7. Ponte C++ -> Python

O conceito de encapsulamento e herança existe nas duas linguagens, mas o mecanismo não e identico.

### Em C++

- `private`, `protected` e `public` são parte formal da linguagem;
- `friend` permite uma exceção declarada explicitamente;
- herança pública comunica relacao `is-a` e preserva acesso conforme a base.

### Em Python

- não existe "privado real" no mesmo sentido de C++;
- o tutorial oficial recomenda tratar nomes com `_` como não públicos por convenção;
- herança usa `class Derivada(Base):` e `super()`;
- não existe equivalente direto a `friend`, então a saída usual e expor um método/propriedade pública bem pensada.

### Espelho em Python no repositório

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

### Comparação rápida

| Aspecto | C++ | Python | Impacto didatico |
|---|---|---|---|
| Controle de acesso | formal na linguagem | convencional na maior parte dos casos | C++ torna a fronteira mais explícita |
| Encapsulamento | `private` + API pública | `_atributo`, `@property` e disciplina de projeto | Python depende mais de convenção |
| Herança | `class Derivada : public Base` | `class Derivada(Base)` | a ideia de especializacao contínua |
| Sobrescrita | precisa respeitar a interface da base | o tutorial oficial lembra que métodos são efetivamente `virtual` | Python torna polimorfismo mais flexivel |
| `friend` | existe | não ha equivalente direto | em Python, prefira API pública clara |

### Recomendação prática por cenário

- em C++, use encapsulamento para defender invariantes e reduzir mutacao acidental;
- em Python, use convencoes e `@property` quando a leitura/escrita precisar de regra;
- nas duas linguagens, so use herança quando a especializacao fizer sentido no domínio.

---

## 8. Mini-caso prático: monitoramento de uma estação de tratamento

Imagine uma estação com estes ativos:

- um `SensorPressao` na linha de recalque;
- uma `BombaRecalque` que envia agua ao reservatório elevado;
- um serviço de manutenção que precisa auditar calibracao.

### Situacao 1. Leitura operacional

O sistema atualiza a pressão por um método controlado:

```cpp
sensor.atualizarValor(6.8);
```

Em vez de escrever no atributo diretamente, a classe decide como aceitar esse valor.

### Situacao 2. Especializacao

`SensorPressao` e `BombaRecalque` compartilham identidade, area e estado de bloqueio porque ambos são `AtivoIndustrial`.

### Situacao 3. Diagnostico excepcional

A manutenção precisa enxergar um `offsetCalibracao` interno.

Em vez de abrir esse campo para todo o sistema, a classe libera acesso pontual a uma função `friend`.

**Licao didatica:** encapsular não significa impedir trabalho. Significa decidir com clareza quais portas existem, quem pode usa-las e por que elas existem.

---

## 9. Exercícios para casa: atividade guiada de encapsulamento e herança

No repositório, ha um arquivo-base para a atividade:

- [projeto_estacao_bombeamento_base.cpp](./projeto_estacao_bombeamento_base.cpp)

Se quiser manter uma referência pronta para revisão posterior, use também:

- [projeto_estacao_bombeamento.cpp](./projeto_estacao_bombeamento.cpp)

### Enunciado

Modele uma pequena estação com:

- `AtivoCampo` como classe-base;
- `SensorNivel` como classe derivada;
- `BombaDosadora` como classe derivada.

### Regras da atividade

- a base deve encapsular `tag` e estado de bloqueio;
- as derivadas não devem acessar diretamente membros `private` da base;
- `protected` deve aparecer apenas onde realmente ajuda a extensão;
- o `friend` de `SensorNivel` deve ser usado apenas para inspecao técnica.

### Tarefas

1. Implementar os construtores das derivadas reaproveitando a classe-base.
2. Criar métodos públicos que validem alterações de nível e vazão.
3. Declarar e definir a função `friend` de inspecao do sensor.
4. Exibir no terminal um resumo dos objetos criados.
5. Justificar, em comentario curto, por que cada membro ficou `public`, `private` ou `protected`.

### Checklist de entrega

- o código compila;
- o estado importante ficou protegido;
- as derivadas reutilizam a base sem duplicacao desnecessaria;
- a função `friend` ficou localizada e justificavel;
- o terminal mostra um resumo legivel dos ativos.

### Comando sugerido para o aluno testar

```bash
g++ -std=c++17 docs/fundamentos_poo_cpp_python/04_encapsulamento_heranca/projeto_estacao_bombeamento.cpp -o projeto_bombeamento
./projeto_bombeamento
```

---

## 10. Videos e materiais complementares

### Vídeo principal desta unidade

- [Curso de C++ #65 - POO, Encapsulamento - P7 (CFB Cursos)](https://www.youtube.com/watch?v=hYp2Ki1u3ms)

### Vídeo complementar para herança

- [Curso de C++ #47 - POO, Classes, Herança - P4 (CFB Cursos)](https://www.youtube.com/watch?v=TLQzg0b-AY4)

### Laboratorio complementar

- [W3Schools - Access Specifiers](https://www.w3schools.com/cpp/cpp_access_specifiers.asp)
- [W3Schools - Encapsulation](https://www.w3schools.com/cpp/cpp_encapsulation.asp)
- [W3Schools - Friend Functions](https://www.w3schools.com/cpp/cpp_friend_function.asp)
- [W3Schools - Inheritance](https://www.w3schools.com/cpp/cpp_inheritance.asp)

### Observação didatica

O vídeo principal ajuda a abrir o assunto. A compreensao melhora quando o aluno alterna entre leitura, execução no navegador e adaptacao do exemplo para um domínio de engenharia.

---

## Perguntas de revisão rápida

1. Qual a diferença prática entre `private` e `protected` em uma hierarquia de classes?
2. Em que situacoes `friend` pode ser justificavel sem enfraquecer demais o encapsulamento?
3. Como decidir se uma relacao deve usar herança pública ou composição?

## Fontes de referência

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
