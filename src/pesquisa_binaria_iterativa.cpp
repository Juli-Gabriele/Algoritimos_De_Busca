
#include <iostream>
using namespace std;

int pesquisaBinariaIterativa(int vetor[], int tamanho, int valor) {
    int inicio = 0;
    int fim = tamanho - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;

        if (vetor[meio] == valor) {
            return meio; // Encontrou o valor
        }
        else if (vetor[meio] < valor) {
            inicio = meio + 1; // Busca na metade direita
        }
        else {
            fim = meio - 1; // Busca na metade esquerda
        }
    }

    return -1; // Valor não encontrado
}

int main() {
    int vetor[] = {2, 4, 6, 8, 10, 12, 14};
    int tamanho = 7;
    int valor;

    cout << "Digite o valor que deseja buscar: ";
    cin >> valor;

    int resultado = pesquisaBinariaIterativa(vetor, tamanho, valor);

    if (resultado != -1) {
        cout << "Valor encontrado no indice: " << resultado << endl;
    }
    else {
        cout << "Valor nao encontrado!" << endl;
    }

    return 0;
}
