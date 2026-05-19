# Projeto Integrador

## Objetivos de aprendizagem

- Aplicar POO em uma solução de ponta a ponta em equipe.
- Integrar modelagem de domínio, persistência e interface.
- Utilizar padrões de projeto básicos com critério.

**Tempo estimado:** 21h

## Vídeo da aula

![type:video](https://www.youtube.com/embed/8pDqJVdNa44)

---

## 1. Desafio proposto

Construir um sistema de monitoramento e ação para contexto de cidade/industria com:

- modulo de coleta/simulação (C++),
- modulo de serviço e dados (Python),
- painel de supervisão (Streamlit).

## 2. Entregaveis mínimos

- Modelo de domínio (classes e relações).
- Código versionado com PRs.
- Persistência local com SQLite.
- Integração por arquivo JSON ou API local.
- Demo funcional.

## 3. Padrões sugeridos

- Strategy para regras variáveis.
- Factory para criação de componentes.
- Observer para notificações de eventos.

## Mini-caso prático

Equipe implementa alarme inteligente de temperatura: sensores enviam leituras, regras classificam risco e painel exibe estado em tempo real.

## Perguntas de revisão rápida

1. Qual parte do sistema mais se beneficia de Strategy?
2. Como desacoplar coleta e visualizacao?
3. Quais evidencias mostram colaboracao efetiva da equipe?

## Fontes de referência

- https://docs.streamlit.io/
- https://www.sqlite.org/docs.html
- https://docs.python.org/3/library/json.html
