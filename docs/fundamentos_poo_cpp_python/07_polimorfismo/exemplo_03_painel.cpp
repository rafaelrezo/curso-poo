#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>

class Sensor {
    std::string tag_;
protected:
    explicit Sensor(std::string tag) : tag_(std::move(tag)) {}
public:
    virtual ~Sensor() = default;
    const std::string& tag() const { return tag_; }
    virtual double valor() const = 0;
    virtual std::string unidade() const = 0;
    virtual bool atualizar(double leitura) = 0;
    virtual bool emAlerta() const = 0;
};

#include <cmath>

class SensorNivel : public Sensor {
    double valor_ = 50.0;
public:
    explicit SensorNivel(std::string tag) : Sensor(std::move(tag)) {}
    double valor() const override { return valor_; }
    std::string unidade() const override { return "%"; }
    bool atualizar(double leitura) override {
        if (!std::isfinite(leitura) || leitura < 0 || leitura > 100) return false;
        valor_ = leitura;
        return true;
    }
    bool emAlerta() const override {
        return valor() < 20.0;
    }
};

class SensorTemperatura : public Sensor {
    double valor_ = 25.0;
public:
    explicit SensorTemperatura(std::string tag) : Sensor(std::move(tag)) {}
    double valor() const override { return valor_; }
    std::string unidade() const override { return "C"; }
    bool atualizar(double leitura) override {
        if (!std::isfinite(leitura) || leitura < -40 || leitura > 125) return false;
        valor_ = leitura;
        return true;
    }
    bool emAlerta() const override {
        return valor() > 45.0;
    }
};

std::string linhaPainel(const Sensor& sensor) {
    std::ostringstream linha;
    linha << sensor.tag() << ": "
          << std::fixed << std::setprecision(1) << sensor.valor()
          << ' ' << sensor.unidade() << " | "
          << (sensor.emAlerta() ? "ALERTA" : "OK");
    return linha.str();
}

int main() {
    SensorNivel nivel{"LT-101"};
    SensorTemperatura temperatura{"TT-201"};
    nivel.atualizar(15);
    temperatura.atualizar(15);
    std::cout << linhaPainel(nivel) << '\n';
    std::cout << linhaPainel(temperatura) << '\n';
}
