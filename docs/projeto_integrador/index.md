# Projeto Integrador

## Objetivos de aprendizagem

- Aplicar POO em uma solucao de ponta a ponta em equipe.
- Integrar modelagem de dominio, persistencia e interface.
- Utilizar padroes de projeto basicos com criterio.

**Tempo estimado:** 21h

## Video da aula

![type:video](https://www.youtube.com/embed/8pDqJVdNa44)

---

## 1. Desafio proposto

Construir um sistema de monitoramento e acao para contexto de cidade/industria com:

- modulo de coleta/simulacao (C++),
- modulo de servico e dados (Python),
- painel de supervisao (Streamlit).

## 2. Entregaveis minimos

- Modelo de dominio (classes e relacoes).
- Codigo versionado com PRs.
- Persistencia local com SQLite.
- Integracao por arquivo JSON ou API local.
- Demo funcional.

## 3. Padroes sugeridos

- Strategy para regras variaveis.
- Factory para criacao de componentes.
- Observer para notificacoes de eventos.

## Mini-caso pratico

Equipe implementa alarme inteligente de temperatura: sensores enviam leituras, regras classificam risco e painel exibe estado em tempo real.

## Perguntas de revisao rapida

1. Qual parte do sistema mais se beneficia de Strategy?
2. Como desacoplar coleta e visualizacao?
3. Quais evidencias mostram colaboracao efetiva da equipe?

## Fontes de referencia

- https://docs.streamlit.io/
- https://www.sqlite.org/docs.html
- https://docs.python.org/3/library/json.html
