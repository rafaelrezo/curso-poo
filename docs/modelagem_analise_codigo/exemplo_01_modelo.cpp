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

class SensorNivel {
    double valor_;
public:
    explicit SensorNivel(double valor) : valor_(valor) {}
    double valor() const { return valor_; }
};

int main() {
    SensorNivel sensor{12};
    Catalogo catalogo;
    catalogo.inserir(IdSensor{"LT-101"}, {sensor.valor(), "%"});
    std::cout << "Registros: " << catalogo.quantidade() << '\n';
    std::cout << "Sensor: " << sensor.valor() << " %\n";
}
