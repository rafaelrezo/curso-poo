#include <iostream>
#include <string>

using namespace std;

class AtivoCampo {
private:
    string tag;
    bool bloqueado;

protected:
    string area;

    void redefinirArea(string novaArea) {
        area = novaArea;
    }

public:
    AtivoCampo(string novaTag, string novaArea)
        : tag(novaTag), bloqueado(false), area(novaArea) {}

    string getTag() const {
        return tag;
    }

    bool estaBloqueado() const {
        return bloqueado;
    }

    void bloquear() {
        bloqueado = true;
    }

    void liberar() {
        bloqueado = false;
    }
};

class SensorNivel : public AtivoCampo {
private:
    double nivelAtual;
    double limiteAlto;

public:
    // TODO: criar o construtor usando a base AtivoCampo.

    // TODO: implementar atualizarNivel() com duas regras:
    // 1. nao alterar o nivel quando o ativo estiver bloqueado;
    // 2. nunca aceitar nivel negativo.

    // TODO: implementar exibirResumo() mostrando tag, area, nivelAtual e limiteAlto.

    // TODO: declarar uma friend function para inspecao tecnica do sensor.
};

// TODO: definir a friend function de SensorNivel acessando diretamente
// nivelAtual e limiteAlto.

class BombaDosadora : public AtivoCampo {
private:
    double vazaoPercentual;

public:
    // TODO: criar o construtor usando a base AtivoCampo.

    // TODO: implementar ajustarVazao() respeitando:
    // 1. bloqueio do ativo;
    // 2. faixa entre 0 e 100.

    // TODO: implementar moverParaArea() reaproveitando o membro protegido da base.

    // TODO: implementar exibirResumo() mostrando tag, area e vazaoPercentual.
};

int main() {
    // TODO: criar um SensorNivel e uma BombaDosadora.
    // TODO: atualizar seus estados.
    // TODO: imprimir um resumo e chamar a inspecao tecnica do sensor.

    return 0;
}
