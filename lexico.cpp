// Compiladores 2025.1 - Analisador Lexico

// No linux:
// Compilar com: g++ -o lexico lexico.cpp
// Executar com: ./lexico arquivo.txt

// No windows:
// Compilar com: g++ lexico.cpp
// Executar com: .\a arquivo.txt

#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Tipos de classes de caracteres
enum classes_caracteres {
    letra, digito, underline, ponto, operador, delimitador,
    espaco_em_branco, barra, fim_do_arquivo
};

// Estados possiveis da maquina de estados
enum estados_maquina {
    inicio, id, numero, decimal, estado_barra, comentario,
    estado_operador, estado_delimitador, estado_fim_de_arquivo
};

// Lista de palavras reservadas da linguagem
vector<string> palavras_reservadas = {"if", "else", "while", "break", "print", "readln", "return", "int", "float", "char", "bool", "true", "false"};

// Verifica se o lexema eh uma palavra reservada
bool ehReservada(const string& palavra){
    for (const auto& reservada : palavras_reservadas){
        if (palavra == reservada){
            return true;
        }
    }
    return false;
}

// Classifica o caractere
classes_caracteres classificar_caractere(char c) {
    if (isalpha(c)) {
        return letra;
    }
    if (isdigit(c)) {
        return digito;
    } 
    if (c == '_') {
        return underline;
    }
    if (c == '.') {
        return ponto;
    } 
    if (c == '+' || c == '-' || c == '*' || c == '=' || c == '!' || c == '<' || c == '>' || c == '&' || c == '|') {
        return operador;
    } 
    if (c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',') {
        return delimitador;
    }
    if (isspace(c)) {
        return espaco_em_branco;
    } 
    if (c == '/') {
        return barra;
    } 
    if (c == EOF) {
        return fim_do_arquivo;
    }
}

