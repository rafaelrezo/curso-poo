#include <iostream>
#include <string>

using namespace std;

class AtivoIndustrial {
private:
    string tag;
    bool bloqueado;

protected:
    string area;

    void redefinirArea(string novaArea) {
        area = novaArea;
    }

public:
    AtivoIndustrial(string novaTag, string novaArea)
        : tag(novaTag), bloqueado(false), area(novaArea) {}

    string getTag() const {
        return tag;
    }

    string getArea() const {
        return area;
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

    void exibirCabecalho(const string& tipo) const {
        cout << "[" << tipo << "] " << tag
             << " | area: " << area
             << " | bloqueado: " << (bloqueado ? "sim" : "nao");
    }
};

class SensorPressao : public AtivoIndustrial {
private:
    double valorAtual;
    double limiteAlarme;
    double offsetCalibracao;

public:
    SensorPressao(string novaTag, string novaArea, double novoLimiteAlarme)
        : AtivoIndustrial(novaTag, novaArea),
          valorAtual(0.0),
          limiteAlarme(novoLimiteAlarme),
          offsetCalibracao(0.0) {}

    void atualizarValor(double novoValor) {
        if (estaBloqueado()) {
            return;
        }

        if (novoValor < 0.0) {
            novoValor = 0.0;
        }

        valorAtual = novoValor;
    }

    void aplicarOffsetCalibracao(double novoOffset) {
        offsetCalibracao = novoOffset;
    }

    bool emAlarme() const {
        return valorAtual > limiteAlarme;
    }

    void moverParaArea(string novaArea) {
        redefinirArea(novaArea);
    }

    void exibirResumo() const {
        exibirCabecalho("SensorPressao");
        cout << " | valor: " << valorAtual << " bar"
             << " | limite: " << limiteAlarme << " bar"
             << " | alarme: " << (emAlarme() ? "sim" : "nao") << "\n";
    }

    friend void emitirRelatorioCalibracao(const SensorPressao& sensor);
};

void emitirRelatorioCalibracao(const SensorPressao& sensor) {
    cout << "[Relatorio de calibracao] " << sensor.getTag()
         << " | valor interno: " << sensor.valorAtual
         << " | offset interno: " << sensor.offsetCalibracao
         << " | limite interno: " << sensor.limiteAlarme << "\n";
}

class BombaRecalque : public AtivoIndustrial {
private:
    double frequenciaHz;

public:
    BombaRecalque(string novaTag, string novaArea)
        : AtivoIndustrial(novaTag, novaArea), frequenciaHz(0.0) {}

    void ajustarFrequencia(double novaFrequencia) {
        if (estaBloqueado()) {
            return;
        }

        if (novaFrequencia < 0.0) {
            novaFrequencia = 0.0;
        }

        if (novaFrequencia > 60.0) {
            novaFrequencia = 60.0;
        }

        frequenciaHz = novaFrequencia;
    }

    void exibirResumo() const {
        exibirCabecalho("BombaRecalque");
        cout << " | frequencia: " << frequenciaHz << " Hz\n";
    }
};

int main() {
    SensorPressao sensorRecalque("PT-301", "Campo", 6.0);
    BombaRecalque bombaPrincipal("P-301", "Casa de bombas");

    sensorRecalque.atualizarValor(6.8);
    sensorRecalque.aplicarOffsetCalibracao(-0.15);
    sensorRecalque.moverParaArea("Utilidades");

    bombaPrincipal.ajustarFrequencia(48.0);
    bombaPrincipal.bloquear();
    bombaPrincipal.ajustarFrequencia(55.0);

    cout << "Resumo dos ativos da estacao:\n";
    sensorRecalque.exibirResumo();
    bombaPrincipal.exibirResumo();

    cout << "\nConsulta tecnica controlada:\n";
    emitirRelatorioCalibracao(sensorRecalque);

    return 0;
}
