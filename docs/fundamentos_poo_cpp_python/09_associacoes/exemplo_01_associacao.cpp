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

class PainelFixo {
    const SensorNivel* sensor_;
public:
    explicit PainelFixo(const SensorNivel& sensor) : sensor_(&sensor) {}
    double leitura() const { return sensor_->valor(); }
};

double consultarAgora(const SensorNivel& sensor) {
    return sensor.valor();
}

int main() {
    SensorNivel sensor{10};
    {
        PainelFixo p{sensor};
        PainelFixo q{sensor};
        std::cout << "Antes: " << p.leitura() << ' ' << q.leitura() << '\n';
        sensor.atualizar(20);
        std::cout << "Depois: " << p.leitura() << ' ' << q.leitura() << '\n';
    } // os paineis terminam aqui; o sensor continua vivo
    std::cout << "Sem paineis: " << consultarAgora(sensor) << '\n';
}
