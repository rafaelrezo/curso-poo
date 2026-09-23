#include <map>
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
    // O map usa esta ordem textual para localizar e distinguir as chaves.
    bool operator<(const IdSensor& outro) const {
        return valor_ < outro.valor_;
    }
    // Igualdade de domínio: compara a tag, não o endereço dos objetos.
    bool operator==(const IdSensor& outro) const {
        return valor_ == outro.valor_;
    }
};

// T varia o tipo do item; a chave e as regras do catálogo permanecem.
template<typename T>
class Catalogo {
    std::map<IdSensor, T> itens_;
public:
    bool inserir(const IdSensor& id, const T& item) {
        // emplace preserva o registro existente quando a chave se repete.
        const auto resultado = itens_.emplace(id, item);
        return resultado.second;  // true apenas quando uma nova entrada foi criada.
    }
    const T* buscar(const IdSensor& id) const {
        auto it = itens_.find(id);
        // end() sinaliza ausência; não podemos acessar um item nessa posição.
        if (it == itens_.end()) {
            return nullptr;
        }
        // second é o item de tipo T; retornamos seu endereço, sem transferir posse.
        return &it->second;
    }
};

int main() {
    Catalogo<std::string> catalogo;
    const bool primeira = catalogo.inserir(IdSensor{"LT-101"}, "Bancada A");
    const bool repetida = catalogo.inserir(IdSensor{"LT-101"}, "Bancada B");
    const auto* nome = catalogo.buscar(IdSensor{"LT-101"});
    std::cout << std::boolalpha << primeira << ' ' << repetida << '\n';
    if (nome != nullptr) {
        std::cout << *nome << '\n';
    }
}
