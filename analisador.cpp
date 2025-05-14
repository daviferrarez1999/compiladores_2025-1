// Compiladores 2025.1 - Analisador Léxico

// analisador_lexico.cpp
// Compilar com: g++ -o analisador analisador.cpp
// Executar com: ./analisador arquivo.txt

#include <iostream>     // Para entrada e saída (cout, cerr)
#include <fstream>      // Para manipulação de arquivos (ifstream)
#include <cctype>       // Para funções de verificação de caracteres (isalpha, isdigit, etc.)
#include <string>       // Para manipulação de strings
#include <vector>       // Para lista de palavras reservadas
#include <algorithm>    // Para transform

using namespace std;    // Para evitar usar std:: a cada chamada

// Define o tamanho máximo de um token (mantido por clareza, embora não seja necessário com string)
#define TAM_TOKEN 100

// Lista de palavras reservadas da linguagem
const vector<string> palavras_reservadas = {
    "if", "else", "while", "break", "print", "readln", "return",
    "int", "float", "char", "bool", "true", "false"
};

// Função que verifica se uma palavra é reservada
bool is_palavra_reservada(const string& palavra) {
    for (const auto& reservada : palavras_reservadas) {
        if (palavra == reservada)
            return true;
    }
    return false;
}

// Função que transforma uma string para maiúsculas (estilo Linux/portável)
string strupr_linux(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// Função que realiza a análise léxica
void analisar(ifstream& arquivo) {
    char c;         // Caractere atual
    string token;   // Token em construção

    while (arquivo.get(c)) {
        // Ignora espaços, tabs e quebras de linha
        if (isspace(c)) continue;

        // Identificadores ou palavras reservadas (começam com letra ou '_')
        if (isalpha(c) || c == '_') {
            token = c;

            while (arquivo.get(c) && (isalnum(c) || c == '_')) {
                token += c;
            }

            if (arquivo) arquivo.unget(); // Devolve o último caractere lido se não for parte do token

            if (is_palavra_reservada(token)) {
                cout << strupr_linux(token) << "\n";
            } else {
                cout << "ID." << token << "\n";
            }
        }

        // Números inteiros
        else if (isdigit(c)) {
            token = c;

            while (arquivo.get(c) && isdigit(c)) {
                token += c;
            }

            if (arquivo) arquivo.unget(); // Devolve o caractere que não faz parte do número

            cout << "NUM." << token << "\n";
        }

        // Símbolos e operadores
        else {
            switch (c) {
                case '(': cout << "LPARENT\n"; break;
                case ')': cout << "RPARENT\n"; break;
                case '{': cout << "LBRACE\n"; break;
                case '}': cout << "RBRACE\n"; break;
                case ';': cout << "SEMICOLON\n"; break;
                case ',': cout << "COMMA\n"; break;
                case '=':
                    if (arquivo.get(c) && c == '=') {
                        cout << "EQ\n";  // EQ para "equal", ou use "EQUALS" se preferir
                    } else {
                        if (arquivo) arquivo.unget(); // devolve o caractere se não for '='
                        cout << "ASSIGN\n";
                    }
                    break;

                case '+': 
                    if (arquivo.get(c) && c == '+') {
                        cout << "TWOPLUS\n";  // EQ para "equal", ou use "EQUALS" se preferir
                    } else {
                        if (arquivo) arquivo.unget(); // devolve o caractere se não for '='
                        cout << "PLUS\n";
                    }
                break;
                case '-': 
                    if (arquivo.get(c) && c == '-') {
                        cout << "TWOMINUS\n";  // EQ para "equal", ou use "EQUALS" se preferir
                    } else {
                        if (arquivo) arquivo.unget(); // devolve o caractere se não for '='
                        cout << "MINUS\n";
                    }
                    break;
                case '*': cout << "MULT\n"; break;
                case '/': cout << "DIV\n"; break;
                case '<': 
                    if (arquivo.get(c) && c == '=') {
                        cout << "LEQ\n";  // GE = Greater or Equal
                    } else {
                        if (arquivo) arquivo.unget(); // devolve o caractere lido se não for '='
                        cout << "LT\n";  // GT = Greater Than
                    }
                    break;
                case '>':
                    if (arquivo.get(c) && c == '=') {
                        cout << "GEQ\n";  // GE = Greater or Equal
                    } else {
                        if (arquivo) arquivo.unget(); // devolve o caractere lido se não for '='
                        cout << "GT\n";  // GT = Greater Than
                    }
                    break;


                // Caso não seja símbolo conhecido, reporta erro
                default:
                    cerr << "ERRO: caractere inválido '" << c << "'\n";
            }
        }
    }

    // Indica o fim do arquivo
    cout << "EOF\n";
}

// Função principal
int main(int argc, char* argv[]) {
    // Verifica se o nome do arquivo foi fornecido
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <arquivo fonte>\n";
        return 1;
    }

    // Abre o arquivo
    ifstream arquivo(argv[1]);
    if (!arquivo.is_open()) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Chama o analisador léxico
    analisar(arquivo);

    // Fecha o arquivo
    arquivo.close();

    return 0;
}
