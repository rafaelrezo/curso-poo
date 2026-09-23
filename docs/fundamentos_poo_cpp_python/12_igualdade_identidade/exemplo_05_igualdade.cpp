#include <iostream>
#include <stdexcept>
#include <string>

class IdSensor {
    std::string valor_;
public:
    explicit IdSensor(std::string valor) : valor_(valor) {
        // Um identificador válido precisa conter uma tag.
        if (valor.empty()) {
            throw std::invalid_argument("tag vazia");
        }
    }
    const std::string& valor() const { return valor_; }
    // Define o significado de == entre dois objetos IdSensor.
    // Em a == b, este método é chamado em a e recebe b como outro.
    bool operator==(const IdSensor& outro) const {
        // valor_ pertence a a; outro.valor_ pertence a b.
        // Aqui, == compara duas strings, não chama novamente este método.
        return valor_ == outro.valor_;
    }
};

int main() {
    IdSensor a{"LT-101"};
    IdSensor b{"LT-101"};  // Outra instância, com o mesmo valor de identificação.
    const IdSensor& alias = a;  // Referência a a: não cria uma cópia.
    std::cout << std::boolalpha;
    // & obtém o endereço; comparar endereços verifica a identidade aqui.
    std::cout << "Alias: " << (&alias == &a) << '\n';
    std::cout << "Mesma instancia: " << (&a == &b) << '\n';
    // Neste exemplo, a == b equivale à chamada a.operator==(b).
    std::cout << "Mesmo identificador: " << (a == b) << '\n';
}
