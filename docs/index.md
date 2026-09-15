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
- `09. Objetos colaborando e contratos`: associação, dependência e interfaces em demonstrações progressivas; prepara a prática A.
- `10. Exceções e recursos`: lançamento, propagação e limpeza; encerra a prática A, integrada ao capítulo 09.
- `11. Identidade, igualdade e coleções`: chaves estáveis, comparação e catálogos em demonstrações; prepara a prática B.
- `12. UML e modelagem de cenários`: fechar a Parte 1 com a prática B: catálogo e modelagem, integrada ao capítulo 11.
- `13. Parte 2 — Projeto Integrador`: começar por princípios de projeto e testes de objetos; aplicar a base em arquitetura, JSON, integração, padrões, persistência e comunicação.
- `14. Checklist profissional de entrega`: reunir as evidências da solução.

## 2. Continuidade para a turma atual

As seções 01 a 06 já ministradas permanecem preservadas. A seção 07 usa o [starter de fundamentos](https://github.com/rafaelrezo/poo-fundamentos-estacao); a seção 08 mantém sua atividade própria. As seções 09 a 12 retomam o fork iniciado em 07.

Os capítulos 09 e 11 concentram exposição dialogada e programas demonstrativos completos. Há **duas entregas no bloco 09–12**: prática A ao final do 10 (colaboração e falhas), e prática B ao final do 12 (catálogo e modelo). Cada atividade usa uma branch, commits do incremento guiado e da extensão, testes cumulativos e uma PR no próprio fork.

O starter fornece os mecanismos que não são foco da implementação: interfaces, fontes, limpeza, comparação, hash e infraestrutura de coleções. Os comandos `make test ETAPA=A` e `make test ETAPA=B` validam, respectivamente, até os contratos técnicos 11 e 13. Os IDs e as branches antigas permanecem disponíveis para compatibilidade, sem entregas adicionais. Forks antigos seguem o [guia de atualização](https://github.com/rafaelrezo/poo-fundamentos-estacao/blob/main/ATUALIZACAO.md).

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
| 09: colaboração e contratos, demonstrativo | 2h | — |
| 10: exceções e prática integrada A (09+10) | 2h | até 2h |
| 11: identidade e coleções, demonstrativo | 2h | — |
| 12: modelagem e prática integrada B (11+12) | 2h | até 2h |
| Total de estudo/prática dos blocos | 10h | até 8h |

As horas externas não integram as 60h presenciais. O bloco 09–12 passa de 8h para um orçamento de **até 4h externas**, incluindo preparação e revisão das duas práticas; as 4h previstas para 07–08 permanecem. Essa redução depende do recorte de tarefas e da infraestrutura fornecida, não apenas da fusão de entregas. Os tempos são estimativas a calibrar com a turma.

Em sala, o 09 e o 11 dedicam 2h às demonstrações; o 10 reserva aproximadamente 80 min à exposição e 40 min à prática A; o 12 reserva 60 min à modelagem e 60 min à prática B. Os vídeos são complementares dentro do tempo previsto, sem tarefas obrigatórias extras nos capítulos expositivos.

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
