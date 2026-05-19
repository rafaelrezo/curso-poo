# Aula 7 - Persistência em CSV e SQLite

## Objetivos de aprendizagem

- Registrar leituras, comandos e alarmes para análise posterior.
- Usar CSV como primeira estratégia de persistência.
- Entender SQLite como evolução natural para consultas mais organizadas.

**Tempo estimado:** 1h30.

## Vídeo da aula

![type:video](https://www.youtube.com/embed/ZFDSPSU6JMw)

---

## 1. Contextualização do problema

Um supervisor que mostra apenas o valor atual ajuda pouco na análise. Para entender comportamento da estação, precisamos de histórico:

- como o nível variou ao longo do tempo;
- quando a bomba ligou ou desligou;
- quantos alarmes ocorreram;
- qual sensor gerou mais ocorrências.

Persistência significa salvar dados de forma que sobrevivam ao encerramento do programa.

---

## 2. CSV primeiro

CSV é um bom primeiro passo porque:

- é texto simples;
- abre em planilhas;
- é fácil de gerar em C++ e ler em Python;
- não exige servidor de banco de dados.

### Arquivo `leituras.csv`

```csv
timestamp,tag,valor,unidade,status
2026-03-25T10:30:00-03:00,LT-101,58.4,%,operando
2026-03-25T10:31:00-03:00,PT-201,2.8,bar,operando
```

### Escrita simples em C++

```cpp
#include <fstream>
#include <string>
using namespace std;

void salvarLeituraCsv(const string& caminho, const string& linha) {
    ofstream arquivo(caminho, ios::app);

    if (!arquivo) {
        throw runtime_error("Não foi possível abrir o CSV");
    }

    arquivo << linha << "\n";
}
```

---

## 3. Leitura no supervisor

```python
from pathlib import Path

import pandas as pd
import streamlit as st

caminho = Path("dados/leituras.csv")

if caminho.exists():
    df = pd.read_csv(caminho)
    st.dataframe(df)
    st.line_chart(df[df["tag"] == "LT-101"], x="timestamp", y="valor")
else:
    st.info("Histórico ainda não foi gerado.")
```

---

## 4. Repository Pattern

Repository isola a forma de salvar e recuperar dados. O restante do sistema não precisa saber se o histórico está em CSV ou SQLite.

```cpp
class RepositorioLeituras {
public:
    virtual ~RepositorioLeituras() = default;
    virtual void salvar(const string& linhaCsv) = 0;
};

class RepositorioCsvLeituras : public RepositorioLeituras {
private:
    string caminho;

public:
    explicit RepositorioCsvLeituras(string caminho) : caminho(caminho) {}

    void salvar(const string& linhaCsv) override {
        ofstream arquivo(caminho, ios::app);
        arquivo << linhaCsv << "\n";
    }
};
```

---

## 5. Evolução para SQLite

SQLite é um banco de dados em arquivo. Ele é adequado para projetos pequenos porque não precisa de servidor.

### Tabelas sugeridas

```sql
CREATE TABLE leituras (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    timestamp TEXT NOT NULL,
    tag TEXT NOT NULL,
    valor REAL NOT NULL,
    unidade TEXT NOT NULL,
    status TEXT NOT NULL
);

CREATE TABLE comandos (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    timestamp TEXT NOT NULL,
    comando TEXT NOT NULL,
    executado INTEGER NOT NULL
);

CREATE TABLE alarmes (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    timestamp TEXT NOT NULL,
    origem TEXT NOT NULL,
    tipo TEXT NOT NULL,
    severidade TEXT NOT NULL
);
```

### Escrita em Python com `sqlite3`

```python
import sqlite3

conexao = sqlite3.connect("dados/estacao.db")
conexao.execute(
    """
    INSERT INTO leituras (timestamp, tag, valor, unidade, status)
    VALUES (?, ?, ?, ?, ?)
    """,
    ("2026-03-25T10:30:00-03:00", "LT-101", 58.4, "%", "operando"),
)
conexao.commit()
conexao.close()
```

---

## 6. Comparação de persistência

| Técnica/Padrão | Melhor uso | Esforço | Entregável | Limitação |
|---|---|---|---|---|
| CSV | histórico simples e inspecionável | baixo | arquivos `leituras.csv`, `comandos.csv`, `alarmes.csv` | consultas complexas ficam trabalhosas |
| SQLite | histórico estruturado e consultas | médio | arquivo `estacao.db` | exige SQL básico |
| Repository | trocar armazenamento sem espalhar mudanças | médio | interface e implementações | aumenta número de classes |

### Recomendação prática por cenário

- Use CSV para a primeira entrega funcional.
- Use SQLite se a equipe quiser filtros, consultas e dashboards melhores.
- Use Repository se a troca entre CSV e SQLite for parte da arquitetura defendida.

---

## 7. Exercícios práticos

1. Salvar leituras em `dados/leituras.csv`.
2. Salvar comandos em `dados/comandos.csv`.
3. Salvar alarmes em `dados/alarmes.csv`.
4. Ler `leituras.csv` no Streamlit e exibir gráfico.
5. Criar opcionalmente um banco SQLite com as três tabelas.

---

## 8. Checklist de entrega

- [ ] Leituras são persistidas.
- [ ] Comandos são persistidos.
- [ ] Alarmes são persistidos.
- [ ] O supervisor mostra histórico em tabela.
- [ ] O supervisor mostra pelo menos um gráfico.
- [ ] O README explica se a equipe escolheu CSV ou SQLite.

---

## 9. Perguntas de revisão rápida

1. Por que histórico é importante em um supervisor?
2. Quando CSV deixa de ser suficiente?
3. Como Repository ajuda a reduzir acoplamento?

---

## 10. Desafios opcionais

- Criar filtros por `tag` e por `status` no Streamlit.
- Criar uma consulta SQLite para contar alarmes por severidade.
- Exportar um resumo diário em CSV.

---

## Fontes de referência

- [Python Docs - csv](https://docs.python.org/3/library/csv.html)
- [Python Docs - sqlite3](https://docs.python.org/3/library/sqlite3.html)
- [SQLite Docs](https://www.sqlite.org/docs.html)
- [Streamlit Docs - Charts](https://docs.streamlit.io/develop/api-reference/charts)
