#include <iostream>
#include <cmath>
#include <stdexcept>
#include <string>
#include <utility>
class Sensor {
    std::string tag_;
protected:
    explicit Sensor(std::string tag) : tag_(std::move(tag)) {
        if (tag_.empty()) throw std::invalid_argument("tag vazia");
    }
public:
    const std::string& tag() const { return tag_; }
};
class SensorNivel : public Sensor {
    double valor_;
public:
    explicit SensorNivel(std::string tag, double valor = 50)
        : Sensor(tag), valor_(valor) {
        if (!std::isfinite(valor) || valor < 0 || valor > 100)
            throw std::invalid_argument("nivel fora da faixa");
    }
    double valor() const { return valor_; }
    std::string unidade() const { return "%"; }
    bool atualizar(double valor) {
        if (!std::isfinite(valor) || valor < 0 || valor > 100) return false;
        valor_ = valor;
        return true;
    }
};

int main() {
    SensorNivel nivel{"LT-101", 50};
    std::cout << nivel.tag() << ": " << nivel.valor() << ' ' << nivel.unidade() << '\n';
    nivel.atualizar(20);
    std::cout << nivel.tag() << ": " << nivel.valor() << ' ' << nivel.unidade() << '\n';
}
