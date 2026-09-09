#include <cassert>
#include <iostream>

class IFonteLeitura {
public:
    virtual ~IFonteLeitura() = default;
    virtual double valor() const = 0;
};

class FonteTeste : public IFonteLeitura {
public:
    double valor() const override { return 10; }
};

class PoliticaAlarme {
    double limite_;
public:
    explicit PoliticaAlarme(double limite) : limite_(limite) {}
    bool aciona(double valor) const { return valor > limite_; }
};

class ControladorConsulta {
    const IFonteLeitura& fonte_;
    const PoliticaAlarme& politica_;
public:
    ControladorConsulta(const IFonteLeitura& fonte, const PoliticaAlarme& politica)
        : fonte_(fonte), politica_(politica) {}
    bool avaliar() const { return politica_.aciona(fonte_.valor()); }
};

int main() {
    FonteTeste fonte;
    PoliticaAlarme politica{10};
    ControladorConsulta controlador{fonte, politica};
    const bool alerta = controlador.avaliar();
    assert(!alerta);
    std::cout << "OK: igualdade ao limite nao alerta\n";
}
