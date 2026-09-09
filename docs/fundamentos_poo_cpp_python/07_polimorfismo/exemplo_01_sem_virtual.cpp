#include <iostream>

class Sensor {
public:
    const char* unidade() const { return "generica"; }
};
class SensorNivel : public Sensor {
public:
    const char* unidade() const { return "%"; }
};
void imprimir(const Sensor& sensor) {
    std::cout << sensor.unidade() << '\n';
}
int main() {
    SensorNivel nivel;
    imprimir(nivel);
}
