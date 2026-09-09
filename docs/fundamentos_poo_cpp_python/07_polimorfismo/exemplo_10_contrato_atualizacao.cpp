#include <cmath>
#include <iostream>

class SensorNivel {
    double valor_ = 50;
public:
    double valor() const { return valor_; }

    bool atualizar(double leitura) {
        if (!std::isfinite(leitura) || leitura < 0 || leitura > 100) return false;
        valor_ = leitura;
        return true;
    }
};

int main() {
    SensorNivel nivel;
    const bool aceita = nivel.atualizar(15);
    std::cout << std::boolalpha << aceita << " | " << nivel.valor() << '\n';
    const bool segundaAceita = nivel.atualizar(120);
    std::cout << segundaAceita << " | " << nivel.valor() << '\n';
}
