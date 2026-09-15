#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

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
    std::vector<IdSensor> ids{IdSensor{"LT-101"}, IdSensor{"LT-101"}, IdSensor{"LT-102"}};
    std::cout << "Quantidade: " << ids.size() << '\n';
    for (const auto& id : ids) std::cout << id.valor() << '\n';
}
