# Aula 8 - Comunicação TCP com JSON por Linha

## Objetivos de aprendizagem

- Entender sockets TCP como canal simples de comunicação entre processos.
- Substituir arquivo JSON por mensagens JSON enviadas por linha.
- Implementar um cliente-servidor simples sem transformar redes no foco da disciplina.

**Tempo estimado:** 1h30.

## Vídeo da aula

![type:video](https://www.youtube.com/embed/ZFDSPSU6JMw)

---

## 1. Contextualização do problema

Até agora, o dispositivo C++ gravou arquivos e o supervisor Python leu esses arquivos. Isso é simples e útil para começar.

Mas sistemas supervisores normalmente recebem dados enquanto o processo está em execução. Para simular essa ideia, usaremos TCP:

```text
Dispositivo C++ -> socket TCP -> Supervisor Python/Streamlit
```

Redes não serão estudadas em profundidade nesta etapa. O objetivo é entender a ideia de cliente, servidor e mensagem.

---

## 2. Conceitos mínimos

| Conceito | Explicação |
|---|---|
| Socket | ponto de comunicação entre programas |
| Servidor | programa que escuta conexoes |
| Cliente | programa que inicia a conexao |
| Porta | número que identifica o serviço no computador |
| TCP | protocolo orientado a conexao, com entrega ordenada de bytes |
| JSON por linha | cada mensagem JSON termina com `\n` |

### Por que JSON por linha?

TCP entrega um fluxo de bytes, não "objetos JSON". Ao terminar cada mensagem com quebra de linha, o receptor sabe onde uma mensagem acaba.

```text
{"tag":"LT-101","valor":58.4,"unidade":"%","timestamp":"2026-03-25T10:30:00-03:00","status":"operando"}\n
```

---

## 3. Servidor Python simples

```python
import json
import socket

HOST = "127.0.0.1"
PORTA = 5050

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as servidor:
    servidor.bind((HOST, PORTA))
    servidor.listen(1)
    print(f"Aguardando conexao em {HOST}:{PORTA}")

    conexao, endereco = servidor.accept()
    with conexao:
        print(f"Conectado por {endereco}")
        arquivo = conexao.makefile("r", encoding="utf-8")
        for linha in arquivo:
            leitura = json.loads(linha)
            print(leitura["tag"], leitura["valor"], leitura["unidade"])
```

Esse exemplo roda no terminal. O Streamlit pode ler de arquivo gerado pelo servidor ou usar uma camada intermediaria simples.

---

## 4. Cliente do dispositivo

Em C++, a biblioteca padrão não define uma API única de sockets. Por isso, nesta etapa, o mais importante é manter o contrato JSON por linha e isolar a comunicação em uma pequena classe. Assim, a implementação do transporte pode ser adaptada ao ambiente usado sem alterar sensores, regras e comandos.

```cpp
#include <string>
using namespace std;

class CanalTelemetria {
public:
    void enviarLinhaJson(const string& json) {
        // Envie a string por TCP na implementação escolhida.
        // A mensagem deve terminar com '\n'.
    }
};
```

### Caminho recomendado

1. Primeiro, faca o dispositivo gerar JSON Lines em arquivo.
2. Depois, teste o servidor Python recebendo mensagens por TCP.
3. Em seguida, substitua o destino do dispositivo: em vez de gravar em arquivo, envie a mesma linha JSON pelo canal TCP.
4. Se o ambiente ainda não estiver pronto para TCP em C++, mantenha o arquivo JSON Lines e documente essa decisão no README.

---

## 5. Aplicação no projeto

O projeto deve manter o contrato JSON, mas mudar o transporte:

| Antes | Depois |
|---|---|
| C++ grava `leitura.json` | C++ envia JSON por TCP |
| Python le arquivo | Python recebe linha do socket |
| Integração por disco | Integração por conexao local |

O contrato e o conteúdo da mensagem continuam sendo mais importantes que o transporte.

---

## 6. Tratamento de erros

O supervisor deve lidar com:

- porta ocupada;
- cliente desconectado;
- JSON incompleto;
- campo obrigatório ausente;
- valor fora do tipo esperado.

```python
try:
    leitura = json.loads(linha)
    validar_leitura(leitura)
except json.JSONDecodeError:
    print("Mensagem recebida não é JSON válido")
except ValueError as erro:
    print(f"Contrato invalido: {erro}")
```

---

## 7. Exercícios práticos

1. Rodar o servidor Python simples.
2. Enviar uma mensagem manual usando um cliente de teste ou script Python.
3. Adaptar o dispositivo C++ para enviar uma leitura por TCP.
4. Enviar uma leitura por linha.
5. Registrar no README como executar o servidor e o cliente.

---

## 8. Checklist de entrega

- [ ] O contrato JSON foi mantido.
- [ ] Cada mensagem JSON termina com `\n`.
- [ ] O servidor Python recebe pelo menos uma leitura.
- [ ] O cliente C++ envia pelo menos uma leitura.
- [ ] Erros de JSON são tratados sem encerrar o supervisor de forma inesperada.
- [ ] O README documenta a forma escolhida de transporte: arquivo JSON Lines ou TCP.

---

## 9. Perguntas de revisão rápida

1. Por que TCP entrega bytes e não objetos JSON prontos?
2. Qual problema a quebra de linha resolve?
3. O que deve permanecer igual quando trocamos arquivo por socket?

---

## 10. Desafios opcionais

- Enviar uma leitura por segundo durante 30 segundos.
- Salvar no CSV as mensagens recebidas por TCP.
- Criar um modo de fallback: se TCP falhar, usar arquivo JSON.

---

## Fontes de referência

- [Python Docs - socket](https://docs.python.org/3/library/socket.html)
- [Python Docs - json](https://docs.python.org/3/library/json.html)
- [Python Docs - socketserver](https://docs.python.org/3/library/socketserver.html)
