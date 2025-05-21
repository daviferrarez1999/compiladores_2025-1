// Compiladores 2025.1 - Analisador Lexico

// No linux:
// Compilar com: g++ -o analisador analisador.cpp
// Executar com: ./analisador arquivo.txt

// No windows:
// Compilar com: g++ analisador.cpp
// Executar com: .\a arquivo.txt

// Bibliotecas utilizadas
#include <iostream>     // entrada e saida (cout, cerr)
#include <fstream>      // manipulacao de arquivos (ifstream)
#include <cctype>       // funcoes de verificacao de caracteres (isalpha, isdigit)
#include <string>       // manipulacao de strings
#include <vector>       // lista de palavras reservadas
#include <algorithm>    // uso em intervalos de elementos (transform)

using namespace std;    // evita usar std:: a cada chamada

// palavras reservadas da linguagem
const vector<string> palavras_reservadas = {
    "if", "else", "while", "break", "print", "readln", "return",
    "int", "float", "char", "bool", "true", "false"
};

// verifica se uma palavra eh reservada
bool is_palavra_reservada(const string& palavra){
    for (const auto& reservada : palavras_reservadas){
        if (palavra == reservada)
            return true;
    }
    return false;
}

// transforma uma string para maiusculas (estilo Linux/portavel)
string strupr_linux(string str){
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// realiza a analise lexica
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

        // ignora espacos, tabs e quebras de linha
        if (isspace(c)) {
            coluna++;
            continue;
        }

        // Ignorando comentários do tipo /* comentário */
        if (c == '/') {
            char proximo;
            if (arquivo.get(proximo)) {
                if (proximo == '*') {
                    // início do comentário de bloco
                    coluna += 2;
                    char anterior = 0;
                    while (arquivo.get(c)) {
                        coluna++;
                        if (c == '\n') {
                            linha++;
                            coluna = 0;
                        }
                        if (anterior == '*' && c == '/') {
                            coluna++; // quando fecha comentário
                            break; // quando sai do comentário
                        }
                        anterior = c;
                    }
                    continue; // volta ao início do loop, ignora tudo do comentário
                } else {
                    // quando não é comentário, é um operador de divisão normal
                    arquivo.unget(); // Devolve o caractere lido
                }
            }
        }

        // Identificadores ou palavras reservadas (comecam com letra ou '_')
        if(isalpha(c) || c == '_'){
            token = c;
            coluna++; // incremento para o primeiro caractere do token

            while (arquivo.get(c) && (isalnum(c) || c == '_')){
                token += c;
                coluna++; // incremento para cada caractere seguinte
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
            coluna++;
            bool tem_ponto = false;
            bool erro_zero_esquerda = false;
            int coluna_erro = coluna -1; // posição inicial do número

            while (arquivo.get(c)){
                if(isdigit(c)){
                    // Verifica zero à esquerda apenas antes do primeiro dígito não-zero
                    if (!tem_ponto && token.size() == 1 && token[0] == '0' && c != '0') {
                        erro_zero_esquerda = true;
                    }
                    token += c;
                    coluna++;
                } else if (c == '.' && !tem_ponto) {
                    tem_ponto = true;
                    token += c;
                    coluna++;
                } else {
                    arquivo.unget();
                    break;
                }
            }
            // verificando depois da leitura dos dígitos
            if (tem_ponto) {
                size_t pos_ponto = token.find('.');
                string parte_inteira = token.substr(0, pos_ponto);
                string parte_frac = token.substr(pos_ponto + 1);

                // Erro: parte inteira com zero à esquerda
                if (parte_inteira.size() > 1 && parte_inteira[0] == '0') {
                    cerr << "\033[31mEncontrado ERRO na linha " << linha << ", coluna " << coluna_erro 
                        << ": zero à esquerda em número float '" << token << "'\033[0m\n";
                }

                // Erro: parte fracionária faltando
                if (parte_frac.empty()) {
                    cerr << "\033[31m Encontrado ERRO na linha " << linha << ", coluna " << (coluna - 1)
                        << ": parte fracionária faltando após ponto decimal\033[0m\n";
                }
            } 
            else {
                // Erro: inteiro com zero à esquerda
                if (token.size() > 1 && token[0] == '0') {
                    cerr << "\033[31mEncontrado ERRO na linha " << linha << ", coluna " << coluna_erro
                        << ": zero à esquerda em número inteiro '" << token << "'\033[0m\n";
                }
            }

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
                    } else {
                        cerr << "\033[31mEncontrado ERRO na linha " << linha << ", coluna " << coluna
                            << ": operador '|' incompleto ou inválido\033[0m\n";
                             if (arquivo) arquivo.unget();
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
                    else {
                        cerr << "\033[31mEncontrado ERRO na linha " << linha << ", coluna " << coluna - 1
                            << ": operador '&' incompleto ou inválido\033[0m\n";
                        if (arquivo) arquivo.unget();
                    }
                    break;

                // Caso não seja simbolo conhecido, reporta erro
                default:
                    cerr << "\033[31m"
                        << "Encontrado ERRO na linha " << linha << ", coluna " << coluna
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
