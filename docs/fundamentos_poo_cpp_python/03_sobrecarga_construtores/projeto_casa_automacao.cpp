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
    SensorNivel(string novaTag)
        : tag(novaTag), minimo(0.0), maximo(100.0), valorAtual(0.0) {}

    SensorNivel(string novaTag, double novoMinimo, double novoMaximo, double novoValor)
        : tag(novaTag), minimo(novoMinimo), maximo(novoMaximo), valorAtual(novoValor) {}

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
    Bomba(string novaTag)
        : tag(novaTag), velocidade(0.0) {}

    Bomba(string novaTag, double velocidadeInicial)
        : tag(novaTag), velocidade(velocidadeInicial) {}

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
    ControladorTanque(string novaTag)
        : tag(novaTag), setpoint(50.0), kp(1.0) {}

    ControladorTanque(string novaTag, double novoSetpoint, double novoKp)
        : tag(novaTag), setpoint(novoSetpoint), kp(novoKp) {}

    void exibirResumo() const {
        cout << "[ControladorTanque] " << tag
             << " | SP=" << setpoint
             << " | Kp=" << kp << "\n";
    }
};

int main() {
    SensorNivel sn1("LT-401");
    SensorNivel sn2("LT-402", 0.0, 5000.0, 2780.0);

    Bomba b1("P-101");
    Bomba b2("P-102", 65.0);

    ControladorTanque c1("LIC-401");
    ControladorTanque c2("LIC-402", 3200.0, 1.8);

    sn1.exibirResumo();
    sn2.exibirResumo();
    b1.exibirResumo();
    b2.exibirResumo();
    c1.exibirResumo();
    c2.exibirResumo();

    return 0;
}
