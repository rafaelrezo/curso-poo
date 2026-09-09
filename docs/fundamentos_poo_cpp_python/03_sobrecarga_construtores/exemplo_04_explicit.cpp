#include <iostream>
#include <stdexcept>
#include <string>

class SensorNivel {
    std::string tag_;
    double valor_;

public:
    explicit SensorNivel(std::string tag)
        : tag_(tag), valor_(50.0) {
        if (tag.empty()) {
            throw std::invalid_argument("tag vazia");
        }
    }

    const std::string& tag() const { return tag_; }
    double valor() const { return valor_; }
};

void exibir(const SensorNivel& sensor) {
    std::cout << sensor.tag() << ": " << sensor.valor() << " %\n";
}

int main() {
    std::string tag = "LT-101";  // texto que identifica o equipamento
    SensorNivel sensor{tag};    // pedido explicito de criacao do objeto

    exibir(sensor);            // a funcao recebe um sensor
    // exibir(tag);            // experimento: tente passar somente o texto
}
