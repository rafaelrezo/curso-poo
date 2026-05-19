# Sobrecarga de construtores em C++: modelando gemeos digitais na piramide de automação

## Objetivos de aprendizagem

- Entender por que uma classe pode precisar de mais de um construtor para representar estados iniciais diferentes do mesmo objeto.
- Identificar como a sobrecarga de construtores e resolvida em C++ e quais cuidados de modelagem isso exige.
- Comparar o suporte em C++ com as alternativas disponiveis em Python para inicializacao flexivel de objetos.

**Tempo estimado:** 4h

## Vídeo da aula

![type:video](https://www.youtube.com/embed/lkaDJMwwGXw)

---

## 1. Problema de modelagem: o mesmo equipamento pode nascer com informações diferentes

Na piramide de automação, o mesmo elemento da planta pode ser conhecido em níveis diferentes de detalhe:

- na camada de campo, um `Sensor` pode ser descoberto apenas pela `tag`;
- na camada de supervisão, esse mesmo sensor pode receber unidade, faixa e valor atual;
- na camada de controle ou engenharia, o objeto pode nascer com dados mais completos, como limites, endereco e descrição funcional.

Se a classe tiver apenas um construtor, o desenvolvedor pode ser forcado a:

- criar objetos incompletos e "remendar" depois;
- repetir atribuicoes em vários pontos do código;
- perder clareza sobre quais dados são realmente obrigatórios.

Sobrecarga de construtores resolve isso quando diferentes formas de criação representam situacoes legitimas do domínio.

**Ideia central desta aula:** não se trata de "ter vários jeitos de escrever o mesmo código". Trata-se de explicitar, no próprio modelo, diferentes formas validas de um objeto nascer.

| Camada da piramide | Informacao disponivel | Exemplo de criação | Decisao didatica |
|---|---|---|---|
| Campo | apenas `tag` e tipo do ativo | `SensorAnalogico("LT-101")` | construtor mínimo |
| Engenharia | `tag`, unidade e faixa | `SensorAnalogico("LT-101", "%", 0.0, 100.0)` | construtor intermediario |
| Supervisao/execução | `tag`, faixa e valor atual | `SensorAnalogico("LT-101", "%", 0.0, 100.0, 63.8)` | construtor completo |

---

## 2. Roteiro base com W3Schools

Os exemplos da W3Schools continuam sendo uma boa porta de entrada para o assunto.

| Secao | Leitura base | Link direto para testar | O que observar |
|---|---|---|---|
| Construtores | [Constructors](https://www.w3schools.com/cpp/cpp_constructors.asp) | [Try: construtor com parâmetros](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_constructor_param) | um objeto pode nascer pronto |
| Sobrecarga de construtores | [Constructor Overloading](https://www.w3schools.com/cpp/cpp_constructors_overloading.asp) | [Try: constructor overloading](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_constructor_overloading) | a mesma classe pode ter mais de um construtor |
| Sobrecarga em geral | [Function Overloading](https://www.w3schools.com/cpp/cpp_function_overloading.asp) | [Try: overload de função](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_function_overloading) | a regra central e assinatura diferente |

### Como usar esta trilha

1. Rode primeiro o exemplo pronto sem alterar nada.
2. Identifique quais construtores existem e quais argumentos cada um recebe.
3. Pergunte: "cada forma de criação corresponde a uma situação real do domínio?"
4. Troque o exemplo genérico por um elemento da automação industrial.

---

## 3. Fundamentacao teórica: o que e sobrecarga de construtor?

Em C++, uma classe pode declarar mais de um construtor, desde que as assinaturas sejam diferentes em número e/ou tipos de parâmetros.

### O que isso significa na prática

- todos os construtores tem o mesmo nome da classe;
- o compilador escolhe qual usar com base nos argumentos fornecidos;
- essa escolha segue as regras de `overload resolution`;
- a sobrecarga não existe para esconder ambiguidade, e sim para modelar formas validas de inicializacao.

### Quando faz sentido usar

- quando o objeto pode nascer com configuração mínima ou completa;
- quando existe um valor padrão coerente para parte dos dados;
- quando o domínio possui mais de um fluxo legitimo de criação.

### Quando não faz sentido usar

- quando os construtores fazem a mesma coisa com pequenas variações cosmeticas;
- quando comecam a surgir muitas assinaturas parecidas e ambiguas;
- quando um método de fabrica ou uma classe auxiliar comunicaria melhor a intencao.

### Leitura didatica no contexto da piramide

Um gemeo digital simples de um `SensorAnalogico` pode nascer de três maneiras:

1. apenas com `tag`, porque o ativo foi detectado mas ainda não parametrizado;
2. com `tag`, `unidade` e faixa, porque veio da engenharia;
3. com tudo isso mais o `valorAtual`, porque veio da supervisão em tempo de execução.

Se essas três situacoes são reais, a classe pode comunicar isso diretamente pelos construtores.

### Regra de bolso para o aluno

Use sobrecarga quando cada assinatura contar uma historia diferente sobre a criação do objeto.

Evite sobrecarga quando:

- a diferença for apenas "comodidade de quem escreve";
- a classe passar a aceitar combinacoes demais e perder clareza;
- um construtor com parâmetros opcionais ou um método nomeado comunicar melhor a intencao.

---

## 4. Como C++ suporta isso formalmente

O suporte em C++ e direto: construtores são funções especiais da classe e participam do mecanismo de sobrecarga da linguagem.

### Regras importantes

- construtor não tem tipo de retorno;
- a diferença entre construtores vem da assinatura, não do nome;
- o compilador escolhe o construtor mais adequado na criação do objeto;
- listas de inicializacao (`: membro(valor)`) são a forma idiomatica de inicializar atributos;
- desde C++11, um construtor pode delegar para outro da mesma classe.

### Exemplo curto de ideia

```cpp
class Sensor {
public:
    Sensor() {}
    Sensor(std::string tag) {}
    Sensor(std::string tag, std::string unidade, double minimo, double maximo) {}
};
```

Aqui não ha três classes diferentes. Há três formas reconhecidas de criar o mesmo tipo de objeto.

### Ponto técnico importante

Se dois construtores ficarem parecidos demais, a chamada pode virar ambigua. Exemplo comum:

```cpp
Sensor(std::string tag, double valor);
Sensor(std::string tag, int valor);
```

Uma chamada mal planejada pode exigir conversoes e confundir o iniciante. Por isso, sobrecarga boa precisa de intencao clara.

### Boas práticas mínimas em C++

- prefira listas de inicializacao em vez de criar o objeto vazio e atribuir depois;
- concentre a validação no construtor mais completo;
- use delegacao para evitar repetir logica em vários construtores;
- considere `explicit` em construtores de um parâmetro para evitar conversoes implicitas acidentais.

```cpp
class SensorAnalogico {
public:
    explicit SensorAnalogico(std::string tag)
        : SensorAnalogico(tag, "desconhecida", 0.0, 100.0, 0.0) {}

    SensorAnalogico(std::string tag, std::string unidade, double minimo, double maximo)
        : SensorAnalogico(tag, unidade, minimo, maximo, minimo) {}

    SensorAnalogico(std::string tag, std::string unidade, double minimo, double maximo, double valor)
        : tag(tag), unidade(unidade), minimo(minimo), maximo(maximo), valorAtual(valor) {}
};
```

### Erros comuns em sala

- criar um construtor "gigante" com parâmetros demais sem nome claro;
- repetir a mesma validação em todos os construtores;
- deixar o objeto nascer em estado inválido e tentar consertar depois;
- usar sobrecarga onde um método nomeado deixaria o fluxo mais legivel.

---

## 5. Exemplo aplicado: gemeos digitais de sensores, atuadores e controladores

No repositório, o exemplo compilável desta aula está em:

- [exemplo_piramide_automacao.cpp](./exemplo_piramide_automacao.cpp)

### O que esse exemplo mostra

- `SensorAnalogico` com construtor mínimo, intermediario e completo;
- `ValvulaControle` com construtor padrão e com abertura inicial;
- `ControladorPID` com construtor completo e construtor delegado;
- objetos nascendo em estados diferentes, mas coerentes com o domínio.

### Trecho central em C++

```cpp
SensorAnalogico sensorDescoberto("LT-101");
SensorAnalogico sensorEngenharia("LT-101", "%", 0.0, 100.0);
SensorAnalogico sensorConfigurado("TT-201", "C", 0.0, 150.0, 72.5);

ValvulaControle valvulaPadrao("XV-101");
ValvulaControle valvulaOperando("FV-301", 45.0);

ControladorPID controladorRapido("LIC-101");
ControladorPID controladorCompleto("TIC-201", 2.0, 0.5, 0.1);
```

### Por que esse exemplo e didatico

- a `tag` comunica identidade do ativo;
- a sobrecarga mostra diferentes níveis de detalhamento do mesmo ativo;
- o aluno percebe que o construtor não e "enfeite sintatico", mas uma decisão de modelagem;
- o domínio de automação ajuda a visualizar o objeto como representacao de equipamento real.

### Leitura guiada da saída

Ao executar, discuta com a turma:

- qual objeto nasceu com dados mínimos;
- qual objeto representa um ativo já parametrizado pela engenharia;
- qual objeto representa um ativo com dado de processo já conhecido;
- onde a delegacao evitou repeticao no código.

### Como compilar em sala

```bash
g++ -std=c++17 docs/fundamentos_poo_cpp_python/03_sobrecarga_construtores/exemplo_piramide_automacao.cpp -o exemplo_piramide
./exemplo_piramide
```

---

## 6. Ponte C++ -> Python

O conceito de inicializacao existe nas duas linguagens, mas o mecanismo não e o mesmo.

### Em C++

Há suporte nativo a múltiplos construtores na mesma classe.

```cpp
class SensorAnalogico {
public:
    SensorAnalogico(std::string tag) : tag(tag) {}
    SensorAnalogico(std::string tag, std::string unidade, double minimo, double maximo)
        : tag(tag), unidade(unidade), minimo(minimo), maximo(maximo) {}
};
```

### Em Python

Python não oferece sobrecarga real de `__init__` como em C++. Se você escrever dois `__init__`, o ultimo sobrescreve o anterior.

```python
class SensorAnalogico:
    def __init__(self, tag, unidade="desconhecida", minimo=0.0, maximo=100.0, valor=0.0):
        self.tag = tag
        self.unidade = unidade
        self.minimo = minimo
        self.maximo = maximo
        self.valor = valor
```

### Entao Python não suporta?

**Resposta didatica:** não do mesmo jeito que C++.

Python costuma resolver o problema com:

- parâmetros opcionais em um único `__init__`;
- argumentos nomeados;
- métodos de fabrica com `@classmethod`;
- em alguns casos, `dataclasses` para reduzir código repetitivo.
- em projetos maiores, funções de fabrica ou classes auxiliares para manter a criação legivel.

### Exemplo de construtor alternativo em Python

No repositório, o espelho em Python está em:

- [exemplo_python_init_flexivel.py](./exemplo_python_init_flexivel.py)

```python
class SensorAnalogico:
    def __init__(self, tag, unidade="desconhecida", minimo=0.0, maximo=100.0, valor=0.0):
        self.tag = tag
        self.unidade = unidade
        self.minimo = minimo
        self.maximo = maximo
        self.valor = valor

    @classmethod
    def descoberto_na_rede(cls, tag):
        return cls(tag)

    @classmethod
    def parametrizado(cls, tag, unidade, minimo, maximo):
        return cls(tag, unidade, minimo, maximo, minimo)

    @classmethod
    def com_leitura_atual(cls, tag, unidade, minimo, maximo, valor):
        return cls(tag, unidade, minimo, maximo, valor)
```

### Comparação rápida

| Aspecto | C++ | Python | Impacto didatico |
|---|---|---|---|
| Multiplos construtores | sim, nativamente | não, no mesmo formato | C++ torna a diferença de assinatura parte da modelagem |
| Inicializacao flexivel | sobrecarga e delegacao | defaults e `@classmethod` | Python favorece flexibilidade sintatica |
| Risco para o iniciante | ambiguidade de sobrecarga | objetos excessivamente permissivos | em ambos os casos, a modelagem precisa ser intencional |

### Comparação de técnicas de inicializacao

| Técnica/Padrão | Melhor uso | Esforço | Entregável | Limitação |
|---|---|---|---|---|
| Sobrecarga de construtores em C++ | estados iniciais bem distintos e tipados | baixo a médio | objeto coerente já na criação | pode gerar ambiguidade se as assinaturas forem parecidas |
| `__init__` com parâmetros opcionais em Python | classes pequenas com poucas variações | baixo | inicializacao curta e flexivel | aceita combinacoes ruins com facilidade |
| `@classmethod` nomeado em Python | fluxos de criação com nomes claros | médio | código mais legivel para quem le | aumenta a quantidade de métodos |
| Método de fabrica/classe auxiliar | regra de criação mais rica ou com validacoes externas | médio a alto | construcao mais explícita e organizada | adiciona mais camadas ao exemplo |

### Recomendação prática por cenário

- use sobrecarga em C++ quando o tipo e as assinaturas ajudarem a contar a historia do domínio;
- use `@classmethod` em Python quando quiser nomes claros para diferentes fluxos de criação;
- evite "um único construtor faz tudo" quando a inicializacao começar a exigir muitos `if`s.

---

## 7. Mini-caso prático: um gemeo digital ao longo da piramide

Imagine a linha abaixo:

- um transmissor de nível `LT-101` na camada de campo;
- uma válvula `XV-101` comandada pelo CLP;
- um controlador `LIC-101` na camada de controle.

### Situacao 1. Cadastro inicial

O sistema de engenharia conhece apenas a `tag` do equipamento.

```cpp
SensorAnalogico lt101("LT-101");
```

### Situacao 2. Parametrizacao

Agora o sistema recebe unidade e faixa de operação.

```cpp
SensorAnalogico lt101("LT-101", "%", 0.0, 100.0);
```

### Situacao 3. Execucao em planta

O supervisório passa a leitura atual do processo.

```cpp
SensorAnalogico lt101("LT-101", "%", 0.0, 100.0, 63.8);
```

**Licao didatica:** o objeto contínua sendo `SensorAnalogico`. O que muda e a quantidade de informacao confiável disponivel no momento da criação.

---

## 8. Projeto exemplo para seguir em sala

### Cenário

Você vai representar um pequeno trecho de uma estação automatizada de mistura:

- um sensor de nível;
- uma válvula de entrada;
- um controlador PID simples.

### Objetivo de demonstração

1. Mostrar mais de um construtor por classe.
2. Criar objetos com diferentes níveis de configuração.
3. Exibir no terminal o estado inicial de cada gemeo digital.

### Roteiro de conducao em sala

1. Compile e execute o arquivo base.
2. Mostre qual construtor foi chamado em cada criação.
3. Peca aos alunos para prever a saída antes de rodar.
4. Troque valores e acrescente um novo ativo com outra forma de criação.
5. Discuta qual construtor representa melhor cada etapa da planta.

### Saida esperada da discussao

- quando usar construtor mínimo;
- quando exigir mais parâmetros;
- quando a flexibilidade melhora o modelo;
- quando a flexibilidade piora a clareza.

### Perguntas que valem a pena fazer em sala

1. Este construtor realmente representa um estado inicial válido?
2. Há alguma sobrecarga que está sobrando?
3. A leitura da chamada deixa claro o que o objeto sabe ao nascer?
4. Em Python, eu preferiria defaults ou um método nomeado?

---

## 9. Exercícios para casa: projeto básico de prática

No repositório, ha um arquivo-base para a atividade:

- [projeto_casa_automacao_base.cpp](./projeto_casa_automacao_base.cpp)

Se quiser levar uma referência pronta para revisão posterior, mantenha também:

- [projeto_casa_automacao.cpp](./projeto_casa_automacao.cpp)

### Enunciado

Modele três gemeos digitais simples:

- `SensorNivel`
- `Bomba`
- `ControladorTanque`

### Regras da atividade

Cada classe deve ter:

- um construtor mínimo, com identidade básica;
- um construtor mais completo, com parâmetros de operação;
- um método `exibirResumo()`.

### Tarefas

1. Criar pelo menos dois objetos de cada classe usando construtores diferentes.
2. Mostrar no terminal como cada objeto nasceu.
3. Explicar, em comentario curto, por que cada sobrecarga faz sentido no domínio.
4. Evitar criar construtores que deixem o objeto em estado absurdo.

### Checklist de entrega

- o código compila;
- cada classe possui ao menos duas formas validas de criação;
- o nome dos parâmetros ajuda a entender a intencao;
- o método `exibirResumo()` mostra claramente o estado inicial do objeto;
- ha um comentario curto justificando cada sobrecarga criada.

### Resultado esperado

Um programa pequeno, legivel e compilável, cujo foco seja apenas reforcar o conceito de sobrecarga de construtores.

### Desafio opcional

Adicione validação simples:

- bomba com velocidade entre `0` e `100`;
- sensor com faixa mínima menor que máxima;
- controlador com ganho proporcional positivo.

### Comando sugerido para o aluno testar

```bash
g++ -std=c++17 docs/fundamentos_poo_cpp_python/03_sobrecarga_construtores/projeto_casa_automacao.cpp -o projeto_casa
./projeto_casa
```

---

## 10. Videos e materiais complementares

### Vídeo principal desta unidade

- [Tutorial C++ - Construtores (MrAppleBR)](https://www.youtube.com/watch?v=lkaDJMwwGXw)

### Vídeo/laboratorio complementar

- [W3Schools - Constructor Overloading](https://www.w3schools.com/cpp/cpp_constructors_overloading.asp)
- [W3Schools - Try it Yourself: constructor overloading](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_constructor_overloading)

### Observação didatica

O vídeo principal e curto e funciona bem como aquecimento. A parte específica de sobrecarga fica mais forte quando o aluno le, roda e modifica o exemplo interátivo da W3Schools e, na sequência, traduz o domínio para automação.

---

## Perguntas de revisão rápida

1. Quando a sobrecarga de construtores melhora a modelagem de um objeto?
2. Como o compilador de C++ decide qual construtor chamar?
3. Por que Python resolve esse problema de forma diferente de C++?

## Fontes de referência

- https://www.w3schools.com/cpp/cpp_constructors.asp
- https://www.w3schools.com/cpp/cpp_constructors_overloading.asp
- https://www.w3schools.com/cpp/cpp_function_overloading.asp
- https://en.cppreference.com/w/cpp/language/constructor
- https://en.cppreference.com/w/cpp/language/overload_resolution
- https://docs.python.org/3/tutorial/classes.html
- https://docs.python.org/3/library/dataclasses.html
