#include <iostream>
#include <vector>
using namespace std;

double media(const vector<double>& valores) {
    double soma = 0.0;
    for (double v : valores) soma += v;
    return valores.empty() ? 0.0 : soma / valores.size();
}

int main() {
    vector<double> leituras = {21.1, 22.4, 23.0, 21.8};
    cout << "Media C++: " << media(leituras) << endl;
    return 0;
}
