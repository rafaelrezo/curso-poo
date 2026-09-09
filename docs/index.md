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
- `05. Construtores e sobrecarga`: oferecer diferentes formas de inicialização, compartilhar validação e comparar C++ com Python.
- `06. Composição e responsabilidades`: fazer sensor, bomba e controlador colaborarem sem concentrar funções.
- `07. Herança e especialização`: representar famílias de sensores e justificar a relação “é um”.
- `08. Polimorfismo e contratos`: consultar sensores diferentes por uma interface comum.
- `09. Objetos colaborando e contratos`: associação, dependência, agregação, abstração e interfaces em dois incrementos.
- `10. Exceções e recursos`: lançamento, propagação, exceções próprias, RAII e `finally`.
- `11. Identidade, igualdade e coleções`: chaves estáveis, comparação, hash, catálogos e tipos genéricos.
- `12. UML e modelagem de cenários`: fechar a Parte 1 com notações, multiplicidades e prática de modelagem.
- `13. Parte 2 — Projeto Integrador`: começar por princípios de projeto e testes de objetos; aplicar a base em arquitetura, JSON, integração, padrões, persistência e comunicação.
- `14. Checklist profissional de entrega`: reunir as evidências da solução.

## 2. Continuidade para a turma atual

As seções 01 a 06 já ministradas permanecem preservadas. A seção 07 usa o [starter de fundamentos](https://github.com/rafaelrezo/poo-fundamentos-estacao); a seção 08 mantém sua atividade própria. As seções 09 a 12 retomam o fork iniciado em 07.

Os capítulos antigos 09+10 formam agora o09; os antigos12+13 formam o11. Exceções passa a10; UML passa a12 e encerra a Parte 1. Princípios e testes abre a Parte 2 como seu capítulo 01. Os endereços anteriores encaminham ao conteúdo atual.

Os números de `ETAPA` e das branches existentes são identificadores técnicos dos contratos, preservados para não invalidar forks. A aula09 usa etapas09 e10; a aula10 usa etapa11; a aula11 usa etapas12 e13; UML12 verifica até a etapa13. A autoria de testes (etapa 14) começa na Parte 2, depois da UML.

## 3. Organização das 60h

As durações das páginas indicam estudo e prática. O calendário é **provisório**, condicionado à confirmação das horas já ministradas e da preparação externa efetiva.

| Bloco | Horas em sala propostas | Organização |
|---|---:|---|
| Seções 01–06 já ministradas | 24h reservadas | confirmar no diário; preservar conteúdo e entregas |
| Seções 07–12 | 10h | cinco encontros e trabalho orientado |
| Avaliações e checkpoints | 4h | evidências e defesa dos conceitos |
| Parte 2 — fundamentos de projeto e projeto | 22h | 2h iniciais de princípios/testes e 20h de marcos |
| Total condicionado à reserva inicial | 60h | 30 encontros de 2h |

A transferência de princípios e testes move 2h para o início da Parte 2; não elimina conteúdo nem acrescenta horas ao total.

| Fundamentos restantes | Sala | Trabalho orientado |
|---|---:|---:|
| 07–08: herança e polimorfismo | 2h | 4h |
| 09: colaboração e contratos | 2h | 2h |
| 10: exceções e recursos | 2h | — |
| 11: identidade e coleções | 2h | 4h |
| 12: oficina UML | 2h | 2h |
| Total de estudo/prática dos blocos | 10h | 12h |

As 12h externas não integram as 60h presenciais. A aula11 exige preparação prévia e implementação orientada do catálogo; sem essa preparação, o calendário precisa de ajuste. A fusão de capítulos organiza a continuidade, mas não reduz automaticamente o esforço de aprender os conceitos.

## 4. Resultado esperado ao concluir a Parte 1

O estudante implementa objetos válidos, relações, herança, substituição, abstrações, exceções, igualdade e coleções genéricas. Executa os testes fornecidos, explica seus resultados e representa um cenário em UML. A Parte 2 começa aprofundando princípios de projeto e a escrita de testes próprios, antes de integrar os componentes.

## Mini-caso prático

Um aluno cria e sincroniza seu repositório; depois evolui o mesmo sistema desde um `SensorNivel` até um controlador virtual simplificado em C++ que produz telemetria JSONL para um supervisório didático em Python.

## Perguntas de revisão rápida

1. Qual a diferença entre `commit` e `push`?
2. O que muda quando passamos de variáveis soltas para um objeto com estado e comportamento?
3. Como distinguir, em UML, um controlador que usa um sensor de uma classe que especializa um sensor?

## Fontes de referência

- https://docs.github.com/en/get-started/start-your-journey/hello-world
- https://code.visualstudio.com/docs
- https://docs.python.org/3/library/json.html
- https://docs.streamlit.io/get-started/installation
