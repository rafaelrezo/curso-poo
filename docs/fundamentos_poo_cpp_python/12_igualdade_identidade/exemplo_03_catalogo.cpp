#include <map>
#include <iostream>
#include <stdexcept>
#include <string>

class IdSensor {
    std::string valor_;
public:
    explicit IdSensor(std::string valor) : valor_(valor) {
        if (valor.empty()) throw std::invalid_argument("tag vazia");
    }
    const std::string& valor() const { return valor_; }
    bool operator<(const IdSensor& outro) const { return valor_ < outro.valor_; }
    bool operator==(const IdSensor& outro) const { return valor_ == outro.valor_; }
};

template<typename T>
class Catalogo {
    std::map<IdSensor, T> itens_;
public:
    bool inserir(const IdSensor& id, const T& item) {
        return itens_.emplace(id, item).second;
    }
    const T* buscar(const IdSensor& id) const {
        auto it = itens_.find(id);
        return it == itens_.end() ? nullptr : &it->second;
    }
};

int main() {
    Catalogo<std::string> catalogo;
    const bool primeira = catalogo.inserir(IdSensor{"LT-101"}, "Bancada A");
    const bool repetida = catalogo.inserir(IdSensor{"LT-101"}, "Bancada B");
    const auto* nome = catalogo.buscar(IdSensor{"LT-101"});
    std::cout << std::boolalpha << primeira << ' ' << repetida << '\n';
    if (nome) std::cout << *nome << '\n';
}
