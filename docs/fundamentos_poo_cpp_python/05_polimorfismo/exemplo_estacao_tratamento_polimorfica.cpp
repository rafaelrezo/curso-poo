#include <iostream>
#include <string>
#include <vector>

using namespace std;

class AtuadorMalha {
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
    AtuadorMalha(string novaTag, string novaArea)
        : tag(novaTag), area(novaArea) {}

    virtual ~AtuadorMalha() = default;

    string getTag() const {
        return tag;
    }

    string getArea() const {
        return area;
    }

    virtual void aplicarReferenciaPercentual(double percentual) = 0;
    virtual void exibirResumo() const = 0;
};

class InversorBomba : public AtuadorMalha {
private:
    double frequenciaHz;

public:
    InversorBomba(string novaTag, string novaArea)
        : AtuadorMalha(novaTag, novaArea), frequenciaHz(0.0) {}

    void aplicarReferenciaPercentual(double percentual) override {
        percentual = limitarPercentual(percentual);
        frequenciaHz = (percentual / 100.0) * 60.0;
    }

    void exibirResumo() const override {
        cout << "[InversorBomba] " << getTag()
             << " | area: " << getArea()
             << " | frequencia: " << frequenciaHz << " Hz\n";
    }
};

class ValvulaProporcional : public AtuadorMalha {
private:
    double aberturaPercentual;

public:
    ValvulaProporcional(string novaTag, string novaArea)
        : AtuadorMalha(novaTag, novaArea), aberturaPercentual(0.0) {}

    void aplicarReferenciaPercentual(double percentual) override {
        aberturaPercentual = limitarPercentual(percentual);
    }

    void exibirResumo() const override {
        cout << "[ValvulaProporcional] " << getTag()
             << " | area: " << getArea()
             << " | abertura: " << aberturaPercentual << "%\n";
    }
};

class DosadorQuimico : public AtuadorMalha {
private:
    double vazaoLh;

public:
    DosadorQuimico(string novaTag, string novaArea)
        : AtuadorMalha(novaTag, novaArea), vazaoLh(0.0) {}

    void aplicarReferenciaPercentual(double percentual) override {
        percentual = limitarPercentual(percentual);
        vazaoLh = (percentual / 100.0) * 120.0;
    }

    void exibirResumo() const override {
        cout << "[DosadorQuimico] " << getTag()
             << " | area: " << getArea()
             << " | vazao: " << vazaoLh << " L/h\n";
    }
};

int main() {
    InversorBomba bombaRecalque("P-201", "Recalque");
    ValvulaProporcional valvulaCoagulante("FV-310", "Mistura rapida");
    DosadorQuimico dosadorCloro("DP-101", "Cloracao");

    vector<AtuadorMalha*> atuadores = {
        &bombaRecalque,
        &valvulaCoagulante,
        &dosadorCloro
    };

    const double referenciaSupervisoria = 62.5;

    cout << "Aplicando referencia unica do supervisorio: "
         << referenciaSupervisoria << "%\n";

    for (AtuadorMalha* atuador : atuadores) {
        atuador->aplicarReferenciaPercentual(referenciaSupervisoria);
    }

    cout << "Resposta individual de cada equipamento:\n";
    for (const AtuadorMalha* atuador : atuadores) {
        atuador->exibirResumo();
    }

    return 0;
}
