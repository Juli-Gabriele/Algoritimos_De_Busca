cmake_minimum_required(VERSION 3.16)
project(seminario_busca_ed2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Cada arquivo de src/ é um programa independente, com o seu próprio main().
add_executable(pesquisa_linear            src/pesquisa_linear.cpp)
add_executable(pesquisa_binaria_iterativa src/pesquisa_binaria_iterativa.cpp)
add_executable(pesquisa_binaria_recursiva src/pesquisa_binaria_recursiva.cpp)
add_executable(lower_upper_bound          src/lower_upper_bound.cpp)
add_executable(busca_resposta             src/busca_resposta.cpp)
