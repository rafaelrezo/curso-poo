#include <iostream>

class SensorNivel {
    double valor_;
public:
    explicit SensorNivel(double valor) : valor_(valor) {}
    double valor() const { return valor_; }
    void atualizar(double valor) { valor_ = valor; }
};

class PainelFixo {
    const SensorNivel* sensor_; // 1. Guarda o endereco, nao uma copia da leitura.
public:
    explicit PainelFixo(const SensorNivel& sensor) : sensor_(&sensor) {} // 2. Mantem o vinculo.
    double leitura() const { return sensor_->valor(); } // 3. Consulta o estado atual.
};

int main() {
    SensorNivel sensor{10};
    PainelFixo painel{sensor};
    sensor.atualizar(20);
    std::cout << "Sensor: " << sensor.valor() << '\n';
    std::cout << "Painel: " << painel.leitura() << '\n';
}
