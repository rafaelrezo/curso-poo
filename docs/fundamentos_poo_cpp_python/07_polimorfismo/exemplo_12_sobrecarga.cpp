#include <iostream>

void imprimir(int valor) {
    std::cout << "inteiro: " << valor << '\n';
}

void imprimir(double valor) {
    std::cout << "real: " << valor << '\n';
}

int main() {
    imprimir(15);
    imprimir(15.5);
}
