#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

class SensorNivel {
    std::string tag_;
    double valor_;

    void validar() const {
        if (tag_.empty() || !std::isfinite(valor_) || valor_ < 0 || valor_ > 100) {
            throw std::invalid_argument("tag vazia ou nivel fora de 0..100");
        }
    }

public:
    explicit SensorNivel(std::string tag)
        : tag_(tag), valor_(50.0) {
        validar();
    }

    SensorNivel(std::string tag, double valor)
        : tag_(tag), valor_(valor) {
        validar();
    }

    const std::string& tag() const { return tag_; }
    double valor() const { return valor_; }
};

int main() {
    SensorNivel usual{"LT-101"};              // um argumento
    SensorNivel informado{"LT-102", 42.5};    // dois argumentos
    SensorNivel zero{"LT-104", 0.0};          // zero foi informado

    std::cout << usual.tag() << ": " << usual.valor() << " %\n";
    std::cout << informado.tag() << ": " << informado.valor() << " %\n";
    std::cout << zero.tag() << ": " << zero.valor() << " %\n";
}
