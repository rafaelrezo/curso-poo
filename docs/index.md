# Curso de Programação Orientada a Objetos (POO)

## Objetivos de aprendizagem

- Compreender e aplicar os conceitos centrais de POO em problemas reais de engenharia.
- Evoluir de C/procedural para modelagem orientada a objetos com C++ e Python.
- Entregar software com práticas de mercado: Git, revisão, testes e colaboracao.

**Tempo estimado:** 60h (30 encontros de 2h)

## Vídeo de contexto

![type:video](https://www.youtube.com/embed/4hpUALY4Rrk)

---

## 1. Conteudo liberado hoje

- `01. Fundamentos Git e IDE`: preparar ambiente, criar repositório, sincronizar e colaborar.
- `02. Fundamentos de POO em C++ e Python`: iniciar classes, objetos, métodos e construtores com exemplos guiados.
- `03. Sobrecarga de construtores em C++`: modelar diferentes formas de nascimento do objeto com foco em automação industrial.
- `04. Acesso, encapsulamento e herança em C++`: proteger estado interno, discutir `friend` com critério e reutilizar comportamento com hierarquias simples.
- `05. Polimorfismo em C++`: comandar diferentes ativos por uma interface comum, com `virtual` e `override`.
- `06. Arquivos e JSON para integração`: gravar telemetria em C++ e consumir os dados em Python com Streamlit.
- `07. Datas, horarios e timestamps`: representar instantes, duracoes e horarios em JSON com UTC.
- `08. UML e Diagrama de Classes`: modelar a estrutura do sistema antes de codificar e relacionar UML com encapsulamento, composição e herança.
- `09. Parte 2 - Projeto Integrador`: evoluir a integração entre controlador C++ e mini-SCADA Python com modelagem, listas dinâmicas, validação, banco de dados e tratamento de exceções.
- As demais unidades avançadas continuam no repositório, mas seguem ocultadas enquanto a trilha publicada é consolidada.

## 2. Trilha inicial do curso

1. Uso do VS Code para C++ e Python (criar, compilar e executar).
2. Fundamentos de Git e GitHub no fluxo prático.
3. Criação do repositório e sincronizacao com remoto.
4. Introducao a classes, objetos, métodos e construtores em C++.
5. Sobrecarga de construtores em C++ com comparação didatica com Python.
6. Encapsulamento, `friend` e herança em C++ para proteger invariantes e especializar ativos do domínio.
7. Polimorfismo em C++ para tratar diferentes equipamentos por um contrato único.
8. Manipulacao de arquivos e JSON para integrar C++ e Python.
9. Datas, horarios e timestamps para telemetria, logs e ordenacao de eventos.
10. UML e diagrama de classes para representar estrutura, responsabilidade e relações antes da implementação.
11. Evolucao da integração entre controlador C++ e mini-SCADA Python com listas dinâmicas, banco de dados e tratamento controlado de erros.

## 3. Resultado esperado nesta virada de unidade

- Cada aluno com repositório funcional no GitHub.
- Autonomia mínima para clonar, versionar e sincronizar alterações.
- Base conceitual suficiente para ler código orientado a objetos, representar a estrutura do domínio em UML, entender diferentes formas de inicializacao, proteger estado, reconhecer quando um mesmo comando deve acionar comportamentos distintos e integrar dados entre C++ e Python por JSON.

## Mini-caso prático

Um aluno cria e sincroniza seu repositório; na sequência, le e adapta exemplos de `Sensor`, `Valvula`, `Controlador` e `AtuadorMalha` em C++, transforma esse entendimento em um diagrama de classes UML, gera telemetria em JSON e consome esses dados em um painel Python simples.

## Perguntas de revisão rápida

1. Qual a diferença entre `commit` e `push`?
2. O que muda quando passamos de variáveis soltas para um objeto com estado e comportamento?
3. Por que JSON ajuda a integrar programas escritos em linguagens diferentes?

## Fontes de referência

- https://docs.github.com/en/get-started/start-your-journey/hello-world
- https://code.visualstudio.com/docs
- https://docs.github.com/en/education/manage-coursework-with-github-classroom
- https://docs.python.org/3/library/json.html
- https://docs.streamlit.io/get-started/installation
