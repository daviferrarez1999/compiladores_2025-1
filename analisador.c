// Compiladores 2025.1 - Analisador Léxico

// Para ler o arquivo no terminal: Se tiver no output, saia "cd .." e em seguida digite .\output\analisador.exe arquivo.txt
// Bibliotecas padrão da linguagem C usadas no programa
#include <stdio.h>   // Para entrada e saída (ex: printf, fopen)
#include <stdlib.h>  // Para funções utilitárias (ex: exit)
#include <ctype.h>   // Para funções de verificação de caracteres (ex: isalpha, isdigit)
#include <string.h>  // Para manipulação de strings (ex: strcmp)

// Define o tamanho máximo que um token pode ter
#define TAM_TOKEN 100

// Lista com todas as palavras reservadas da linguagem C–
// Essas palavras são reconhecidas como comandos específicos, não como identificadores
const char* palavras_reservadas[] = {
    "if", "else", "while", "break", "print", "readln", "return",
    "int", "float", "char", "bool", "true", "false"
};

// Quantidade total de palavras reservadas na lista
int total_palavras = 13;

// Função que verifica se uma string (palavra) é uma palavra reservada
int is_palavra_reservada(const char* palavra) {
    for (int i = 0; i < total_palavras; i++) {
        // Compara a palavra recebida com cada palavra reservada da lista
        if (strcmp(palavras_reservadas[i], palavra) == 0) {
            return 1; // É palavra reservada
        }
    }
    return 0; // Não é palavra reservada
}

// Função principal que faz a análise léxica do arquivo
void analisar(FILE* arquivo) {
    char c;                    // Armazena o caractere lido
    char token[TAM_TOKEN];     // Vetor para armazenar o token em construção
    int i;                     // Índice auxiliar

    // Lê o arquivo caractere por caractere até o fim
    while ((c = fgetc(arquivo)) != EOF) { 
        // Ignora espaços, tabs e quebras de linha (não são tokens)
        if (isspace(c)) continue;

        // Se o caractere é uma letra ou underline (_), pode ser um identificador ou palavra reservada
        if (isalpha(c) || c == '_') {  //isalpha -> verifica se é letra
            i = 0;
            token[i++] = c;

            // Continua lendo enquanto forem letras, números ou underline
            while (isalnum(c = fgetc(arquivo)) || c == '_') {  //isalnum -> verifica se é letra ou número
                token[i++] = c;
            }

            token[i] = '\0';       // Finaliza a string. \0 é o caractere nulo que indica o fim da string
            ungetc(c, arquivo);    // Devolve o último caractere lido (não faz parte do token). 

            // Verifica se o token é uma palavra reservada
            if (is_palavra_reservada(token)) {
                // Converte para maiúsculas (ex: if → IF) e imprime
                printf("%s\n", strupr(token));  // ⚠️ Só funciona no Windows!
            } else {
                // Senão, é um identificador comum (ex: x, soma)
                printf("ID.%s\n", token);
            }
        }

        // Se o caractere é um dígito, trata como número inteiro
        else if (isdigit(c)) { //isdgit -> verifica se é dígito
            i = 0;
            token[i++] = c;

            // Continua lendo enquanto forem dígitos
            while (isdigit(c = fgetc(arquivo))) {
                token[i++] = c;
            }

            token[i] = '\0';       // Finaliza a string
            ungetc(c, arquivo);    // Devolve o caractere que não pertence ao número

            printf("NUM.%s\n", token); // Imprime o número encontrado
        }

        // Tratamento dos símbolos individuais (operadores e pontuação)
        else {
            switch (c) {
                case '(': printf("LPARENT\n"); break;     // Parêntese esquerdo
                case ')': printf("RPARENT\n"); break;     // Parêntese direito
                case '{': printf("LBRACE\n"); break;      // Chave esquerda
                case '}': printf("RBRACE\n"); break;      // Chave direita
                case ';': printf("SEMICOLON\n"); break;   // Ponto e vírgula
                case ',': printf("COMMA\n"); break;       // Vírgula
                case '=': printf("ASSIGN\n"); break;      // Sinal de atribuição
                case '+': printf("PLUS\n"); break;        // Operador de soma
                case '-': printf("MINUS\n"); break;       // Operador de subtração
                case '*': printf("MULT\n"); break;        // Operador de multiplicação
                case '/': printf("DIV\n"); break;         // Operador de divisão
                case '<': printf("LT\n"); break;          // Menor que
                case '>': printf("GT\n"); break;          // Maior que

                // Se não for nenhum dos reconhecidos, imprime erro
                default:
                    printf("ERRO: caractere inválido '%c'\n", c);
            }
        }
    }

    // Após o fim do arquivo, imprime EOF
    printf("EOF\n");
}

// Função principal do programa
int main(int argc, char* argv[]) {
    // Verifica se o nome do arquivo foi passado na linha de comando
    if (argc < 2) {
        printf("Uso: %s <arquivo fonte>\n", argv[0]);
        return 1;
    }

    // Tenta abrir o arquivo para leitura
    FILE* arquivo = fopen(argv[1], "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo"); // Mensagem de erro se não conseguir abrir
        return 1;
    }

    // Chama a função que faz a análise léxica
    analisar(arquivo);

    // Fecha o arquivo após análise
    fclose(arquivo);
    return 0;
}
