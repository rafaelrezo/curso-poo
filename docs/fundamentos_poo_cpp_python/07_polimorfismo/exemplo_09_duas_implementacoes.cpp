#include <iostream>

class Sensor {
public:
    virtual ~Sensor() = default;
    virtual const char* unidade() const = 0;
};

class SensorNivel : public Sensor {
public:
    const char* unidade() const override { return "%"; }
};

class SensorTemperatura : public Sensor {
public:
    const char* unidade() const override { return "C"; }
};

void imprimir(const Sensor& sensor) {
    std::cout << sensor.unidade() << '\n';
}

int main() {
    SensorNivel nivel;
    SensorTemperatura temperatura;
    imprimir(nivel);
    imprimir(temperatura);
}
