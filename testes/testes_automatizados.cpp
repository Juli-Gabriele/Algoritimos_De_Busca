// Testes das implementações de src/. A STL aparece só aqui, como referência.
#include "busca.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

int totalVerificacoes = 0;
int totalFalhas = 0;

#define VERIFICAR(condicao)                                                         \
    do {                                                                            \
        ++totalVerificacoes;                                                        \
        if (!(condicao)) {                                                          \
            ++totalFalhas;                                                          \
            std::cerr << "FALHOU (linha " << __LINE__ << "): " #condicao "\n";      \
        }                                                                           \
    } while (0)

using busca::NAO_ENCONTRADO;
using Vetor = std::vector<int>;

void secao(const std::string& nome) { std::cout << "- " << nome << '\n'; }

void testarPesquisaLinear() {
    secao("pesquisa linear");
    const Vetor desordenado{42, 7, 19, 3, 88, 25, 61, 14, 50, 33};
    VERIFICAR(busca::pesquisaLinear(desordenado, 25) == 5);
    VERIFICAR(busca::pesquisaLinear(desordenado, 42) == 0);
    VERIFICAR(busca::pesquisaLinear(desordenado, 33) == 9);
    VERIFICAR(busca::pesquisaLinear(desordenado, 99) == NAO_ENCONTRADO);
    VERIFICAR(busca::pesquisaLinear({}, 1) == NAO_ENCONTRADO);
    VERIFICAR(busca::pesquisaLinear({5}, 5) == 0);
    VERIFICAR(busca::pesquisaLinear({5, 5, 5}, 5) == 0);  // primeira ocorrência

    busca::Metricas m;
    busca::pesquisaLinear(desordenado, 25, &m);
    VERIFICAR(m.comparacoes == 6);                        // índices 0..5
    busca::pesquisaLinear(desordenado, 99, &m);
    VERIFICAR(m.comparacoes == desordenado.size());       // pior caso: n
    busca::pesquisaLinear({}, 1, &m);
    VERIFICAR(m.comparacoes == 0);
}

template <typename Funcao>
void testarBinaria(const char* nome, Funcao buscar) {
    secao(nome);
    const Vetor v{2, 5, 8, 12, 16, 23, 38, 56, 72, 91};
    for (std::size_t i = 0; i < v.size(); ++i) {
        VERIFICAR(buscar(v, v[i], nullptr) == static_cast<std::ptrdiff_t>(i));
    }
    for (const int ausente : {-10, 0, 1, 3, 7, 24, 90, 100}) {
        VERIFICAR(buscar(v, ausente, nullptr) == NAO_ENCONTRADO);
    }
    VERIFICAR(buscar(Vetor{}, 5, nullptr) == NAO_ENCONTRADO);   // vazio
    VERIFICAR(buscar(Vetor{42}, 42, nullptr) == 0);              // um elemento
    VERIFICAR(buscar(Vetor{42}, 41, nullptr) == NAO_ENCONTRADO);
    VERIFICAR(buscar(Vetor{42}, 43, nullptr) == NAO_ENCONTRADO);

    // Duplicatas: devolve ALGUMA ocorrência válida.
    const Vetor dup{1, 2, 2, 2, 3};
    const auto r = buscar(dup, 2, nullptr);
    VERIFICAR(r >= 1 && r <= 3 && dup[static_cast<std::size_t>(r)] == 2);

    // Valores extremos de int (verifica comparações, sem overflow).
    const Vetor extremos{std::numeric_limits<int>::min(), -1, 0, 1, std::numeric_limits<int>::max()};
    VERIFICAR(buscar(extremos, std::numeric_limits<int>::min(), nullptr) == 0);
    VERIFICAR(buscar(extremos, std::numeric_limits<int>::max(), nullptr) == 4);

    // Condições de parada: nunca mais que floor(log2 n) + 1 iterações.
    busca::Metricas m;
    buscar(v, 23, &m);
    VERIFICAR(m.iteracoes == 3);     // rastreamento do teste de mesa
    buscar(v, 7, &m);
    VERIFICAR(m.iteracoes == 3);
    buscar(Vetor{}, 7, &m);
    VERIFICAR(m.iteracoes == 0);
    for (const std::size_t n : {1u, 2u, 3u, 10u, 1000u, 100000u}) {
        Vetor grande(n);
        for (std::size_t i = 0; i < n; ++i) grande[i] = static_cast<int>(3 * i);
        const auto limite = static_cast<std::size_t>(std::floor(std::log2(static_cast<double>(n)))) + 1;
        for (const int alvo : {-1, static_cast<int>(3 * n), static_cast<int>(3 * (n / 2)), static_cast<int>(3 * (n / 2) + 1)}) {
            buscar(grande, alvo, &m);
            VERIFICAR(m.iteracoes <= limite);
        }
    }
}

void testarLimites() {
    secao("lower bound / upper bound");
    const Vetor v{1, 2, 2, 2, 4, 6, 8};
    VERIFICAR(busca::lowerBound(v, 2) == 1);
    VERIFICAR(busca::upperBound(v, 2) == 4);
    VERIFICAR(busca::contarOcorrencias(v, 2) == 3);
    VERIFICAR(busca::lowerBound(v, 3) == 4 && busca::upperBound(v, 3) == 4);
    VERIFICAR(busca::lowerBound(v, 0) == 0 && busca::upperBound(v, 0) == 0);
    VERIFICAR(busca::lowerBound(v, 9) == v.size() && busca::upperBound(v, 9) == v.size());
    VERIFICAR(busca::lowerBound(v, 8) == 6 && busca::upperBound(v, 8) == 7);
    VERIFICAR(busca::lowerBound({}, 5) == 0 && busca::upperBound({}, 5) == 0);
    VERIFICAR(busca::contarOcorrencias({7, 7, 7, 7}, 7) == 4);
    VERIFICAR(busca::contarOcorrencias({7, 7, 7, 7}, 6) == 0);
}

