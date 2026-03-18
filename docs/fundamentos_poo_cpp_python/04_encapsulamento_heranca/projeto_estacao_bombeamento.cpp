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
    SensorNivel(string novaTag, string novaArea, double novoLimiteAlto)
        : AtivoCampo(novaTag, novaArea), nivelAtual(0.0), limiteAlto(novoLimiteAlto) {}

    void atualizarNivel(double novoNivel) {
        if (estaBloqueado()) {
            return;
        }

        if (novoNivel < 0.0) {
            novoNivel = 0.0;
        }

        nivelAtual = novoNivel;
    }

    void exibirResumo() const {
        cout << "[SensorNivel] " << getTag()
             << " | area: " << area
             << " | nivel: " << nivelAtual
             << " | limite alto: " << limiteAlto << "\n";
    }

    friend void inspecionarSensor(const SensorNivel& sensor);
};

void inspecionarSensor(const SensorNivel& sensor) {
    cout << "[Inspecao] " << sensor.getTag()
         << " | nivel interno: " << sensor.nivelAtual
         << " | limite interno: " << sensor.limiteAlto << "\n";
}

class BombaDosadora : public AtivoCampo {
private:
    double vazaoPercentual;

public:
    BombaDosadora(string novaTag, string novaArea)
        : AtivoCampo(novaTag, novaArea), vazaoPercentual(0.0) {}

    void ajustarVazao(double novaVazao) {
        if (estaBloqueado()) {
            return;
        }

        if (novaVazao < 0.0) {
            novaVazao = 0.0;
        }

        if (novaVazao > 100.0) {
            novaVazao = 100.0;
        }

        vazaoPercentual = novaVazao;
    }

    void moverParaArea(string novaArea) {
        redefinirArea(novaArea);
    }

    void exibirResumo() const {
        cout << "[BombaDosadora] " << getTag()
             << " | area: " << area
             << " | vazao: " << vazaoPercentual << "%\n";
    }
};

int main() {
    SensorNivel sensor("LT-501", "Tanque de preparo", 85.0);
    BombaDosadora bomba("P-501", "Skid de dosagem");

    sensor.atualizarNivel(62.5);
    bomba.ajustarVazao(47.0);
    bomba.moverParaArea("Utilidades");

    sensor.exibirResumo();
    bomba.exibirResumo();
    inspecionarSensor(sensor);

    return 0;
}
