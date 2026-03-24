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
    // TODO: criar o construtor usando a base AtuadorLinha.

    // TODO: implementar aplicarReferenciaPercentual() convertendo
    // 0 a 100% para 0 a 60 Hz.

    // TODO: implementar exibirResumo() mostrando tag, area e frequenciaHz.
};

class ValvulaDosagem : public AtuadorLinha {
private:
    double aberturaPercentual;

public:
    // TODO: criar o construtor usando a base AtuadorLinha.

    // TODO: implementar aplicarReferenciaPercentual() limitando
    // a faixa entre 0 e 100%.

    // TODO: implementar exibirResumo() mostrando tag, area e aberturaPercentual.
};

int main() {
    // TODO: criar um MotorEsteira e uma ValvulaDosagem.

    // TODO: armazenar os dois em um vector<AtuadorLinha*>.

    // TODO: aplicar a mesma referencia percentual aos dois atuadores.

    // TODO: exibir os resumos usando o vetor polimorfico.

    return 0;
}
