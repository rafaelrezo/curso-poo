#include <iostream>

class SensorNivel {
    double valor_;
public:
    explicit SensorNivel(double valor) : valor_(valor) {}
    double valor() const { return valor_; }
    void atualizar(double valor) { valor_ = valor; }
};

// Contrato: as duas operacoes que a apresentacao pode pedir.
class IFonteLeitura {
public:
    virtual ~IFonteLeitura() = default;
    virtual double valor() const = 0;
    virtual const char* unidade() const = 0;
};

// Implementacao que consulta um sensor externo.
class FonteNivel : public IFonteLeitura {
    const SensorNivel& sensor_;
public:
    explicit FonteNivel(const SensorNivel& sensor) : sensor_(sensor) {}
    double valor() const override { return sensor_.valor(); }
    const char* unidade() const override { return "%"; }
};

// Outra implementacao do mesmo contrato, sem sensor instalado.
class FonteConstante : public IFonteLeitura {
public:
    double valor() const override { return 42.5; }
    const char* unidade() const override { return "%"; }
};

// Cliente: recebe o contrato e consulta o objeto concreto.
void mostrar(const IFonteLeitura& fonte) {
    std::cout << fonte.valor() << ' ' << fonte.unidade() << '\n';
}

int main() {
    SensorNivel sensor{10};
    FonteNivel real{sensor};
    FonteConstante simulada;
    mostrar(real);
    mostrar(simulada);
    sensor.atualizar(20);
    mostrar(real);
}
