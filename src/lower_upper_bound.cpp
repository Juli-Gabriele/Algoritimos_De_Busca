
#include <iostream>
using namespace std;

// Primeira posição com valor maior ou igual ao procurado
int lowerBound(int vetor[], int tamanho, int valor) {
    int inicio = 0;
    int fim = tamanho;

    while (inicio < fim) {
        int meio = inicio + (fim - inicio) / 2;

        if (vetor[meio] < valor) {
            inicio = meio + 1;
        } else {
            fim = meio;
        }
    }

    return inicio;
}

// Primeira posição com valor maior que o procurado
int upperBound(int vetor[], int tamanho, int valor) {
    int inicio = 0;
    int fim = tamanho;

    while (inicio < fim) {
        int meio = inicio + (fim - inicio) / 2;

        if (vetor[meio] <= valor) {
            inicio = meio + 1;
        } else {
            fim = meio;
        }
    }

    return inicio;
}

int main() {
    int vetor[] = {1, 2, 2, 2, 4, 6, 8};
    int tamanho = 7;
    int valor = 2;

    int inferior = lowerBound(vetor, tamanho, valor);
    int superior = upperBound(vetor, tamanho, valor);

    cout << "Lower Bound: " << inferior << endl;
    cout << "Upper Bound: " << superior << endl;
    cout << "Quantidade de ocorrencias: "
         << superior - inferior << endl;

    return 0;
}
