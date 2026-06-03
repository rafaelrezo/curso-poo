#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class Sensor {
private:
    string tag;
    string tipoSensor;
    string unidade;
    double minimo;
    double maximo;
    double valorAtual;

protected:
    void atualizarValor(double novoValor) {
        if (novoValor < minimo || novoValor > maximo) {
            throw out_of_range("leitura fora da faixa permitida");
        }

        valorAtual = novoValor;
    }

public:
    Sensor(string tag, string tipoSensor, string unidade, double minimo, double maximo)
        : tag(tag),
          tipoSensor(tipoSensor),
          unidade(unidade),
          minimo(minimo),
          maximo(maximo),
          valorAtual(0.0) {}

    virtual ~Sensor() = default;

    virtual void simularLeitura() = 0;

    string getTag() const { return tag; }
    string getTipo() const { return tipoSensor; }
    string getUnidade() const { return unidade; }
    double getValorAtual() const { return valorAtual; }

    string gerarJsonOperando() const {
        return "{\"tag\":\"" + tag +
               "\",\"tipo\":\"" + tipoSensor +
               "\",\"valor\":" + to_string(valorAtual) +
               ",\"unidade\":\"" + unidade +
               "\",\"status\":\"operando\"}";
    }

    string gerarJsonFalha(const string& mensagem) const {
        return "{\"tag\":\"" + tag +
               "\",\"tipo\":\"" + tipoSensor +
               "\",\"valor\":null" +
               ",\"unidade\":\"" + unidade +
               "\",\"status\":\"falha\"" +
               ",\"erro\":\"" + mensagem + "\"}";
    }
};

class SensorNivel : public Sensor {
private:
    double leituraSimulada;

public:
    SensorNivel(string tag, double leituraSimulada)
        : Sensor(tag, "nivel", "%", 0.0, 100.0),
          leituraSimulada(leituraSimulada) {}

    void simularLeitura() override {
        atualizarValor(leituraSimulada);
    }
};

class SensorPressao : public Sensor {
private:
    double leituraSimulada;

public:
    SensorPressao(string tag, double leituraSimulada)
        : Sensor(tag, "pressao", "bar", 0.0, 10.0),
          leituraSimulada(leituraSimulada) {}

    void simularLeitura() override {
        atualizarValor(leituraSimulada);
    }
};

int main() {
    vector<unique_ptr<Sensor>> sensores;

    sensores.push_back(make_unique<SensorNivel>("LT-101", 42.0));
    sensores.push_back(make_unique<SensorPressao>("PT-201", 2.7));
    sensores.push_back(make_unique<SensorNivel>("LT-999", -5.0));

    for (const auto& sensor : sensores) {
        try {
            sensor->simularLeitura();
            cout << sensor->gerarJsonOperando() << endl;
        } catch (const exception& erro) {
            cout << sensor->gerarJsonFalha(erro.what()) << endl;
        }
    }

    return 0;
}
