# Fundamentos Git e IDE

## Objetivos de aprendizagem

- Configurar e usar o VS Code para criar, compilar e executar programas em C++ e Python.
- Aplicar o fluxo prático de versionamento com Git e GitHub.
- Colaborar com colegas usando branch, fork e pull request.

**Tempo estimado:** 8h

## Vídeo da aula

![type:video](https://www.youtube.com/embed/h96Z38oMJZA)

---

## 1. VS Code na prática (antes do Git)

### 1.1 Preparacao mínima

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

As capturas de tela e videos curtos desta aula são recursos de apoio preparados pelo professor.

- Os alunos **não** precisam gerar screenshots para entrega.

Guias com capturas oficiais:

- VS Code (interface): https://code.visualstudio.com/docs/getstarted/userinterface
- C++ no VS Code: https://code.visualstudio.com/docs/languages/cpp
- Python no VS Code: https://code.visualstudio.com/docs/python/python-tutorial
- GitHub Hello World (branch, commit e PR): https://docs.github.com/en/get-started/start-your-journey/hello-world
- Fork e PR no GitHub: https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/working-with-forks

---

## 2. Git e GitHub na prática

### Vídeo da aula (Git)

![type:video](https://www.youtube.com/embed/4hpUALY4Rrk)

### 2.1 Conceitos principais

| Conceito | O que e | Exemplo prático |
|---|---|---|
| Repositorio | Histórico do projeto | pasta do trabalho no GitHub |
| Commit | Registro de uma mudança | `feat: adiciona leitura de sensor` |
| Branch | Linha de desenvolvimento | `feature/ajuste-saída` |
| Push | Enviar commits para remoto | `git push origin main` |
| Pull Request | Proposta de integração | revisão de código entre colegas |

### 2.2 Fluxo essencial

1. `git clone <url-do-repo>`
2. `git switch -c feature/minha-mudanca`
3. `git add .`
4. `git commit -m "feat: minha mudanca"`
5. `git push -u origin feature/minha-mudanca`
6. Abrir Pull Request no GitHub

### 2.3 Convencoes basicas

- Commits curtos e objetivos.
- Uma mudança logica por commit.
- Descrever o motivo da alteração na mensagem.

---

## 3. Colaboracao: fork e pull request

### Vídeo complementar (Fork e PR)

![type:video](https://www.youtube.com/embed/OlArEishhQg)

### 3.1 O que e fork

- `Fork` cria uma cópia do repositório de outra pessoa na sua conta.
- Ideal para contribuir em repositórios que você não controla diretamente.

### 3.2 Fluxo prático de fork

1. Fazer fork do repositório do colega no GitHub.
2. Clonar o fork:

```bash
git clone <url-do-seu-fork>
cd <repo>
```

3. Adicionar o repositório original como `upstream`:

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

6. Abrir PR do seu fork para o repositório original.

---

## 4. Atividade prática guiada (hoje)

### 4.1 Cenário

Você vai criar um pequeno programa de leitura de sensores em C++ e Python, versionar em etapas e integrar uma contribuicao de um colega.

### 4.2 Código base sugerido

Arquivos prontos para download:

- [sensor.cpp](atividade_base/sensor.cpp)
- [sensor.py](atividade_base/sensor.py)
- [Instruções da atividade](atividade_base/instrucoes.txt)

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
2. Commit 2: incluir validação para lista vazia e ajuste de mensagem de saída.
3. Commit 3: criar `README.md` com instruções de compilação/execução.
4. Criar branch `feature/log-saída` e adicionar log de data/hora no resultado.
5. Abrir PR para `main` e pedir revisão de um colega.

### 4.4 Etapa de colaboracao entre colegas

1. Fazer fork do repositório do colega.
2. Propor uma melhoria pequena (ex.: padronizar nomes, melhorar README).
3. Abrir PR no repositório original do colega.
4. Colega revisa, comenta e integra.

### 4.5 Entregaveis

- Link do repositório no GitHub.
- Histórico com commits separados por etapa.
- 1 PR no próprio repositório e 1 PR em fork/colega.

## Mini-caso prático

Uma equipe precisa ajustar rapidamente um programa de monitoramento. Cada membro faz uma parte, versiona em branch separada e integra por PR apos revisão técnica.

## Perguntas de revisão rápida

1. Qual a diferença prática entre trabalhar por branch e por fork?
2. Por que dividir o trabalho em vários commits pequenos?
3. Qual informacao mínima um PR deve conter para facilitar revisão?

## Fontes de referência

- https://git-scm.com/book/en/v2
- https://docs.github.com/en/get-started/start-your-journey/hello-world
- https://docs.github.com/en/pull-requests/collaborating-with-pull-requests
- https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/working-with-forks
- https://code.visualstudio.com/docs
- https://code.visualstudio.com/docs/languages/cpp
- https://code.visualstudio.com/docs/python/python-tutorial

---

## 5. Guia publico para entregas no GitHub Classroom

As atividades práticas da disciplina são entregues em repositórios individuais do GitHub Classroom.

Se você ainda não entendeu como aceitar uma atividade, clonar o repositório, fazer `commit`, `push`, consultar prazo e ler o feedback do professor, use este guia:

- [Como receber e entregar atividades no GitHub Classroom](02_github_classroom_estudantes/index.md)
