
#include <iostream>
using namespace std;

int pesquisaBinariaRecursiva(int vetor[], int inicio, int fim, int valor) {
    if (inicio > fim) {
        return -1; // Valor não encontrado
    }

    int meio = inicio + (fim - inicio) / 2;

    if (vetor[meio] == valor) {
        return meio; // Valor encontrado
    }

    if (valor > vetor[meio]) {
        return pesquisaBinariaRecursiva(vetor, meio + 1, fim, valor);
    }

    return pesquisaBinariaRecursiva(vetor, inicio, meio - 1, valor);
}

int main() {
    int vetor[] = {2, 4, 6, 8, 10, 12, 14};
    int tamanho = 7;
    int valor;

    cout << "Digite o valor que deseja buscar: ";
    cin >> valor;

    int resultado = pesquisaBinariaRecursiva(vetor, 0, tamanho - 1, valor);

    if (resultado != -1) {
        cout << "Valor encontrado no indice: " << resultado << endl;
    } else {
        cout << "Valor nao encontrado!" << endl;
    }

    return 0;
}
