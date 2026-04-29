#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

using namespace std;

string formatar_utc(chrono::system_clock::time_point instante) {
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

int main() {
    auto agora = chrono::system_clock::now();
    auto agora_segundos = chrono::time_point_cast<chrono::seconds>(agora);
    auto epoch_segundos = agora_segundos.time_since_epoch().count();

    cout << "Agora em UTC: " << formatar_utc(agora) << "\n";
    cout << "Timestamp Unix em segundos: " << epoch_segundos << "\n";

    auto proxima_leitura = agora + chrono::minutes(15);
    cout << "Proxima leitura: " << formatar_utc(proxima_leitura) << "\n";

    auto inicio_medicao = chrono::steady_clock::now();
    this_thread::sleep_for(chrono::milliseconds(250));
    auto fim_medicao = chrono::steady_clock::now();

    auto duracao_ms = chrono::duration_cast<chrono::milliseconds>(
        fim_medicao - inicio_medicao
    );

    cout << "Duracao medida: " << duracao_ms.count() << " ms\n";
    return 0;
}
