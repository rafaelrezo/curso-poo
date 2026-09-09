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

class Sessao {
    int& abertas_;
public:
    explicit Sessao(int& abertas) : abertas_(abertas) { ++abertas_; }
    ~Sessao() { --abertas_; }
    Sessao(const Sessao&) = delete;
    Sessao& operator=(const Sessao&) = delete;
};

double adquirir(const FonteConstante& fonte, bool disponivel, int& abertas) {
    Sessao sessao{abertas};
    if (!disponivel) throw FalhaLeitura("fonte indisponivel");
    return fonte.valor();
}

double lerServico(const FonteConstante& fonte, bool disponivel, int& abertas) {
    return adquirir(fonte, disponivel, abertas);
}

struct ResultadoLeitura {
    bool sucesso;
    double valor;
};

ResultadoLeitura executarCiclo(const FonteConstante& fonte, bool disponivel,
                              int& abertas) {
    try {
        return {true, lerServico(fonte, disponivel, abertas)};
    } catch (const FalhaLeitura&) {
        return {false, 0};
    }
}

void mostrar(const ResultadoLeitura& resultado, int abertas) {
    if (resultado.sucesso) std::cout << "Leitura: " << resultado.valor;
    else std::cout << "Sem leitura";
    std::cout << " | sessoes: " << abertas << '\n';
}

int main() {
    FonteConstante fonte;
    int abertas = 0;
    auto primeiro = executarCiclo(fonte, true, abertas);
    mostrar(primeiro, abertas);
    auto segundo = executarCiclo(fonte, false, abertas);
    mostrar(segundo, abertas);
    auto terceiro = executarCiclo(fonte, true, abertas);
    mostrar(terceiro, abertas);
}
