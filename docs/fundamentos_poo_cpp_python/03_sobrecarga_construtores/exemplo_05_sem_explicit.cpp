#include <iostream>
#include <stdexcept>
#include <string>

class SensorNivel {
    std::string tag_;
public:
    // Sem explicit: permite converter uma std::string em SensorNivel.
    SensorNivel(std::string tag) : tag_(tag) {
        if (tag.empty()) throw std::invalid_argument("tag vazia");
    }

    const std::string& tag() const { return tag_; }
};

void imprimir(SensorNivel sensor) {
    std::cout << sensor.tag() << '\n';
}

int main() {
    std::string nome = "LT-101";
    imprimir(nome); // a funcao espera SensorNivel, mas recebe uma string
}
