#include <iostream>
#include <string>

using namespace std;

class SensorNivel {
private:
    string tag;
    double minimo;
    double maximo;
    double valorAtual;

public:
    // TODO: criar um construtor minimo que receba apenas a tag.
    // Dica: use uma faixa padrao coerente.

    // TODO: criar um construtor completo com tag, minimo, maximo e valorAtual.

    void exibirResumo() const {
        cout << "[SensorNivel] " << tag
             << " | faixa: " << minimo << " a " << maximo
             << " | valor atual: " << valorAtual << "\n";
    }
};

class Bomba {
private:
    string tag;
    double velocidade;

public:
    // TODO: criar um construtor minimo.

    // TODO: criar um construtor completo com velocidade inicial.

    void exibirResumo() const {
        cout << "[Bomba] " << tag
             << " | velocidade: " << velocidade << "%\n";
    }
};

class ControladorTanque {
private:
    string tag;
    double setpoint;
    double kp;

public:
    // TODO: criar um construtor minimo.

    // TODO: criar um construtor completo com setpoint e kp.

    void exibirResumo() const {
        cout << "[ControladorTanque] " << tag
             << " | SP=" << setpoint
             << " | Kp=" << kp << "\n";
    }
};

int main() {
    // TODO: crie ao menos dois objetos de cada classe usando sobrecarga.
    // Exemplo esperado:
    // SensorNivel sn1("LT-401");
    // SensorNivel sn2("LT-402", 0.0, 5000.0, 2780.0);

    // TODO: chame exibirResumo() para todos os objetos.

    return 0;
}
