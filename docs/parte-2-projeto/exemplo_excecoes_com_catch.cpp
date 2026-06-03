#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

class SensorNivel {
private:
    string tag;
    double valorAtual;
    double minimo;
    double maximo;

public:
    SensorNivel(string tag)
        : tag(tag),
          valorAtual(0.0),
          minimo(0.0),
          maximo(100.0) {}

    void atualizarValor(double novoValor) {
        if (novoValor < minimo || novoValor > maximo) {
            throw out_of_range("leitura fora da faixa do sensor");
        }

        valorAtual = novoValor;
    }

    void imprimir() const {
        cout << tag << " = " << valorAtual << "%" << endl;
    }

    string getTag() const {
        return tag;
    }
};

int main() {
    SensorNivel nivel("LT-999");

    cout << "Iniciando ciclo de leitura" << endl;

    try {
        nivel.atualizarValor(-5.0);
        nivel.imprimir();
    } catch (const exception& erro) {
        cout << nivel.getTag()
             << " | falha | "
             << erro.what()
             << endl;
    }

    cout << "Fim do ciclo de leitura" << endl;

    return 0;
}
