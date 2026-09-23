#include <iostream>
#include <map>
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
    // O map usa esta ordem textual para localizar e distinguir as chaves.
    bool operator<(const IdSensor& outro) const {
        return valor_ < outro.valor_;
    }
    // Igualdade de domínio: compara a tag, não o endereço dos objetos.
    bool operator==(const IdSensor& outro) const {
        return valor_ == outro.valor_;
    }
};

struct Medicao {
    double valor;
    std::string unidade;
};

class Catalogo {
    // Cada chave identifica uma medição; a leitura não faz parte da chave.
    std::map<IdSensor, Medicao> itens_;
public:
    bool inserir(const IdSensor& id, const Medicao& item) {
        // emplace preserva o registro existente quando a chave se repete.
        const auto resultado = itens_.emplace(id, item);
        return resultado.second;  // true apenas quando uma nova entrada foi criada.
    }
    const Medicao* buscar(const IdSensor& id) const {
        auto it = itens_.find(id);
        // end() sinaliza ausência; não podemos acessar um item nessa posição.
        if (it == itens_.end()) {
            return nullptr;
        }
        // second é a medição; retornamos seu endereço, sem transferir posse.
        return &it->second;
    }
    std::size_t quantidade() const { return itens_.size(); }
};

int main() {
    Catalogo catalogo;
    std::cout << std::boolalpha;
    std::cout << "Primeira: " << catalogo.inserir(IdSensor{"LT-101"}, {12, "%"}) << '\n';
    // A mesma tag com outra leitura continua sendo uma duplicata.
    std::cout << "Duplicada: " << catalogo.inserir(IdSensor{"LT-101"}, {99, "%"}) << '\n';
    // Uma nova instância de IdSensor localiza a entrada pela tag.
    const auto* item = catalogo.buscar(IdSensor{"LT-101"});
    // Só acessamos a medição depois de confirmar que o ponteiro não é nulo.
    if (item != nullptr) {
        std::cout << "Preservada: " << item->valor << ' ' << item->unidade << '\n';
    }
    std::cout << "Ausente: " << (catalogo.buscar(IdSensor{"LT-999"}) == nullptr) << '\n';
    std::cout << "Quantidade: " << catalogo.quantidade() << '\n';
}
