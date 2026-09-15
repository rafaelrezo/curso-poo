#include <iostream>
#include <stdexcept>

class FonteConstante {
public:
    double valor() const { return 42.5; }
};

class FalhaLeitura : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

double adquirir(const FonteConstante& fonte, bool disponivel) {
    if (!disponivel) throw FalhaLeitura("fonte indisponivel");
    return fonte.valor();
}

int main() {
    FonteConstante fonte;
    for (bool disponivel : {true, false, true}) {
        try {
            const double valor = adquirir(fonte, disponivel);
            std::cout << "Leitura: " << valor << '\n';
        } catch (const FalhaLeitura& erro) {
            std::cout << "Sem leitura: " << erro.what() << '\n';
        }
    }
}
