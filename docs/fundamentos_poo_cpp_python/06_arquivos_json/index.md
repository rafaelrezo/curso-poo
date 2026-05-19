# Manipulacao de arquivos e JSON em C++ e Python

## Objetivos de aprendizagem

- Gravar e ler arquivos de texto em C++ e Python com cuidado sobre caminho, formato é erro de abertura.
- Entender JSON como contrato simples de dados entre aplicações escritas em linguagens diferentes.
- Construir um fluxo em que C++ gera telemetria em JSON e Python consome esses dados em um painel Streamlit.

**Tempo estimado:** 4h

## Vídeo da aula

![type:video](https://www.youtube.com/embed/ZFDSPSU6JMw)

---

## 1. Por que arquivos ainda importam?

Mesmo em sistemas conectados por rede, banco de dados e APIs, arquivos continuam aparecendo em tarefas reais:

- registrar logs de execução;
- salvar configuracoes;
- exportar leituras de sensores;
- trocar dados entre programas;
- manter histórico para auditoria e depuracao.

Para quem já programou em C, a ideia de arquivo não e nova. O ponto desta aula e reorganizar esse conhecimento em um fluxo mais próximo de aplicações modernas: um programa gera dados, outro programa interpreta esses dados e entrega uma visualizacao.

**Ideia central desta aula:** arquivo não e so "saída no disco". Arquivo também pode ser contrato de integração entre sistemas.

---

## 2. Arquivos em C++: escrita com `<fstream>`

Em C++, a biblioteca padrão oferece fluxos de arquivo em `<fstream>`:

| Classe | Melhor uso | Exemplo mental |
|---|---|---|
| `ofstream` | escrever em arquivo | exportar leituras |
| `ifstream` | ler de arquivo | carregar configuração |
| `fstream` | ler e escrever | atualizar arquivo existente |

### Exemplo mínimo de escrita

```cpp
#include <fstream>
#include <iostream>
using namespace std;

int main() {
    ofstream arquivo("saida.txt");

    if (!arquivo) {
        cerr << "Nao foi possivel criar o arquivo\n";
        return 1;
    }

    arquivo << "sensor=TQ-01\n";
    arquivo << "temperatura=27.4\n";
    return 0;
}
```

### O que observar

- `ofstream arquivo("saída.txt")` tenta abrir o arquivo para escrita.
- `if (!arquivo)` verifica falha de abertura.
- `<<` escreve no arquivo como se fosse `cout`.
- Ao final do escopo, o arquivo e fechado automaticamente pelo objeto.

### Erros comuns

- gravar no diretório errado e achar que o arquivo não foi criado;
- esquecer de verificar erro de abertura;
- misturar texto formatado para humanos com dados que outro programa precisa ler;
- usar virgula decimal em JSON quando o formato éxige ponto decimal.

---

## 3. Arquivos em Python: leitura é escrita com `pathlib` e `json`

Python também trabalha bem com arquivos de texto. Para caminhos, `pathlib.Path` deixa o código mais claro.

```python
from pathlib import Path

caminho = Path("saida.txt")
caminho.write_text("sensor=TQ-01\ntemperatura=27.4\n", encoding="utf-8")

conteudo = caminho.read_text(encoding="utf-8")
print(conteudo)
```

Para JSON, a biblioteca padrão já inclui o modulo `json`.

```python
import json
from pathlib import Path

dados = {
    "sensor": "TQ-01",
    "temperatura_c": 27.4,
    "ativo": True,
}

Path("leitura.json").write_text(
    json.dumps(dados, indent=2),
    encoding="utf-8",
)
```

### Ponte C++ -> Python

| Ação | C++ | Python | Impacto prático |
|---|---|---|---|
| Escrever texto | `ofstream` | `Path.write_text()` ou `open()` | persistir saída de execução |
| Ler texto | `ifstream` | `Path.read_text()` ou `open()` | carregar dados externos |
| Tratar JSON | biblioteca externa ou escrita controlada | `json` da biblioteca padrão | Python e mais direto para consumir JSON |
| Caminhos | strings, `filesystem` em C++17 | `pathlib.Path` | evita confusao entre Windows, Linux e WSL |

---

## 4. O que e JSON?

JSON, ou JavaScript Object Notation, e um formato textual para representar dados estruturados.

Um exemplo pequeno:

```json
{
  "tag": "TQ-01",
  "temperatura_c": 27.4,
  "ativo": true,
  "alarmes": ["alta_temperatura", "manutencao"]
}
```

JSON trabalha com poucos tipos:

| Tipo JSON | Exemplo | Equivalente comum em Python | Observação para C++ |
|---|---|---|---|
| objeto | `{ "tag": "TQ-01" }` | `dict` | costuma virar classe, struct ou mapa |
| array | `[1, 2, 3]` | `list` | costuma virar `vector` |
| string | `"TQ-01"` | `str` | precisa escapar aspas e barras |
| number | `27.4` | `int` ou `float` | decimal usa ponto, não virgula |
| boolean | `true` | `True` | cuidado com maiusculas diferentes |
| null | `null` | `None` | representa ausencia explícita |

### Por que JSON e importante para integração?

JSON virou comum porque e:

- legivel para humanos;
- fácil de gerar e validar;
- independente de linguagem;
- usado em APIs web, configuracoes, mensageria, logs estruturados e dashboards;
- bom o suficiente para passar dados entre um programa C++ e uma aplicação Python.

### Limites que o aluno precisa conhecer

- JSON não tem tipo nativo de data: timestamps costumam ser strings.
- JSON não aceita comentario.
- JSON não garante unidade física: `27.4` precisa vir com nome claro, como `temperatura_c`.
- JSON bem formado não significa JSON correto para o domínio.

---

## 5. Mini-caso prático: C++ gera telemetria, Python mostra painel

Imagine uma aplicação C++ simples simulando estatisticas de uma estação:

- uso de CPU;
- uso de memoria;
- temperatura de operação;
- instante de cada leitura.

No computador real dos alunos, capturar CPU e memoria de forma portavel exigiria APIs específicas do Windows, Linux ou bibliotecas externas. Para a aula, a decisão didatica e gerar valores aleatorios controlados. Assim o foco fica no contrato de dados.

O exemplo C++ está em:

- [gerador_telemetria_json.cpp](./gerador_telemetria_json.cpp)

Ele gera um arquivo chamado `telemetria.json` com esta estrutura:

```json
{
  "fonte": "simulador_cpp",
  "descricao": "telemetria simulada de uma estacao",
  "gerado_em": "2026-04-29T12:00:00Z",
  "leituras": [
    {
      "timestamp": "2026-04-29T12:00:00Z",
      "cpu_percent": 51.20,
      "memoria_percent": 64.10,
      "temperatura_c": 31.80
    }
  ]
}
```

### Como compilar e executar

No Linux ou WSL:

```bash
g++ -std=c++17 docs/fundamentos_poo_cpp_python/06_arquivos_json/gerador_telemetria_json.cpp -o gerador_telemetria
./gerador_telemetria
```

No Windows com MinGW, dentro da pasta do repositório:

```bash
g++ -std=c++17 docs/fundamentos_poo_cpp_python/06_arquivos_json/gerador_telemetria_json.cpp -o gerador_telemetria.exe
gerador_telemetria.exe
```

### Decisoes técnicas do exemplo

- O C++ usa `ofstream` para escrever o arquivo.
- O JSON e gerado manualmente para evitar dependencia externa na primeira aula.
- O timestamp usa UTC com sufixo `Z`.
- A função `escapar_json()` existe porque strings em JSON precisam tratar aspas, barras e quebras de linha.
- Os numeros usam `locale::classic()` para garantir ponto decimal.

Em projetos maiores, a recomendacao e usar uma biblioteca JSON em C++, como `nlohmann/json`, porque gerar JSON manualmente escala mal quando a estrutura cresce.

---

## 6. Consumindo o JSON com Streamlit

O exemplo Python está em:

- [app_streamlit_telemetria.py](./app_streamlit_telemetria.py)
- [requirements_streamlit.txt](./requirements_streamlit.txt)

O app procura `telemetria.json` no diretório atual ou permite upload do arquivo pela interface.

### Como executar

Depois de gerar `telemetria.json`, instale as dependencias e rode:

```bash
python -m pip install -r docs/fundamentos_poo_cpp_python/06_arquivos_json/requirements_streamlit.txt
streamlit run docs/fundamentos_poo_cpp_python/06_arquivos_json/app_streamlit_telemetria.py
```

### O que o Python faz

```python
import json
from pathlib import Path

dados = json.loads(Path("telemetria.json").read_text(encoding="utf-8"))
leituras = dados["leituras"]
```

O Python transforma o objeto JSON em estruturas nativas:

- objeto JSON vira `dict`;
- array JSON vira `list`;
- string JSON vira `str`;
- number JSON vira `int` ou `float`.

### Onde aparece a integração

O C++ não conhece Streamlit. O Streamlit não conhece o código C++.

Os dois concordam apenas sobre o arquivo:

```text
gerador_telemetria_json.cpp -> telemetria.json -> app_streamlit_telemetria.py
```

Esse e um principio importante de arquitetura: integrar sistemas por contrato reduz acoplamento.

---

## 7. Atividade guiada

### Enunciado

Adapte o gerador C++ para representar uma pequena planta de engenharia com três variáveis de processo.

Exemplos:

- reservatório: `nivel_percent`, `vazao_entrada_l_min`, `vazao_saida_l_min`;
- ambiente: `temperatura_c`, `umidade_percent`, `co2_ppm`;
- linha de producao: `pecas_minuto`, `falhas_minuto`, `energia_kw`.

### Regras

1. O arquivo final deve continuar sendo JSON válido.
2. Cada leitura deve ter `timestamp`.
3. Cada campo numérico deve deixar a unidade clara no nome.
4. O app Streamlit deve exibir pelo menos uma serie em gráfico.
5. O aluno deve explicar quais campos formam o contrato entre C++ e Python.

### Comando para validar JSON

Python pode validar e formatar o arquivo:

```bash
python -m json.tool telemetria.json
```

Se houver erro de virgula, aspas ou colchete, esse comando falha e mostra onde a estrutura quebrou.

---

## Perguntas de revisão rápida

1. Por que um arquivo JSON pode integrar um programa C++ e um programa Python sem que um conheca o código do outro?
2. Que problema aparece se um campo numérico vier como `"27,4"` em vez de `27.4`?
3. Em que situação você deixaria de gerar JSON manualmente em C++ e passaria a usar uma biblioteca?

## Fontes de referência

- https://en.cppreference.com/w/cpp/header/fstream
- https://en.cppreference.com/w/cpp/io/basic_ofstream
- https://www.json.org/json-en.html
- https://docs.python.org/3/library/json.html
- https://docs.python.org/3/library/pathlib.html
- https://docs.streamlit.io/get-started/installation
- https://docs.streamlit.io/develop/api-reference/data/st.dataframe
- https://docs.streamlit.io/develop/api-reference/charts/st.line_chart
