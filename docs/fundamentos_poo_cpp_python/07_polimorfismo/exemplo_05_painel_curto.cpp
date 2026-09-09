#include <iostream>
#include <string>

class Sensor {
public:
    virtual ~Sensor() = default;
    virtual double valor() const = 0;
    virtual std::string unidade() const = 0;
    virtual bool emAlerta() const = 0;
};

class SensorNivel : public Sensor {
public:
    double valor() const override { return 15; }
    std::string unidade() const override { return "%"; }
    bool emAlerta() const override { return valor() < 20; }
};

class SensorTemperatura : public Sensor {
public:
    double valor() const override { return 15; }
    std::string unidade() const override { return "C"; }
    bool emAlerta() const override { return valor() > 45; }
};

void mostrar(const Sensor& sensor) {
    std::cout << sensor.valor() << ' ' << sensor.unidade() << " | ";
    if (sensor.emAlerta()) std::cout << "ALERTA\n";
    else std::cout << "OK\n";
}

int main() {
    SensorNivel nivel;
    SensorTemperatura temperatura;
    mostrar(nivel);
    mostrar(temperatura);
}
