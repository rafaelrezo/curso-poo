#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

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
    // Igualdade de domínio: compara a tag, não o endereço dos objetos.
    bool operator==(const IdSensor& outro) const {
        return valor_ == outro.valor_;
    }
};

int main() {
    // A sequência conserva as três entradas, inclusive a tag repetida.
    std::vector<IdSensor> ids{
        IdSensor{"LT-101"},
        IdSensor{"LT-101"},
        IdSensor{"LT-102"}
    };
    std::cout << "Quantidade: " << ids.size() << '\n';
    // const auto& permite ler cada elemento sem copiá-lo nem modificá-lo.
    for (const auto& id : ids) {
        std::cout << id.valor() << '\n';
    }
}
