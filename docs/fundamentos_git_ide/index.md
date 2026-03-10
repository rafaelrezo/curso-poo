# Fundamentos Git e IDE

## Objetivos de aprendizagem

- Configurar e usar o VS Code para criar, compilar e executar programas em C++ e Python.
- Aplicar o fluxo pratico de versionamento com Git e GitHub.
- Colaborar com colegas usando branch, fork e pull request.

**Tempo estimado:** 8h

## Video da aula

![type:video](https://www.youtube.com/embed/h96Z38oMJZA)

---

## 1. VS Code na pratica (antes do Git)

### 1.1 Preparacao minima

- Instalar `VS Code`.
- Instalar `g++` (toolchain C++).
- Verificar `python3 --version`.
- Instalar extensoes:
  - `C/C++` (Microsoft)
  - `Python` (Microsoft)

### 1.2 Tutorial didatico: C++

Crie `main.cpp`:

```cpp
#include <iostream>
using namespace std;

int main() {
    double temperatura = 26.5;
    cout << "Sensor ativo: " << temperatura << " C" << endl;
    return 0;
}
```

Compile e execute no terminal integrado do VS Code:

```bash
g++ -std=c++17 -Wall -Wextra -O2 main.cpp -o app_cpp
./app_cpp
```

### 1.3 Tutorial didatico: Python

Crie `main.py`:

```python
def main():
    temperatura = 26.5
    print(f"Sensor ativo: {temperatura} C")

if __name__ == "__main__":
    main()
```

Execute no terminal integrado:

```bash
python3 main.py
```

### 1.4 Videos complementares (VS Code)

- C++ no VS Code (pt-BR):
  ![type:video](https://www.youtube.com/embed/h96Z38oMJZA)
- Python no VS Code (pt-BR):
  ![type:video](https://www.youtube.com/embed/7Kpd6eprz4k)

### 1.5 Recursos didaticos visuais (docente)

As capturas de tela e videos curtos desta aula sao recursos de apoio preparados pelo professor.

- Os alunos **nao** precisam gerar screenshots para entrega.

Guias com capturas oficiais:

- VS Code (interface): https://code.visualstudio.com/docs/getstarted/userinterface
- C++ no VS Code: https://code.visualstudio.com/docs/languages/cpp
- Python no VS Code: https://code.visualstudio.com/docs/python/python-tutorial
- GitHub Hello World (branch, commit e PR): https://docs.github.com/en/get-started/start-your-journey/hello-world
- Fork e PR no GitHub: https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/working-with-forks

---

## 2. Git e GitHub na pratica

### Video da aula (Git)

![type:video](https://www.youtube.com/embed/4hpUALY4Rrk)

### 2.1 Conceitos principais

| Conceito | O que e | Exemplo pratico |
|---|---|---|
| Repositorio | Historico do projeto | pasta do trabalho no GitHub |
| Commit | Registro de uma mudanca | `feat: adiciona leitura de sensor` |
| Branch | Linha de desenvolvimento | `feature/ajuste-saida` |
| Push | Enviar commits para remoto | `git push origin main` |
| Pull Request | Proposta de integracao | revisao de codigo entre colegas |

### 2.2 Fluxo essencial

1. `git clone <url-do-repo>`
2. `git switch -c feature/minha-mudanca`
3. `git add .`
4. `git commit -m "feat: minha mudanca"`
5. `git push -u origin feature/minha-mudanca`
6. Abrir Pull Request no GitHub

### 2.3 Convencoes basicas

- Commits curtos e objetivos.
- Uma mudanca logica por commit.
- Descrever o motivo da alteracao na mensagem.

---

## 3. Colaboracao: fork e pull request

### Video complementar (Fork e PR)

![type:video](https://www.youtube.com/embed/OlArEishhQg)

### 3.1 O que e fork

- `Fork` cria uma copia do repositorio de outra pessoa na sua conta.
- Ideal para contribuir em repositorios que voce nao controla diretamente.

### 3.2 Fluxo pratico de fork

1. Fazer fork do repositorio do colega no GitHub.
2. Clonar o fork:

```bash
git clone <url-do-seu-fork>
cd <repo>
```

3. Adicionar o repositorio original como `upstream`:

```bash
git remote add upstream <url-repo-original>
git remote -v
```

4. Criar branch e desenvolver:

```bash
git switch -c feature/melhoria-readme
```

5. Commit e push no fork:

```bash
git add .
git commit -m "docs: melhora instrucoes de execucao"
git push -u origin feature/melhoria-readme
```

6. Abrir PR do seu fork para o repositorio original.

---

## 4. Atividade pratica guiada (hoje)

### 4.1 Cenario

Voce vai criar um pequeno programa de leitura de sensores em C++ e Python, versionar em etapas e integrar uma contribuicao de um colega.

### 4.2 Codigo base sugerido

Arquivos prontos para download:

- [sensor.cpp](atividade_base/sensor.cpp)
- [sensor.py](atividade_base/sensor.py)
- [Instrucoes da atividade](atividade_base/instrucoes.txt)

`sensor.cpp`:

```cpp
#include <iostream>
#include <vector>
using namespace std;

double media(const vector<double>& valores) {
    double soma = 0.0;
    for (double v : valores) soma += v;
    return valores.empty() ? 0.0 : soma / valores.size();
}

int main() {
    vector<double> leituras = {21.1, 22.4, 23.0, 21.8};
    cout << "Media C++: " << media(leituras) << endl;
    return 0;
}
```

`sensor.py`:

```python
def media(valores):
    if not valores:
        return 0.0
    return sum(valores) / len(valores)

if __name__ == "__main__":
    leituras = [21.1, 22.4, 23.0, 21.8]
    print(f"Media Python: {media(leituras):.2f}")
```

### 4.3 Etapas de versionamento

1. Commit 1: adicionar arquivos base (`sensor.cpp`, `sensor.py`).
2. Commit 2: incluir validacao para lista vazia e ajuste de mensagem de saida.
3. Commit 3: criar `README.md` com instrucoes de compilacao/execucao.
4. Criar branch `feature/log-saida` e adicionar log de data/hora no resultado.
5. Abrir PR para `main` e pedir revisao de um colega.

### 4.4 Etapa de colaboracao entre colegas

1. Fazer fork do repositorio do colega.
2. Propor uma melhoria pequena (ex.: padronizar nomes, melhorar README).
3. Abrir PR no repositorio original do colega.
4. Colega revisa, comenta e integra.

### 4.5 Entregaveis

- Link do repositorio no GitHub.
- Historico com commits separados por etapa.
- 1 PR no proprio repositorio e 1 PR em fork/colega.

## Mini-caso pratico

Uma equipe precisa ajustar rapidamente um programa de monitoramento. Cada membro faz uma parte, versiona em branch separada e integra por PR apos revisao tecnica.

## Perguntas de revisao rapida

1. Qual a diferenca pratica entre trabalhar por branch e por fork?
2. Por que dividir o trabalho em varios commits pequenos?
3. Qual informacao minima um PR deve conter para facilitar revisao?

## Fontes de referencia

- https://git-scm.com/book/en/v2
- https://docs.github.com/en/get-started/start-your-journey/hello-world
- https://docs.github.com/en/pull-requests/collaborating-with-pull-requests
- https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/working-with-forks
- https://code.visualstudio.com/docs
- https://code.visualstudio.com/docs/languages/cpp
- https://code.visualstudio.com/docs/python/python-tutorial
