# Sobrecarga de construtores em C++: modelando gemeos digitais na piramide de automacao

## Objetivos de aprendizagem

- Entender por que uma classe pode precisar de mais de um construtor para representar estados iniciais diferentes do mesmo objeto.
- Identificar como a sobrecarga de construtores e resolvida em C++ e quais cuidados de modelagem isso exige.
- Comparar o suporte em C++ com as alternativas disponiveis em Python para inicializacao flexivel de objetos.

**Tempo estimado:** 4h

## Video da aula

![type:video](https://www.youtube.com/embed/lkaDJMwwGXw)

---

## 1. Problema de modelagem: o mesmo equipamento pode nascer com informacoes diferentes

Na piramide de automacao, o mesmo elemento da planta pode ser conhecido em niveis diferentes de detalhe:

- na camada de campo, um `Sensor` pode ser descoberto apenas pela `tag`;
- na camada de supervisao, esse mesmo sensor pode receber unidade, faixa e valor atual;
- na camada de controle ou engenharia, o objeto pode nascer com dados mais completos, como limites, endereco e descricao funcional.

Se a classe tiver apenas um construtor, o desenvolvedor pode ser forcado a:

- criar objetos incompletos e "remendar" depois;
- repetir atribuicoes em varios pontos do codigo;
- perder clareza sobre quais dados sao realmente obrigatorios.

Sobrecarga de construtores resolve isso quando diferentes formas de criacao representam situacoes legitimas do dominio.

**Ideia central desta aula:** nao se trata de "ter varios jeitos de escrever o mesmo codigo". Trata-se de explicitar, no proprio modelo, diferentes formas validas de um objeto nascer.

| Camada da piramide | Informacao disponivel | Exemplo de criacao | Decisao didatica |
|---|---|---|---|
| Campo | apenas `tag` e tipo do ativo | `SensorAnalogico("LT-101")` | construtor minimo |
| Engenharia | `tag`, unidade e faixa | `SensorAnalogico("LT-101", "%", 0.0, 100.0)` | construtor intermediario |
| Supervisao/execucao | `tag`, faixa e valor atual | `SensorAnalogico("LT-101", "%", 0.0, 100.0, 63.8)` | construtor completo |

---

## 2. Roteiro base com W3Schools

Os exemplos da W3Schools continuam sendo uma boa porta de entrada para o assunto.

| Secao | Leitura base | Link direto para testar | O que observar |
|---|---|---|---|
| Construtores | [Constructors](https://www.w3schools.com/cpp/cpp_constructors.asp) | [Try: construtor com parametros](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_constructor_param) | um objeto pode nascer pronto |
| Sobrecarga de construtores | [Constructor Overloading](https://www.w3schools.com/cpp/cpp_constructors_overloading.asp) | [Try: constructor overloading](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_constructor_overloading) | a mesma classe pode ter mais de um construtor |
| Sobrecarga em geral | [Function Overloading](https://www.w3schools.com/cpp/cpp_function_overloading.asp) | [Try: overload de funcao](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_function_overloading) | a regra central e assinatura diferente |

### Como usar esta trilha

1. Rode primeiro o exemplo pronto sem alterar nada.
2. Identifique quais construtores existem e quais argumentos cada um recebe.
3. Pergunte: "cada forma de criacao corresponde a uma situacao real do dominio?"
4. Troque o exemplo generico por um elemento da automacao industrial.

---

## 3. Fundamentacao teorica: o que e sobrecarga de construtor?

Em C++, uma classe pode declarar mais de um construtor, desde que as assinaturas sejam diferentes em numero e/ou tipos de parametros.

### O que isso significa na pratica

- todos os construtores tem o mesmo nome da classe;
- o compilador escolhe qual usar com base nos argumentos fornecidos;
- essa escolha segue as regras de `overload resolution`;
- a sobrecarga nao existe para esconder ambiguidade, e sim para modelar formas validas de inicializacao.

### Quando faz sentido usar

- quando o objeto pode nascer com configuracao minima ou completa;
- quando existe um valor padrao coerente para parte dos dados;
- quando o dominio possui mais de um fluxo legitimo de criacao.

### Quando nao faz sentido usar

- quando os construtores fazem a mesma coisa com pequenas variacoes cosmeticas;
- quando comecam a surgir muitas assinaturas parecidas e ambiguas;
- quando um metodo de fabrica ou uma classe auxiliar comunicaria melhor a intencao.

### Leitura didatica no contexto da piramide

Um gemeo digital simples de um `SensorAnalogico` pode nascer de tres maneiras:

1. apenas com `tag`, porque o ativo foi detectado mas ainda nao parametrizado;
2. com `tag`, `unidade` e faixa, porque veio da engenharia;
3. com tudo isso mais o `valorAtual`, porque veio da supervisao em tempo de execucao.

Se essas tres situacoes sao reais, a classe pode comunicar isso diretamente pelos construtores.

### Regra de bolso para o aluno

Use sobrecarga quando cada assinatura contar uma historia diferente sobre a criacao do objeto.

Evite sobrecarga quando:

- a diferenca for apenas "comodidade de quem escreve";
- a classe passar a aceitar combinacoes demais e perder clareza;
- um construtor com parametros opcionais ou um metodo nomeado comunicar melhor a intencao.

---

## 4. Como C++ suporta isso formalmente

O suporte em C++ e direto: construtores sao funcoes especiais da classe e participam do mecanismo de sobrecarga da linguagem.

### Regras importantes

- construtor nao tem tipo de retorno;
- a diferenca entre construtores vem da assinatura, nao do nome;
- o compilador escolhe o construtor mais adequado na criacao do objeto;
- listas de inicializacao (`: membro(valor)`) sao a forma idiomatica de inicializar atributos;
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

Aqui nao ha tres classes diferentes. Ha tres formas reconhecidas de criar o mesmo tipo de objeto.

### Ponto tecnico importante

Se dois construtores ficarem parecidos demais, a chamada pode virar ambigua. Exemplo comum:

```cpp
Sensor(std::string tag, double valor);
Sensor(std::string tag, int valor);
```

Uma chamada mal planejada pode exigir conversoes e confundir o iniciante. Por isso, sobrecarga boa precisa de intencao clara.

### Boas praticas minimas em C++

- prefira listas de inicializacao em vez de criar o objeto vazio e atribuir depois;
- concentre a validacao no construtor mais completo;
- use delegacao para evitar repetir logica em varios construtores;
- considere `explicit` em construtores de um parametro para evitar conversoes implicitas acidentais.

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

- criar um construtor "gigante" com parametros demais sem nome claro;
- repetir a mesma validacao em todos os construtores;
- deixar o objeto nascer em estado invalido e tentar consertar depois;
- usar sobrecarga onde um metodo nomeado deixaria o fluxo mais legivel.

---

## 5. Exemplo aplicado: gemeos digitais de sensores, atuadores e controladores

No repositorio, o exemplo compilavel desta aula esta em:

- [exemplo_piramide_automacao.cpp](./exemplo_piramide_automacao.cpp)

### O que esse exemplo mostra

- `SensorAnalogico` com construtor minimo, intermediario e completo;
- `ValvulaControle` com construtor padrao e com abertura inicial;
- `ControladorPID` com construtor completo e construtor delegado;
- objetos nascendo em estados diferentes, mas coerentes com o dominio.

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
- a sobrecarga mostra diferentes niveis de detalhamento do mesmo ativo;
- o aluno percebe que o construtor nao e "enfeite sintatico", mas uma decisao de modelagem;
- o dominio de automacao ajuda a visualizar o objeto como representacao de equipamento real.

### Leitura guiada da saida

Ao executar, discuta com a turma:

- qual objeto nasceu com dados minimos;
- qual objeto representa um ativo ja parametrizado pela engenharia;
- qual objeto representa um ativo com dado de processo ja conhecido;
- onde a delegacao evitou repeticao no codigo.

### Como compilar em sala

```bash
g++ -std=c++17 docs/fundamentos_poo_cpp_python/03_sobrecarga_construtores/exemplo_piramide_automacao.cpp -o exemplo_piramide
./exemplo_piramide
```

---

## 6. Ponte C++ -> Python

O conceito de inicializacao existe nas duas linguagens, mas o mecanismo nao e o mesmo.

### Em C++

Ha suporte nativo a multiplos construtores na mesma classe.

```cpp
class SensorAnalogico {
public:
    SensorAnalogico(std::string tag) : tag(tag) {}
    SensorAnalogico(std::string tag, std::string unidade, double minimo, double maximo)
        : tag(tag), unidade(unidade), minimo(minimo), maximo(maximo) {}
};
```

### Em Python

Python nao oferece sobrecarga real de `__init__` como em C++. Se voce escrever dois `__init__`, o ultimo sobrescreve o anterior.

```python
class SensorAnalogico:
    def __init__(self, tag, unidade="desconhecida", minimo=0.0, maximo=100.0, valor=0.0):
        self.tag = tag
        self.unidade = unidade
        self.minimo = minimo
        self.maximo = maximo
        self.valor = valor
```

### Entao Python nao suporta?

**Resposta didatica:** nao do mesmo jeito que C++.

Python costuma resolver o problema com:

- parametros opcionais em um unico `__init__`;
- argumentos nomeados;
- metodos de fabrica com `@classmethod`;
- em alguns casos, `dataclasses` para reduzir codigo repetitivo.
- em projetos maiores, funcoes de fabrica ou classes auxiliares para manter a criacao legivel.

### Exemplo de construtor alternativo em Python

No repositorio, o espelho em Python esta em:

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

### Comparacao rapida

| Aspecto | C++ | Python | Impacto didatico |
|---|---|---|---|
| Multiplos construtores | sim, nativamente | nao, no mesmo formato | C++ torna a diferenca de assinatura parte da modelagem |
| Inicializacao flexivel | sobrecarga e delegacao | defaults e `@classmethod` | Python favorece flexibilidade sintatica |
| Risco para o iniciante | ambiguidade de sobrecarga | objetos excessivamente permissivos | em ambos os casos, a modelagem precisa ser intencional |

### Comparacao de tecnicas de inicializacao

| Tecnica/Padrao | Melhor uso | Esforco | Entregavel | Limitacao |
|---|---|---|---|---|
| Sobrecarga de construtores em C++ | estados iniciais bem distintos e tipados | baixo a medio | objeto coerente ja na criacao | pode gerar ambiguidade se as assinaturas forem parecidas |
| `__init__` com parametros opcionais em Python | classes pequenas com poucas variacoes | baixo | inicializacao curta e flexivel | aceita combinacoes ruins com facilidade |
| `@classmethod` nomeado em Python | fluxos de criacao com nomes claros | medio | codigo mais legivel para quem le | aumenta a quantidade de metodos |
| Metodo de fabrica/classe auxiliar | regra de criacao mais rica ou com validacoes externas | medio a alto | construcao mais explicita e organizada | adiciona mais camadas ao exemplo |

### Recomendacao pratica por cenario

- use sobrecarga em C++ quando o tipo e as assinaturas ajudarem a contar a historia do dominio;
- use `@classmethod` em Python quando quiser nomes claros para diferentes fluxos de criacao;
- evite "um unico construtor faz tudo" quando a inicializacao comecar a exigir muitos `if`s.

---

## 7. Mini-caso pratico: um gemeo digital ao longo da piramide

Imagine a linha abaixo:

- um transmissor de nivel `LT-101` na camada de campo;
- uma valvula `XV-101` comandada pelo CLP;
- um controlador `LIC-101` na camada de controle.

### Situacao 1. Cadastro inicial

O sistema de engenharia conhece apenas a `tag` do equipamento.

```cpp
SensorAnalogico lt101("LT-101");
```

### Situacao 2. Parametrizacao

Agora o sistema recebe unidade e faixa de operacao.

```cpp
SensorAnalogico lt101("LT-101", "%", 0.0, 100.0);
```

### Situacao 3. Execucao em planta

O supervisorio passa a leitura atual do processo.

```cpp
SensorAnalogico lt101("LT-101", "%", 0.0, 100.0, 63.8);
```

**Licao didatica:** o objeto continua sendo `SensorAnalogico`. O que muda e a quantidade de informacao confiavel disponivel no momento da criacao.

---

## 8. Projeto exemplo para seguir em sala

### Cenario

Voce vai representar um pequeno trecho de uma estacao automatizada de mistura:

- um sensor de nivel;
- uma valvula de entrada;
- um controlador PID simples.

### Objetivo de demonstracao

1. Mostrar mais de um construtor por classe.
2. Criar objetos com diferentes niveis de configuracao.
3. Exibir no terminal o estado inicial de cada gemeo digital.

### Roteiro de conducao em sala

1. Compile e execute o arquivo base.
2. Mostre qual construtor foi chamado em cada criacao.
3. Peca aos alunos para prever a saida antes de rodar.
4. Troque valores e acrescente um novo ativo com outra forma de criacao.
5. Discuta qual construtor representa melhor cada etapa da planta.

### Saida esperada da discussao

- quando usar construtor minimo;
- quando exigir mais parametros;
- quando a flexibilidade melhora o modelo;
- quando a flexibilidade piora a clareza.

### Perguntas que valem a pena fazer em sala

1. Este construtor realmente representa um estado inicial valido?
2. Ha alguma sobrecarga que esta sobrando?
3. A leitura da chamada deixa claro o que o objeto sabe ao nascer?
4. Em Python, eu preferiria defaults ou um metodo nomeado?

---

## 9. Exercicios para casa: projeto basico de pratica

No repositorio, ha um arquivo-base para a atividade:

- [projeto_casa_automacao_base.cpp](./projeto_casa_automacao_base.cpp)

Se quiser levar uma referencia pronta para revisao posterior, mantenha tambem:

- [projeto_casa_automacao.cpp](./projeto_casa_automacao.cpp)

### Enunciado

Modele tres gemeos digitais simples:

- `SensorNivel`
- `Bomba`
- `ControladorTanque`

### Regras da atividade

Cada classe deve ter:

- um construtor minimo, com identidade basica;
- um construtor mais completo, com parametros de operacao;
- um metodo `exibirResumo()`.

### Tarefas

1. Criar pelo menos dois objetos de cada classe usando construtores diferentes.
2. Mostrar no terminal como cada objeto nasceu.
3. Explicar, em comentario curto, por que cada sobrecarga faz sentido no dominio.
4. Evitar criar construtores que deixem o objeto em estado absurdo.

### Checklist de entrega

- o codigo compila;
- cada classe possui ao menos duas formas validas de criacao;
- o nome dos parametros ajuda a entender a intencao;
- o metodo `exibirResumo()` mostra claramente o estado inicial do objeto;
- ha um comentario curto justificando cada sobrecarga criada.

### Resultado esperado

Um programa pequeno, legivel e compilavel, cujo foco seja apenas reforcar o conceito de sobrecarga de construtores.

### Desafio opcional

Adicione validacao simples:

- bomba com velocidade entre `0` e `100`;
- sensor com faixa minima menor que maxima;
- controlador com ganho proporcional positivo.

### Comando sugerido para o aluno testar

```bash
g++ -std=c++17 docs/fundamentos_poo_cpp_python/03_sobrecarga_construtores/projeto_casa_automacao.cpp -o projeto_casa
./projeto_casa
```

---

## 10. Videos e materiais complementares

### Video principal desta unidade

- [Tutorial C++ - Construtores (MrAppleBR)](https://www.youtube.com/watch?v=lkaDJMwwGXw)

### Video/laboratorio complementar

- [W3Schools - Constructor Overloading](https://www.w3schools.com/cpp/cpp_constructors_overloading.asp)
- [W3Schools - Try it Yourself: constructor overloading](https://www.w3schools.com/cpp/trycpp.asp?filename=demo_constructor_overloading)

### Observacao didatica

O video principal e curto e funciona bem como aquecimento. A parte especifica de sobrecarga fica mais forte quando o aluno le, roda e modifica o exemplo interativo da W3Schools e, na sequencia, traduz o dominio para automacao.

---

## Perguntas de revisao rapida

1. Quando a sobrecarga de construtores melhora a modelagem de um objeto?
2. Como o compilador de C++ decide qual construtor chamar?
3. Por que Python resolve esse problema de forma diferente de C++?

## Fontes de referencia

- https://www.w3schools.com/cpp/cpp_constructors.asp
- https://www.w3schools.com/cpp/cpp_constructors_overloading.asp
- https://www.w3schools.com/cpp/cpp_function_overloading.asp
- https://en.cppreference.com/w/cpp/language/constructor
- https://en.cppreference.com/w/cpp/language/overload_resolution
- https://docs.python.org/3/tutorial/classes.html
- https://docs.python.org/3/library/dataclasses.html
