#include <iostream>
#include <stdexcept>
#include <string>

class IdSensor {
    std::string valor_;
public:
    explicit IdSensor(std::string valor) : valor_(valor) {
        if (valor.empty()) throw std::invalid_argument("tag vazia");
    }
    const std::string& valor() const { return valor_; }
    bool operator==(const IdSensor& outro) const { return valor_ == outro.valor_; }
};

int main() {
    IdSensor a{"LT-101"};
    IdSensor b{"LT-101"};
    const IdSensor& alias = a;
    std::cout << std::boolalpha;
    std::cout << "Alias: " << (&alias == &a) << '\n';
    std::cout << "Mesma instancia: " << (&a == &b) << '\n';
    std::cout << "Mesmo identificador: " << (a == b) << '\n';
}
