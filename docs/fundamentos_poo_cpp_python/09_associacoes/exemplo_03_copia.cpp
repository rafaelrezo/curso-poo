#include <iostream>

class SensorNivel {
    double valor_;
public:
    explicit SensorNivel(double valor) : valor_(valor) {}
    double valor() const { return valor_; }
    void atualizar(double valor) { valor_ = valor; }
};

class PainelFixo {
    double leitura_;
public:
    explicit PainelFixo(const SensorNivel& sensor) : leitura_(sensor.valor()) {}
    double leitura() const { return leitura_; }
};

int main() {
    SensorNivel sensor{10};
    PainelFixo painel{sensor};
    sensor.atualizar(20);
    std::cout << "Sensor: " << sensor.valor() << '\n';
    std::cout << "Painel: " << painel.leitura() << '\n';
}
