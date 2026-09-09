#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

class SensorNivel {
    std::string tag_;
    double valor_;

public:
    explicit SensorNivel(std::string tag) : SensorNivel(tag, 50.0) {}

    SensorNivel(std::string tag, double valor) : tag_(tag), valor_(valor) {
        if (tag.empty() || !std::isfinite(valor) || valor < 0 || valor > 100) {
            throw std::invalid_argument("tag vazia ou nivel fora de 0..100");
        }
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
