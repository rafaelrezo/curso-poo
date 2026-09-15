#include <iostream>

class SensorNivel {
    double valor_;
public:
    explicit SensorNivel(double valor) : valor_(valor) {}
    double valor() const { return valor_; }
    void atualizar(double valor) { valor_ = valor; }
};

class PainelFixo {
    const SensorNivel* sensor_;
public:
    explicit PainelFixo(const SensorNivel& sensor) : sensor_(&sensor) {}
    double leitura() const { return sensor_->valor(); }
    void conectar(const SensorNivel& sensor) { sensor_ = &sensor; }
};

int main() {
    SensorNivel a{10};
    SensorNivel b{70};
    {
        PainelFixo p{a}; // p.sensor_ guarda &a.
        PainelFixo q{a}; // q.sensor_ tambem guarda &a.
        a.atualizar(20);
        std::cout << "Compartilhado: " << p.leitura() << ' ' << q.leitura() << '\n';
        p.conectar(b); // Apenas p.sensor_ passa a guardar &b.
        std::cout << "Apos troca: " << p.leitura() << ' ' << q.leitura() << '\n';
    }
    std::cout << "Sem paineis: " << a.valor() << ' ' << b.valor() << '\n';
}
