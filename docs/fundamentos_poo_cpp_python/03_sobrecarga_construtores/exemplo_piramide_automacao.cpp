#include <iostream>
#include <string>

using namespace std;

class SensorAnalogico {
private:
    string tag;
    string unidade;
    double minimo;
    double maximo;
    double valorAtual;

    static double ajustarValorParaFaixa(double valor, double faixaMinima, double faixaMaxima) {
        if (valor < faixaMinima) {
            return faixaMinima;
        }

        if (valor > faixaMaxima) {
            return faixaMaxima;
        }

        return valor;
    }

public:
    explicit SensorAnalogico(string novaTag)
        : SensorAnalogico(novaTag, "desconhecida", 0.0, 100.0, 0.0) {}

    SensorAnalogico(string novaTag, string novaUnidade, double novoMinimo, double novoMaximo)
        : SensorAnalogico(novaTag, novaUnidade, novoMinimo, novoMaximo, novoMinimo) {}

    SensorAnalogico(string novaTag, string novaUnidade, double novoMinimo, double novoMaximo, double novoValor)
        : tag(novaTag), unidade(novaUnidade), minimo(novoMinimo), maximo(novoMaximo), valorAtual(novoValor) {
        if (minimo >= maximo) {
            minimo = 0.0;
            maximo = 100.0;
        }

        valorAtual = ajustarValorParaFaixa(valorAtual, minimo, maximo);
    }

    void exibirResumo() const {
        cout << "[Sensor] " << tag
             << " | faixa: " << minimo << " a " << maximo
             << " " << unidade
             << " | valor atual: " << valorAtual << " " << unidade << "\n";
    }
};

class ValvulaControle {
private:
    string tag;
    double abertura;

public:
    ValvulaControle(string novaTag)
        : tag(novaTag), abertura(0.0) {}

    ValvulaControle(string novaTag, double aberturaInicial)
        : tag(novaTag), abertura(aberturaInicial) {}

    void exibirResumo() const {
        cout << "[Valvula] " << tag
             << " | abertura inicial: " << abertura << "%\n";
    }
};

class ControladorPID {
private:
    string tag;
    double kp;
    double ki;
    double kd;

public:
    ControladorPID(string novaTag)
        : ControladorPID(novaTag, 1.0, 0.0, 0.0) {}

    ControladorPID(string novaTag, double ganhoP, double ganhoI, double ganhoD)
        : tag(novaTag), kp(ganhoP), ki(ganhoI), kd(ganhoD) {}

    void exibirResumo() const {
        cout << "[Controlador] " << tag
             << " | Kp=" << kp
             << " Ki=" << ki
             << " Kd=" << kd << "\n";
    }
};

int main() {
    SensorAnalogico sensorDescoberto("LT-101");
    SensorAnalogico sensorEngenharia("LT-101", "%", 0.0, 100.0);
    SensorAnalogico sensorConfigurado("TT-201", "C", 0.0, 150.0, 72.5);

    ValvulaControle valvulaPadrao("XV-101");
    ValvulaControle valvulaOperando("FV-301", 45.0);

    ControladorPID controladorRapido("LIC-101");
    ControladorPID controladorCompleto("TIC-201", 2.0, 0.5, 0.1);

    cout << "Gemeos digitais criados para diferentes niveis da piramide de automacao:\n";
    sensorDescoberto.exibirResumo();
    sensorEngenharia.exibirResumo();
    sensorConfigurado.exibirResumo();
    valvulaPadrao.exibirResumo();
    valvulaOperando.exibirResumo();
    controladorRapido.exibirResumo();
    controladorCompleto.exibirResumo();

    return 0;
}
