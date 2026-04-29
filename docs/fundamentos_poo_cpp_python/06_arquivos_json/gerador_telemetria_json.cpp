#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <random>
#include <sstream>
#include <string>

using namespace std;

string escapar_json(const string& texto) {
    ostringstream saida;

    for (char caractere : texto) {
        switch (caractere) {
            case '"':
                saida << "\\\"";
                break;
            case '\\':
                saida << "\\\\";
                break;
            case '\n':
                saida << "\\n";
                break;
            case '\r':
                saida << "\\r";
                break;
            case '\t':
                saida << "\\t";
                break;
            default:
                saida << caractere;
                break;
        }
    }

    return saida.str();
}

string timestamp_utc(chrono::system_clock::time_point instante) {
    time_t tempo = chrono::system_clock::to_time_t(instante);
    tm calendario{};

#ifdef _WIN32
    gmtime_s(&calendario, &tempo);
#else
    gmtime_r(&tempo, &calendario);
#endif

    ostringstream saida;
    saida << put_time(&calendario, "%Y-%m-%dT%H:%M:%SZ");
    return saida.str();
}

double sortear_uniforme(mt19937& gerador, double minimo, double maximo) {
    uniform_real_distribution<double> distribuicao(minimo, maximo);
    return distribuicao(gerador);
}

int main() {
    const string caminho = "telemetria.json";
    const int total_leituras = 24;

    ofstream arquivo(caminho);
    if (!arquivo) {
        cerr << "Erro: nao foi possivel criar " << caminho << "\n";
        return 1;
    }

    random_device semente;
    mt19937 gerador(semente());
    auto inicio = chrono::system_clock::now();

    arquivo.imbue(locale::classic());
    arquivo << fixed << setprecision(2);

    arquivo << "{\n";
    arquivo << "  \"fonte\": \"" << escapar_json("simulador_cpp") << "\",\n";
    arquivo << "  \"descricao\": \"" << escapar_json("telemetria simulada de uma estacao") << "\",\n";
    arquivo << "  \"gerado_em\": \"" << timestamp_utc(inicio) << "\",\n";
    arquivo << "  \"leituras\": [\n";

    for (int i = 0; i < total_leituras; ++i) {
        auto instante = inicio + chrono::minutes(i);
        double cpu = sortear_uniforme(gerador, 18.0, 82.0);
        double memoria = sortear_uniforme(gerador, 35.0, 76.0);
        double temperatura = sortear_uniforme(gerador, 23.0, 41.0);

        arquivo << "    {\n";
        arquivo << "      \"timestamp\": \"" << timestamp_utc(instante) << "\",\n";
        arquivo << "      \"cpu_percent\": " << cpu << ",\n";
        arquivo << "      \"memoria_percent\": " << memoria << ",\n";
        arquivo << "      \"temperatura_c\": " << temperatura << "\n";
        arquivo << "    }";

        if (i + 1 < total_leituras) {
            arquivo << ",";
        }
        arquivo << "\n";
    }

    arquivo << "  ]\n";
    arquivo << "}\n";

    cout << "Arquivo gerado: " << caminho << "\n";
    cout << "Leituras gravadas: " << total_leituras << "\n";
    return 0;
}
