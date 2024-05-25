#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LENGTH 50

typedef struct {
    int token_no;
    char lexeme[MAX_TOKEN_LENGTH];
    char token_type[MAX_TOKEN_LENGTH];
    int line_no;
} Token;

Token token_table[MAX_TOKENS];
int token_count = 0;

bool is_keyword(char *token) {
    char *keywords[] =  {"int", "main", "begin", "end", "while", };
    int num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(token, keywords[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool is_delimiter(char c) {
    char delimiters[] = {' ', '+', '-', '/', '=', ';', '(', ')', '[', ']', '{', '}', '<', '>', '!', '&', '|', '^', '%', '~', '?', '.', ',', '\'', '\"'};
    int num_delimiters = sizeof(delimiters) / sizeof(delimiters[0]);
    for (int i = 0; i < num_delimiters; i++) {
        if (c == delimiters[i]) {
            return true;
        }
    }
    return false;
}

int main() {
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    char line[1024];
    int line_no = 1;
    while (fgets(line, sizeof(line), fp)) {
        char *token;
        char *delim = " \n\t";
        token = strtok(line, delim);
        while (token != NULL) {
            if (is_delimiter(token[0])) {
                if (strchr("();,", token[0]) != NULL) {
                    token_table[token_count].token_no = token_count + 1;
                    strcpy(token_table[token_count].lexeme, token);
                    strcpy(token_table[token_count].token_type, "Delimiter");
                    token_table[token_count].line_no = line_no;
                    token_count++;
                } else if (strchr("+-*/=", token[0]) != NULL) {
                    if (strchr("+-*/", token[0]) != NULL) {
                        token_table[token_count].token_no = token_count + 1;
                        strcpy(token_table[token_count].lexeme, token);
                        strcpy(token_table[token_count].token_type, "Arithmetic Operator");
                        token_table[token_count].line_no = line_no;
                        token_count++;
                    } else if (token[0] == '=') {
                        token_table[token_count].token_no = token_count + 1;
                        strcpy(token_table[token_count].lexeme, token);
                        strcpy(token_table[token_count].token_type, "Assignment Operator");
                        token_table[token_count].line_no = line_no;
                        token_count++;
                    } else {
                        printf("Error: Invalid character '%c' at line %d\n", token[0], line_no);
                    }
                } else {
                    printf("Error: Invalid character '%c' at line %d\n", token[0], line_no);
                }
            } else {
                if (is_keyword(token)) {
                    token_table[token_count].token_no = token_count + 1;
                    strcpy(token_table[token_count].lexeme, token);
                    strcpy(token_table[token_count].token_type, "Keyword");
                    token_table[token_count].line_no = line_no;
                    token_count++;
                } else if (isdigit(token[0])) {
                    token_table[token_count].token_no = token_count + 1;
                    strcpy(token_table[token_count].lexeme, token);
                    strcpy(token_table[token_count].token_type, "Number");
                    token_table[token_count].line_no = line_no;
                    token_count++;
                } else if (isalpha(token[0])) {
                    token_table[token_count].token_no = token_count + 1;
                    strcpy(token_table[token_count].lexeme, token);
                    strcpy(token_table[token_count].token_type, "Identifier");
                    token_table[token_count].line_no = line_no;
                    token_count++;
                } else {
                    printf("Error: Invalid character '%c' at line %d\n", token[0], line_no);
                }
            }
            token = strtok(NULL, delim);
        }
        line_no++;
    }

    fclose(fp);

    printf("Global Token Table:\n");
    printf("+--------------+----------+---------------+----------+\n");
    printf("| Token No     | Lexeme   | Token         | Line No  |\n");
    printf("+--------------+----------+---------------+----------+\n");
    for (int i = 0; i < token_count; i++) {
        printf("| %-12d | %-8s | %-13s | %-8d |\n",
               token_table[i].token_no,
               token_table[i].lexeme,
               token_table[i].token_type,
               token_table[i].line_no);
    }
    printf("+--------------+----------+---------------+----------+\n");

    FILE *output_file = fopen("tokens.txt", "w");
    if (output_file == NULL) {
        printf("Error opening output file.\n");
        return 1;
    }

    fprintf(output_file, "(");
    for (int i = 0; i < token_count; i++) {
        char symbol = token_table[i].lexeme[0];
        if (is_keyword(token_table[i].lexeme)) {
            fprintf(output_file, "%c", tolower(symbol));
        } else {
            fprintf(output_file, "%c", symbol);
        }
    }
    fprintf(output_file, ")\n");

    fclose(output_file);

    return 0;
}