// Força bruta de referência: testa todos os limites possíveis em ordem.
long long cargaPorForcaBruta(const Vetor& duracoes, int k) {
    if (duracoes.empty()) return 0;
    long long soma = 0;
    for (const int d : duracoes) soma += d;
    for (long long limite = 0; limite <= soma; ++limite) {
        if (busca::cargaViavel(duracoes, k, limite)) return limite;
    }
    return soma;
}

void testarEspacoResposta() {
    secao("busca no espaço de resposta");
    VERIFICAR(busca::menorCargaMaxima({7, 2, 5, 10, 8}, 2) == 18);
    VERIFICAR(busca::menorCargaMaxima({7, 2, 5, 10, 8}, 1) == 32);   // uma pessoa faz tudo
    VERIFICAR(busca::menorCargaMaxima({7, 2, 5, 10, 8}, 5) == 10);   // cada uma faz um
    VERIFICAR(busca::menorCargaMaxima({7, 2, 5, 10, 8}, 9) == 10);   // sobram trabalhadores
    VERIFICAR(busca::menorCargaMaxima({1, 2, 3, 4, 5}, 2) == 9);
    VERIFICAR(busca::menorCargaMaxima({}, 3) == 0);
    VERIFICAR(busca::menorCargaMaxima({5}, 1) == 5);

    // Monotonicidade: viável(L) implica viável(L + 1).
    const Vetor d{7, 2, 5, 10, 8};
    for (long long limite = 0; limite < 40; ++limite) {
        if (busca::cargaViavel(d, 2, limite)) VERIFICAR(busca::cargaViavel(d, 2, limite + 1));
    }
    VERIFICAR(!busca::cargaViavel(d, 2, 17) && busca::cargaViavel(d, 2, 18));
    VERIFICAR(!busca::cargaViavel(d, 100, 9));  // trabalho de 10 não cabe em limite 9

    // Entradas inválidas.
    bool lancou = false;
    try { busca::menorCargaMaxima(d, 0); } catch (const std::invalid_argument&) { lancou = true; }
    VERIFICAR(lancou);
    lancou = false;
    try { busca::menorCargaMaxima({3, -1}, 2); } catch (const std::invalid_argument&) { lancou = true; }
    VERIFICAR(lancou);

    // Comparação com força bruta em entradas aleatórias.
    std::mt19937 gerador(2026);
    std::uniform_int_distribution<int> tamanho(1, 8), valor(0, 20), trabalhadores(1, 5);
    for (int caso = 0; caso < 300; ++caso) {
        Vetor dur(static_cast<std::size_t>(tamanho(gerador)));
        for (auto& x : dur) x = valor(gerador);
        const int k = trabalhadores(gerador);
        VERIFICAR(busca::menorCargaMaxima(dur, k) == cargaPorForcaBruta(dur, k));
    }
}

void testarContraSTL() {
    secao("comparação com a STL (vetores aleatórios)");
    std::mt19937 gerador(42);
    std::uniform_int_distribution<int> tamanho(0, 40), valor(-15, 15);
    for (int caso = 0; caso < 2000; ++caso) {
        Vetor v(static_cast<std::size_t>(tamanho(gerador)));
        for (auto& x : v) x = valor(gerador);
        std::sort(v.begin(), v.end());
        const int alvo = valor(gerador);

        const auto esperadoLower = static_cast<std::size_t>(std::lower_bound(v.begin(), v.end(), alvo) - v.begin());
        const auto esperadoUpper = static_cast<std::size_t>(std::upper_bound(v.begin(), v.end(), alvo) - v.begin());
        VERIFICAR(busca::lowerBound(v, alvo) == esperadoLower);
        VERIFICAR(busca::upperBound(v, alvo) == esperadoUpper);

        const bool existe = std::binary_search(v.begin(), v.end(), alvo);
        for (const auto r : {busca::pesquisaBinariaIterativa(v, alvo), busca::pesquisaBinariaRecursiva(v, alvo)}) {
            if (existe) VERIFICAR(r >= 0 && v[static_cast<std::size_t>(r)] == alvo);
            else VERIFICAR(r == NAO_ENCONTRADO);
        }
        const auto posLinear = busca::pesquisaLinear(v, alvo);
        const auto refLinear = std::find(v.begin(), v.end(), alvo);
        VERIFICAR(posLinear == (refLinear == v.end() ? NAO_ENCONTRADO : refLinear - v.begin()));
    }
}

}  // namespace

int main() {
    std::cout << "Executando testes...\n";
    testarPesquisaLinear();
    testarBinaria("pesquisa binária iterativa", [](const Vetor& v, int x, busca::Metricas* m) {
        return busca::pesquisaBinariaIterativa(v, x, m);
    });
    testarBinaria("pesquisa binária recursiva", [](const Vetor& v, int x, busca::Metricas* m) {
        return busca::pesquisaBinariaRecursiva(v, x, m);
    });
    testarLimites();
    testarEspacoResposta();
    testarContraSTL();

    std::cout << "\nVerificações: " << totalVerificacoes << " | falhas: " << totalFalhas << '\n'
              << (totalFalhas == 0 ? "TODOS OS TESTES PASSARAM\n" : "HÁ TESTES FALHANDO\n");
    return totalFalhas == 0 ? 0 : 1;
}
