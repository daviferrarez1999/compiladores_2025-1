// Compiladores 2025.1 - Analisador Lexico

// No linux:
// Compilar com: g++ -o analisador analisador.cpp
// Executar com: ./analisador arquivo.txt

// No windows:
// Compilar com: g++ analisador.cpp
// Executar com: .\a arquivo.txt

#include <iostream>     // Para entrada e saida (cout, cerr)
#include <fstream>      // Para manipulacao de arquivos (ifstream)
#include <cctype>       // Para funcoes de verificacao de caracteres (isalpha, isdigit)
#include <string>       // Para manipulacao de strings
#include <vector>       // Para lista de palavras reservadas
#include <algorithm>    // Para uso em intervalos de elementos (transform)

using namespace std;    // Para evitar usar std:: a cada chamada

// Lista de palavras reservadas da linguagem
const vector<string> palavras_reservadas = {
    "if", "else", "while", "break", "print", "readln", "return",
    "int", "float", "char", "bool", "true", "false"
};

// Funcao que verifica se uma palavra eh reservada
bool is_palavra_reservada(const string& palavra){
    for (const auto& reservada : palavras_reservadas){
        if (palavra == reservada)
            return true;
    }
    return false;
}

// Funcao que transforma uma string para maiusculas (estilo Linux/portavel)
string strupr_linux(string str){
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// Funcao que realiza a analise lexica
void analisar(ifstream& arquivo){
    char c;         // Caractere atual
    string token;   // Token em construcao
    int linha = 1;
    int coluna = 0;

    while (arquivo.get(c)){
        if (c == '\n'){
            linha++;
            coluna = 0;
            continue;
        }

        // Ignora espacos, tabs e quebras de linha
        if (isspace(c)) {
            coluna++;
            continue;
        }

        // Identificadores ou palavras reservadas (comecam com letra ou '_')
        if(isalpha(c) || c == '_'){
            token = c;

            while (arquivo.get(c) && (isalnum(c) || c == '_')){
                token += c;
            }

            if (arquivo) arquivo.unget(); // Devolve o ultimo caractere lido se nao for parte do token

            if (is_palavra_reservada(token)){
                cout << strupr_linux(token) << "\n";
            } else {
                cout << "ID." << token << "\n";
            }
        }

        // Numeros inteiros
        else if (isdigit(c)){
            token = c;

            while (arquivo.get(c) && isdigit(c)){
                token += c;
            }

            if (arquivo) arquivo.unget(); // Devolve o caractere que nao faz parte do numero

            cout << "NUM." << token << "\n";
        }

        // Simbolos e operadores
        else{
            switch (c){
                case '(': cout << "LPARENT\n"; break; 
                case ')': cout << "RPARENT\n"; break; 
                case '{': cout << "LBRACE\n"; break; 
                case '}': cout << "RBRACE\n"; break; 
                case ';': cout << "SEMICOLON\n"; break; 
                case ',': cout << "COMMA\n"; break;
                case '%': cout << "MOD\n"; break; // Modulo da divisao
                case '=':
                    if (arquivo.get(c) && c == '='){
                        cout << "EQ\n";  // EQ para "equal"
                    } else {
                        if (arquivo) arquivo.unget(); // Devolve o caractere se nao for '='
                        cout << "ASSIGN\n";
                    }
                    break;

                case '+': 
                    if (arquivo.get(c) && c == '+'){
                        cout << "INCREMENT\n";  // Incremento
                    } else {
                        if (arquivo) arquivo.unget(); // Devolve o caractere se não tiver mais um '+'
                        cout << "PLUS\n";
                    }
                break;
                case '-': 
                    if (arquivo.get(c) && c == '-'){
                        cout << "DECREMENT\n";  // Decremento
                    } else {
                        if (arquivo) arquivo.unget(); // Devolve o caractere se não tiver mais um '-'
                        cout << "MINUS\n";
                    }
                    break;
                case '*': cout << "MULT\n"; break;
                case '/': cout << "DIV\n"; break;
                case '<': 
                    if (arquivo.get(c) && c == '='){
                        cout << "LEQ\n";  // LEQ = Less Than or Equal
                    } else {
                        if (arquivo) arquivo.unget(); // Devolve o caractere lido se não for '='
                        cout << "LT\n";  // LT = Less Than
                    }
                    break;
                case '>':
                    if (arquivo.get(c) && c == '='){
                        cout << "GEQ\n";  // GE = Greater or Equal
                    } else {
                        if (arquivo) arquivo.unget(); // Devolve o caractere lido se não for '='
                        cout << "GT\n";  // GT = Greater Than
                    }
                    break;
                    case '|': 
                    if (arquivo.get(c) && c == '|'){
                        cout << "OR\n";  
                    }
                    break;
                case '!':
                    if (arquivo.get(c) && c == '='){
                        cout << "DIFF\n";  // DIFF  = Different
                    } else {
                        if (arquivo) arquivo.unget(); // Devolve o caractere lido se não for '='
                        cout << "NEG\n";  // NEG = Negacao logica 
                    }
                    break;
                case '&':
                    if (arquivo.get(c) && c == '&'){
                        cout << "AND\n";  
                    } 
                    break;

                // Caso não seja simbolo conhecido, reporta erro
                default:
                    cerr << "\033[31m"
                        << "ERRO na linha " << linha << ", coluna " << coluna
                        << ": caractere inválido '" << c << "'"
                        << "\033[0m"
                        << "'\n";
                    break;
            }
        }
    }

    // Indica o fim do arquivo
    cout << "EOF\n";
}

// Função principal
int main(int argc, char* argv[]){
    // Verifica se o nome do arquivo foi fornecido
    if(argc < 2){
        cerr << "Uso: " << argv[0] << " <arquivo fonte>\n";
        return 1;
    }

    // Abre o arquivo
    ifstream arquivo(argv[1]);
    if (!arquivo.is_open()){
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Chama o analisador lexico
    analisar(arquivo);

    // Fecha o arquivo
    arquivo.close();

    return 0;
}
