#include <iostream>
#include <vector>
using namespace std;

int pesquisaLinear(const vector<int>& vetor, int alvo) {
    for (int i = 0; i < (int)vetor.size(); i++) {
        if (vetor[i] == alvo) {
            return i;
        }
    }
    return -1;
}

int main() {
    vector<int> numeros = {4, 9, 2, 7, 5};

    cout << pesquisaLinear(numeros, 7) << endl;  // imprime 3
    cout << pesquisaLinear(numeros, 8) << endl;  // imprime -1
    return 0;
}