#include <iostream>
#include <stdexcept>

class FonteConstante {
public:
    double valor() const { return 42.5; }
};

class FalhaLeitura : public std::runtime_error { // Tipo padrao de falha, em <stdexcept>.
public:
    using std::runtime_error::runtime_error; // Reutiliza o construtor com mensagem.
};

class Sessao {
    int& abertas_; // Referencia ao contador do main; nao cria uma copia.
public:
    // explicit impede conversao automatica de int para Sessao.
    // : abertas_(abertas) liga a referencia ao contador antes do corpo.
    explicit Sessao(int& abertas) : abertas_(abertas) { ++abertas_; }
    ~Sessao() { --abertas_; } // Ao sair do escopo, libera a sessao.
    Sessao(const Sessao&) = delete; // Proibe criar outra Sessao copiando esta.
    Sessao& operator=(const Sessao&) = delete; // Proibe atribuir uma Sessao a outra.
};

double adquirir(const FonteConstante& fonte, bool disponivel, int& abertas) {
    Sessao sessao{abertas}; // Abre agora; o destrutor fecha no retorno ou no throw.
    std::cout << "Durante aquisicao | sessoes: " << abertas << '\n';
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
    } catch (const FalhaLeitura&) { // Trata apenas a falha prevista.
        return {false, 0}; // O bool marca ausencia; zero nao e uma leitura.
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
    // auto deduz ResultadoLeitura, o tipo devolvido por executarCiclo.
    auto primeiro = executarCiclo(fonte, true, abertas);
    mostrar(primeiro, abertas);
    auto segundo = executarCiclo(fonte, false, abertas);
    mostrar(segundo, abertas);
    auto terceiro = executarCiclo(fonte, true, abertas);
    mostrar(terceiro, abertas);
}
