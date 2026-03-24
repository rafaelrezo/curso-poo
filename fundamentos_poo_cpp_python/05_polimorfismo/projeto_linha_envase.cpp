#include <iostream>
#include <string>
#include <vector>

using namespace std;

class AtuadorLinha {
private:
    string tag;
    string area;

protected:
    double limitarPercentual(double percentual) const {
        if (percentual < 0.0) {
            return 0.0;
        }

        if (percentual > 100.0) {
            return 100.0;
        }

        return percentual;
    }

public:
    AtuadorLinha(string novaTag, string novaArea)
        : tag(novaTag), area(novaArea) {}

    virtual ~AtuadorLinha() = default;

    string getTag() const {
        return tag;
    }

    string getArea() const {
        return area;
    }

    virtual void aplicarReferenciaPercentual(double percentual) = 0;
    virtual void exibirResumo() const = 0;
};

class MotorEsteira : public AtuadorLinha {
private:
    double frequenciaHz;

public:
    MotorEsteira(string novaTag, string novaArea)
        : AtuadorLinha(novaTag, novaArea), frequenciaHz(0.0) {}

    void aplicarReferenciaPercentual(double percentual) override {
        percentual = limitarPercentual(percentual);
        frequenciaHz = (percentual / 100.0) * 60.0;
    }

    void exibirResumo() const override {
        cout << "[MotorEsteira] " << getTag()
             << " | area: " << getArea()
             << " | frequencia: " << frequenciaHz << " Hz\n";
    }
};

class ValvulaDosagem : public AtuadorLinha {
private:
    double aberturaPercentual;

public:
    ValvulaDosagem(string novaTag, string novaArea)
        : AtuadorLinha(novaTag, novaArea), aberturaPercentual(0.0) {}

    void aplicarReferenciaPercentual(double percentual) override {
        aberturaPercentual = limitarPercentual(percentual);
    }

    void exibirResumo() const override {
        cout << "[ValvulaDosagem] " << getTag()
             << " | area: " << getArea()
             << " | abertura: " << aberturaPercentual << "%\n";
    }
};

int main() {
    MotorEsteira esteiraEntrada("M-101", "Entrada de garrafas");
    ValvulaDosagem valvulaXarope("FV-201", "Dosagem");

    vector<AtuadorLinha*> atuadores = {&esteiraEntrada, &valvulaXarope};
    const double referencia = 55.0;

    for (AtuadorLinha* atuador : atuadores) {
        atuador->aplicarReferenciaPercentual(referencia);
    }

    cout << "Linha de envase com referencia unica de " << referencia << "%\n";
    for (const AtuadorLinha* atuador : atuadores) {
        atuador->exibirResumo();
    }

    return 0;
}
