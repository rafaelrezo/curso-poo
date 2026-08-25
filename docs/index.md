# Curso de Programação Orientada a Objetos (POO)

## Objetivos de aprendizagem

- Compreender e aplicar os conceitos centrais de POO em problemas reais de engenharia.
- Evoluir de C/procedural para modelagem orientada a objetos com C++ e Python.
- Entregar software com práticas de mercado: Git, revisão, testes e colaboracao.

**Tempo estimado:** 60h (30 encontros de 2h)

## Vídeo de contexto

![type:video](https://www.youtube.com/embed/4hpUALY4Rrk)

---

## 1. Conteúdo da trilha

- `01. Fundamentos Git e IDE`: preparar ambiente, criar repositório, sincronizar e colaborar.
- `02. Revisão de Programação em C`: recuperar estruturas, funções, ponteiros, listas e arquivos em uma atividade cumulativa.
- `03. Do C aos objetos`: compreender a transição histórica, comparar os fluxos de execução e criar a primeira classe em C++ e Python.
- `04. Objetos válidos`: proteger invariantes com construtores, encapsulamento e testes de fronteira.
- `05. Composição e responsabilidades`: fazer sensor, bomba e controlador colaborarem sem concentrar funções.
- `06. Herança e polimorfismo`: representar diferentes sensores por um contrato comum.
- `07. Coleções dinâmicas`: evoluir da lista manual de C para `vector`, `unique_ptr` e `list`.
- `08. Contrato JSON e integração`: gerar telemetria em C++ e consumi-la primeiro no terminal e depois no Streamlit.
- `09. Exceções e consolidação`: manter o lote em execução, separar falhas e concluir o checkpoint integrado.
- `Parte 2 — Projeto Integrador`: aplicar padrões, banco de dados, comunicação, testes, qualidade e CI aos building blocks já funcionais.

## 2. Trilha inicial do curso

1. Uso do VS Code para C++ e Python (criar, compilar e executar).
2. Fundamentos de Git e GitHub no fluxo prático.
3. Criação do repositório e sincronizacao com remoto.
4. Transição da `struct` para classes e objetos equivalentes em C++ e Python.
5. Encapsulamento e invariantes para fazer o objeto nascer e permanecer válido.
6. Composição e UML mínimo para distribuir responsabilidades.
7. Herança e polimorfismo apenas quando aparece uma família de dispositivos.
8. Coleções dinâmicas e responsabilidade sobre memória.
9. Contrato JSONL, timestamp e primeira integração C++ → Python.
10. Exceções nas fronteiras e checkpoint integrado da Parte 1.
11. Padrões, persistência, comunicação, testes, qualidade e CI na Parte 2.

## 3. Resultado esperado nesta virada de unidade

- Cada aluno com repositório funcional no GitHub.
- Autonomia mínima para clonar, versionar e sincronizar alterações.
- Base conceitual suficiente para modelar objetos válidos, compor responsabilidades, usar polimorfismo e coleções com segurança e integrar C++ e Python por um contrato testado.

## Mini-caso prático

Um aluno cria e sincroniza seu repositório; depois evolui o mesmo sistema desde um `SensorNivel` até um controlador virtual simplificado em C++ que produz telemetria JSONL para um supervisório didático em Python.

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
