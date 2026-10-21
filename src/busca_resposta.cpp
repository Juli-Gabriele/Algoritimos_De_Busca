
#include <iostream>
using namespace std;

// Menor inteiro x tal que x * x >= numero (raiz quadrada arredondada para cima).
int raizQuadradaTeto(int numero) {
    // long long porque meio * meio estoura int para números grandes.
    long long inicio = 0;
    long long fim = numero;
    long long resposta = 0;

    while (inicio <= fim) {
        long long meio = inicio + (fim - inicio) / 2;

        if (meio * meio >= numero) {
            resposta = meio;
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
    }

    return static_cast<int>(resposta);
}

int main() {
    int numero;

    cout << "Digite um numero: ";
    cin >> numero;

    if (numero < 0) {
        cout << "Digite um numero nao negativo." << endl;
        return 1;
    }

    cout << "Resposta: " << raizQuadradaTeto(numero) << endl;

    return 0;
}
