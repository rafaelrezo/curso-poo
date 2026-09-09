#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

class SensorNivel {
    double valor_;
public:
    explicit SensorNivel(double valor) : valor_(valor) {
        if (!std::isfinite(valor) || valor < 0 || valor > 100) {
            throw std::invalid_argument("nivel fora da faixa");
        }
    }
    double valor() const { return valor_; }
    std::string unidade() const { return "%"; }
    bool atualizar(double valor) {
        if (!std::isfinite(valor) || valor < 0 || valor > 100) return false;
        valor_ = valor;
        return true;
    }
};

class IFonteLeitura {
public:
    virtual ~IFonteLeitura() = default;
    virtual double valor() const = 0;
    virtual std::string unidade() const = 0;
};

class FonteNivel : public IFonteLeitura {
    const SensorNivel& sensor_;
public:
    explicit FonteNivel(const SensorNivel& sensor) : sensor_(sensor) {}
    double valor() const override { return sensor_.valor(); }
    std::string unidade() const override { return sensor_.unidade(); }
};

double lerFonte(const IFonteLeitura& fonte) {
    return fonte.valor();
}

int main() {
    SensorNivel sensor{10};
    FonteNivel fonte{sensor};
    std::cout << lerFonte(fonte) << ' ' << fonte.unidade() << '\n';
    sensor.atualizar(20);
    std::cout << lerFonte(fonte) << ' ' << fonte.unidade() << '\n';
    // IFonteLeitura incompleta; // experimento: tente instanciar o contrato
}