string reservada_para_maiuscula(string str){
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso correto: " << argv[0] << endl;
        return 1;
    }

    ifstream arquivo(argv[1]);
    if (!arquivo.is_open()) {
        cerr << "Nao foi possivel abrir o arquivo" << endl;
        return 1;
    }

    char c = arquivo.get();
    estados_maquina estado_atual = inicio;
    string lexema = "";
    int linha = 1;
    int coluna = 1;
    int inicio_lexema_coluna = 1;

    while (estado_atual != estado_fim_de_arquivo) {
        classes_caracteres classeCaractere;
        if(c == EOF){
            classeCaractere = fim_do_arquivo;
        }
        else{
            classeCaractere = classificar_caractere(c);
        } 

        switch (estado_atual) {
            case inicio:
                if (classeCaractere == letra || classeCaractere == underline) {
                    inicio_lexema_coluna = coluna;
                    lexema += c;
                    estado_atual = id;
                    c = arquivo.get();
                }
                else if (classeCaractere == digito) {
                    inicio_lexema_coluna = coluna;
                    lexema += c;
                    estado_atual = numero;
                    c = arquivo.get();
                }
                else if (classeCaractere == ponto) {
                    inicio_lexema_coluna = coluna;
                    lexema += c;
                    estado_atual = decimal;
                    c = arquivo.get();
                }
                else if (classeCaractere == barra) {
                    inicio_lexema_coluna = coluna;
                    estado_atual = estado_barra;
                    c = arquivo.get();
                }
                else if (classeCaractere == operador) {
                    inicio_lexema_coluna = coluna;
                    lexema += c;
                    estado_atual = estado_operador;
                    c = arquivo.get();
                }
                else if (classeCaractere == delimitador) {
                    inicio_lexema_coluna = coluna;
                    lexema += c;
                    estado_atual = estado_delimitador;
                    c = arquivo.get();
                }
                else if (classeCaractere == espaco_em_branco) {
                    if (c == '\n'){
                        linha++; 
                        coluna = 1;  
                    }
                    else{
                        coluna++;
                    }
                    c = arquivo.get();
                }
                else if (classeCaractere == fim_do_arquivo) {
                    cout << "EOF\n";
                    estado_atual = estado_fim_de_arquivo;
                }
                else {
                    cerr << "Erro na linha " << linha << ", coluna: " << inicio_lexema_coluna << ": caractere invalido " << c << endl;
                    c = arquivo.get();
                }
                break;

            case id:
                if (isalnum(c) || c == '_') {
                    lexema += c;
                    c = arquivo.get();
                    coluna++;
                } else {
                    if (ehReservada(lexema))
                        cout << reservada_para_maiuscula(lexema) << "\n";
                    else
                        cout << "ID." << lexema << "\n";
                    lexema.clear();
                    estado_atual = inicio;
                }
                break;

            case numero:
                if (isdigit(c)) {
                    lexema += c;
                    c = arquivo.get();
                    coluna++;
                }
                else if (c == '.') {
                    lexema += c;
                    estado_atual = decimal;
                    c = arquivo.get();
                    coluna++;
                }
                else {
                    if (lexema.length() > 1 && lexema[0] == '0') {
                        cerr << "Erro na linha " << linha << ", coluna: "<< inicio_lexema_coluna << ": numero inteiro com zero a esquerda: " << lexema << endl;
                    }
                    cout << "NUMINT." << lexema << "\n";
                    lexema.clear();
                    estado_atual = inicio;
                }
                break;

            case decimal:
                if (isdigit(c)) {
                    lexema += c;
                    c = arquivo.get();
                    coluna++;
                } else {
                    int posPonto = lexema.find('.');
                    string parteInteira = lexema.substr(0, posPonto);
                    string parteFracionaria = lexema.substr(posPonto + 1);

                    if (parteInteira.length() > 1 && parteInteira[0] == '0') {
                        cerr << "Erro na linha " << linha << ", coluna " << inicio_lexema_coluna << ": numero float com zero a esquerda: " << lexema << endl;
                    }
                    if (parteFracionaria.empty()) {
                        cerr << "Erro na linha " << linha << ", coluna " << inicio_lexema_coluna << ": parte fracionaria ausente depois do ponto decimal" << endl;
                    }

                    cout << "NUM_FLOAT." << lexema << "\n";
                    lexema.clear();
                    estado_atual = inicio;
                }
                break;

            case estado_barra:
                if (c == '*') {
                    char anterior = 0;
                    while (arquivo.get(c)) {
                        coluna++;
                        if (c == '\n'){
                            linha++;
                            coluna = 1;
                        } 
                        if (anterior == '*' && c == '/'){
                            break;
                        } 
                        anterior = c;
                    }
                    c = arquivo.get();
                    estado_atual = inicio;
                } else {
                    cout << "DIV\n";
                    estado_atual = inicio;
                }
                break;

            case estado_operador: {
                string op_composto = lexema+c;
                if ((op_composto == "==") || (op_composto == "<=") ||
                    (op_composto == ">=") || (op_composto == "!=") ||
                    (op_composto == "&&") || (op_composto == "||") ||
                    (op_composto == "++") || (op_composto == "--")) {
                    lexema += c;
                    c = arquivo.get();
                    coluna++;
                }

                if (lexema == "=="){
                    cout << "ASSIGN\n";
                } 
                else if (lexema == "!="){
                    cout << "DIFF\n";
                } 
                else if (lexema == "<="){
                    cout << "LEQ\n";
                } 
                else if (lexema == ">="){
                    cout << "GEQ\n";
                } 
                else if (lexema == "&&"){
                    cout << "AND\n";
                } 
                else if (lexema == "||"){
                    cout << "OR\n";
                } 
                else if (lexema == "++"){
                    cout << "INCREMENT\n";
                } 
                else if (lexema == "--") {
                    cout << "DECREMENT\n";
                }
                else if (lexema == "+"){
                    cout << "PLUS\n";
                } 
                else if (lexema == "-"){
                    cout << "MINUS\n";
                } 
                else if (lexema == "*"){
                    cout << "MULT\n";
                } 
                else if (lexema == "="){
                    cout << "EQ\n";
                } 
                else if (lexema == "<"){
                    cout << "LT\n";
                } 
                else if (lexema == ">"){
                    cout << "GT\n";
                } 
                else if (lexema == "!"){
                    cout << "NEG\n";
                }
                else{
                    cerr << "Erro na linha " << linha << ", coluna " << inicio_lexema_coluna << ": operador invalido" << lexema << endl;
                } 
                lexema.clear();
                estado_atual = inicio;
                break;
            }
            case estado_delimitador:
                if (lexema == "("){
                    cout << "LPARENT\n";
                } 
                else if (lexema == ")"){
                    cout << "RPARENT\n";
                } 
                else if (lexema == "{"){
                    cout << "LBRACE\n";
                } 
                else if (lexema == "}"){
                    cout << "RBRACE\n";
                } 
                else if (lexema == ";"){
                    cout << "SEMICOLON\n";
                }
                else if (lexema == ","){
                    cout << "COMMA\n";
                } 
                else if (lexema == "%"){
                    cout << "MOD\n";
                } 

                lexema.clear();
                estado_atual = inicio;
                break;

            default:
                cerr << "Erro: Estado invalido na linha " << linha << ", coluna " << coluna << endl;
                estado_atual = inicio;
                break;
        }
    }

    arquivo.close();
    return 0;
}
