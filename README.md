<h1 align="center"> Linguagem C-- </h1>

<h4 align="center"> 
    :construction:  Projeto em construção  :construction:
</h4>

:computer: <b> Repositório dedicado à disciplina de Compiladores do CEFET-MG Leopoldina, no semestre 2025/1. </b>

# Recursos utilizados:
 - IDE: Visual Studio Code
 - Linguagem: C++

# Como compilar:
## Linux:
Digite no terminal:
 - g++ -o analisador analisador.cpp
 - ./analisador arquivo.txt

## Windows:
Digite no terminal:
 - g++ analisador.cpp
 - .\a arquivo.txt

# Testes de Erros:
 - Erros detectados são destacados em vermelho no terminal, junto com a linha e a coluna
 - Detecta caracteres inválidos na linguagem
 - Detecta operadores compostos incompletos ou inválidos (exceto !)
 - Detecta números inteiros com zeros à esquerda inválidos, como por exemplo 009
 - Detecta parte fracionária faltando em números decimais (float), como por exemplo 53.
 - Detecta zeros à esquerda a mais do que o permitido em float, como por exemplo 00.5
