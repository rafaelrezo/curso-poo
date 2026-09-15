#include <iostream>
#include <map>
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

struct Medicao {
    double valor;
    std::string unidade;
};

class Catalogo {
    std::map<IdSensor, Medicao> itens_;
public:
    bool inserir(const IdSensor& id, const Medicao& item) {
        return itens_.emplace(id, item).second;
    }
    const Medicao* buscar(const IdSensor& id) const {
        auto it = itens_.find(id);
        return it == itens_.end() ? nullptr : &it->second;
    }
    std::size_t quantidade() const { return itens_.size(); }
};

int main() {
    Catalogo catalogo;
    std::cout << std::boolalpha;
    std::cout << "Primeira: " << catalogo.inserir(IdSensor{"LT-101"}, {12, "%"}) << '\n';
    std::cout << "Duplicada: " << catalogo.inserir(IdSensor{"LT-101"}, {99, "%"}) << '\n';
    const auto* item = catalogo.buscar(IdSensor{"LT-101"});
    if (item) std::cout << "Preservada: " << item->valor << ' ' << item->unidade << '\n';
    std::cout << "Ausente: " << (catalogo.buscar(IdSensor{"LT-999"}) == nullptr) << '\n';
    std::cout << "Quantidade: " << catalogo.quantidade() << '\n';
}
