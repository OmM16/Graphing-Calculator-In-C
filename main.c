#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef enum {
    TOKEN_NUMBER = 0,     
    TOKEN_VARIABLE,   
    TOKEN_PLUS,       
    TOKEN_MINUS,      
    TOKEN_MULTIPLY,   
    TOKEN_DIVIDE,    
    TOKEN_LPARENTHESIS,     
    TOKEN_RPARENTHESIS,  
    TOKEN_SIN,   
    TOKEN_COS,    
    TOKEN_POW,
    TOKEN_TAN,    
    TOKEN_SEC,    
    TOKEN_CSC,    
    TOKEN_COT, 
    TOKEN_LOG,   
    TOKEN_LN,     
    TOKEN_UNKNOWN     
} TokenType;

typedef struct {
    TokenType type;
    char substr[100];  
    double value;     
} Token;

int token_create(char text[100], Token struct_tokens[100]);
void parser(char text[100]);
int is_function(TokenType t);
int precedence(TokenType t);

int main(){
    char text[100];
    printf("Enter the equation: ");
    scanf("%[^\n]", text);    
    printf("%s\n",text);
    
    parser(text);
    
    return 0;
}

int precedence(TokenType t) {
    switch (t) {
        case TOKEN_PLUS: case TOKEN_MINUS: return 1;
        case TOKEN_MULTIPLY: case TOKEN_DIVIDE: return 2;
        case TOKEN_POW: return 3;
        default: return 4;
    }
}

int is_function(TokenType t) {
    return (t == TOKEN_SIN || t == TOKEN_COS || t == TOKEN_TAN || 
            t == TOKEN_SEC || t == TOKEN_CSC || t == TOKEN_COT || 
            t == TOKEN_LOG || t == TOKEN_LN);
}

void parser(char text[100]){
    Token struct_tokens[100];
    int token_count = token_create(text, struct_tokens);
    int i;

    int has_variable = 0;
    for (i = 0; i < token_count; ++i) {
        if (struct_tokens[i].type == TOKEN_VARIABLE) { 
            has_variable = 1; 
            break; 
        }
    }

    /* Shunting-yard: convert to RPN */
    Token output_queue[200];
    int out_count = 0;
    Token op_stack[200];
    int op_top = -1;

    for (i = 0; i < token_count; ++i) {
        Token tk = struct_tokens[i];
        if (tk.type == TOKEN_NUMBER || tk.type == TOKEN_VARIABLE){
            output_queue[out_count++] = tk;
        }
        else if (is_function(tk.type)) {
            op_stack[++op_top] = tk;
        }
        else if (tk.type == TOKEN_PLUS || tk.type == TOKEN_MINUS || 
                 tk.type == TOKEN_MULTIPLY || tk.type == TOKEN_DIVIDE || 
                 tk.type == TOKEN_POW){
            while (op_top >= 0){
                Token top = op_stack[op_top];
                if (top.type == TOKEN_LPARENTHESIS){
                    break;
                }
                int p_top = precedence(top.type);
                int p_tok = precedence(tk.type);
                int right_assoc = (tk.type == TOKEN_POW);

                if ((p_top>p_tok) || (p_top==p_tok && !right_assoc) || 
                    is_function(top.type)){
                    output_queue[out_count++] = top;
                    op_top--;
                }
                else{
                    break;
                }
            }
            op_stack[++op_top] = tk;
        }
        else if (tk.type == TOKEN_LPARENTHESIS){
            op_stack[++op_top] = tk;
        }
        else if (tk.type == TOKEN_RPARENTHESIS){
            int found_lparen = 0;
            while (op_top >= 0) {
                Token top = op_stack[op_top--];
                if (top.type == TOKEN_LPARENTHESIS){ 
                    found_lparen = 1; 
                    break; 
                }
                output_queue[out_count++] = top;
            }
            if (!found_lparen){ 
                printf("Mismatched parentheses\n"); 
                return; 
            }
            if (op_top >= 0 && is_function(op_stack[op_top].type)) {
                output_queue[out_count++] = op_stack[op_top--];
            }
        }
    }   

    while (op_top >= 0) {
        if (op_stack[op_top].type == TOKEN_LPARENTHESIS || 
            op_stack[op_top].type == TOKEN_RPARENTHESIS){ 
            printf("Mismatched parentheses\n"); 
            return; 
        }
        output_queue[out_count++] = op_stack[op_top--];
    }

    if (has_variable) {
        FILE *fp = fopen("data.txt", "w");
        if (fp == NULL) {
            printf("Error opening file!\n");
            return;
        }
        
        printf("\nx\t\ty\n");
        for (double k = -100; k <=100; k+=0.01) {
            double xv = (double)k;
            double val_stack[200];
            int val_top = -1;
            
            for (i = 0; i < out_count; ++i) {
                Token tk = output_queue[i];
                if (tk.type == TOKEN_NUMBER) {
                    val_stack[++val_top] = tk.value;
                }
                else if (tk.type == TOKEN_VARIABLE) {
                    val_stack[++val_top] = xv;
                }
                else if (tk.type == TOKEN_PLUS || tk.type == TOKEN_MINUS || 
                         tk.type == TOKEN_MULTIPLY || tk.type == TOKEN_DIVIDE || 
                         tk.type == TOKEN_POW) {
                    if (val_top < 1){ 
                        val_top = -2; 
                        break; 
                    }
                    double b = val_stack[val_top--];
                    double a = val_stack[val_top--];
                    double res = 0.0;
                    if (tk.type == TOKEN_PLUS) res = a + b;
                    else if (tk.type == TOKEN_MINUS) res = a - b;
                    else if (tk.type == TOKEN_MULTIPLY) res = a * b;
                    else if (tk.type == TOKEN_DIVIDE) res = a / b;
                    else if (tk.type == TOKEN_POW) res = pow(a, b);
                    val_stack[++val_top] = res;
                }
                else if (is_function(tk.type)) {
                    if (val_top < 0) { 
                        val_top = -2; 
                        break; 
                    }
                    double a = val_stack[val_top--];
                    double res = 0.0;
                    switch (tk.type) {
                        case TOKEN_SIN: res = sin(a); break;
                        case TOKEN_COS: res = cos(a); break;
                        case TOKEN_TAN: res = tan(a); break;
                        case TOKEN_SEC: res = 1.0 / cos(a); break;
                        case TOKEN_CSC: res = 1.0 / sin(a); break;
                        case TOKEN_COT: res = 1.0 / tan(a); break;
                        case TOKEN_LOG: res = log10(a); break;
                        case TOKEN_LN:  res = log(a); break;
                        default: res = 0.0; break;
                    }
                    val_stack[++val_top] = res;
                }
            }
            
            if (val_top >= 0) {
                double y = val_stack[val_top];
                printf("%g\t\t%g\n", xv, y);
                fprintf(fp, "%g %g\n", xv, y);
            }
        }
        fclose(fp);
        printf("\nData written to data.txt\n");

        // Write gnuplot script
        FILE *gp = fopen("plot.gp", "w");
        if (gp != NULL) {
            fprintf(gp, "set title 'Equation Plot'\n");
            fprintf(gp, "set xlabel 'x'\n");
            fprintf(gp, "set ylabel 'y'\n");
            fprintf(gp, "set grid\n");
            fprintf(gp, "set yrange [-10:10]\n");
            fprintf(gp, "set style line 1 lc rgb '#0060ad' lt 1 lw 2\n");
            fprintf(gp, "plot 'data.txt' with lines ls 1 title 'y=f(x)'\n");
            fprintf(gp, "pause -1\n");
            fclose(gp);
            system("gnuplot plot.gp");
        } 
        else {
            printf("Error creating gnuplot script!\n");
        }
    } 
    else {
        double val_stack[200];
        int val_top = -1;
        for (i = 0; i < out_count; ++i) {
            Token tk = output_queue[i];
            if (tk.type == TOKEN_NUMBER) {
                val_stack[++val_top] = tk.value;
            }
            else if (tk.type == TOKEN_PLUS || tk.type == TOKEN_MINUS || 
                     tk.type == TOKEN_MULTIPLY || tk.type == TOKEN_DIVIDE || 
                     tk.type == TOKEN_POW) {
                if (val_top < 1){ 
                    printf("Insufficient operands\n"); 
                    return; 
                }
                double b = val_stack[val_top--];
                double a = val_stack[val_top--];
                double res = 0.0;
                if (tk.type == TOKEN_PLUS) res = a + b;
                else if (tk.type == TOKEN_MINUS) res = a - b;
                else if (tk.type == TOKEN_MULTIPLY) res = a * b;
                else if (tk.type == TOKEN_DIVIDE) res = a / b;
                else if (tk.type == TOKEN_POW) res = pow(a, b);
                val_stack[++val_top] = res;
            }
            else if (is_function(tk.type)) {
                if (val_top < 0) { 
                    printf("Insufficient operands for function %s\n", tk.substr); 
                    return; 
                }
                double a = val_stack[val_top--];
                double res = 0.0;
                switch (tk.type) {
                    case TOKEN_SIN: res = sin(a); break;
                    case TOKEN_COS: res = cos(a); break;
                    case TOKEN_TAN: res = tan(a); break;
                    case TOKEN_SEC: res = 1.0 / cos(a); break;
                    case TOKEN_CSC: res = 1.0 / sin(a); break;
                    case TOKEN_COT: res = 1.0 / tan(a); break;
                    case TOKEN_LOG: res = log10(a); break;
                    case TOKEN_LN:  res = log(a); break;
                    default: res = 0.0; break;
                }
                val_stack[++val_top] = res;
            }
        }
        if (val_top >= 0) {
            printf("\nResult: %g\n", val_stack[val_top]);
        } else {
            printf("\nError: Invalid expression\n");
        }
    }
}

int token_create(char text[100], Token struct_tokens[100]){    
    int token_count = 0;
    int i = 0;
    
    while (text[i] != '\0') {
        Token t;
        t.type = TOKEN_UNKNOWN;
        strcpy(t.substr, "");
        t.value = 0.0;
        
        while (text[i] != '\0' && isspace(text[i])) {
            i++;
        }
        if (text[i] == '\0') break;
        
        if (isdigit(text[i]) || (text[i] == '.' && isdigit(text[i+1]))) {
            char numstr[100];
            int j = 0;
            while (text[i] != '\0' && (isdigit(text[i]) || text[i] == '.')) {
                numstr[j++] = text[i++];
            }
            numstr[j] = '\0';
            t.type = TOKEN_NUMBER;
            t.value = atof(numstr);
            strcpy(t.substr, numstr);
        }
        else if (text[i] == '+') {
            t.type = TOKEN_PLUS;
            strcpy(t.substr, "+");
            i++;
        }
        else if (text[i] == '-') {
            t.type = TOKEN_MINUS;
            strcpy(t.substr, "-");
            i++;
        }
        else if (text[i] == '*') {
            t.type = TOKEN_MULTIPLY;
            strcpy(t.substr, "*");
            i++;
        }
        else if (text[i] == '/') {
            t.type = TOKEN_DIVIDE;
            strcpy(t.substr, "/");
            i++;
        }
        else if (text[i] == '^') {
            t.type = TOKEN_POW;
            strcpy(t.substr, "^");
            i++;
        }
        else if (text[i] == '(') {
            t.type = TOKEN_LPARENTHESIS;
            strcpy(t.substr, "(");
            i++;
        }
        else if (text[i] == ')') {
            t.type = TOKEN_RPARENTHESIS;
            strcpy(t.substr, ")");
            i++;
        }
        else if (isalpha(text[i])) {
            char word[100];
            int j = 0;
            while (text[i] != '\0' && isalpha(text[i])) {
                word[j++] = text[i++];
            }
            word[j] = '\0';
            strcpy(t.substr, word);
            
            if (strcmp(word, "x") == 0) {
                t.type = TOKEN_VARIABLE;
            }
            else if (strcmp(word, "sin") == 0) {
                t.type = TOKEN_SIN;
            }
            else if (strcmp(word, "cos") == 0) {
                t.type = TOKEN_COS;
            }
            else if (strcmp(word, "tan") == 0) {
                t.type = TOKEN_TAN;
            }
            else if (strcmp(word, "sec") == 0) {
                t.type = TOKEN_SEC;
            }
            else if (strcmp(word, "csc") == 0) {
                t.type = TOKEN_CSC;
            }
            else if (strcmp(word, "cot") == 0) {
                t.type = TOKEN_COT;
            }
            else if (strcmp(word, "log") == 0) {
                t.type = TOKEN_LOG;
            }
            else if (strcmp(word, "ln") == 0) {
                t.type = TOKEN_LN;
            }
            else {
                t.type = TOKEN_UNKNOWN;
            }
        }
        else {
            i++;
        }
        
        if (t.type != TOKEN_UNKNOWN) {
            struct_tokens[token_count] = t;
            printf("%s: %d\n", t.substr, t.type);
            token_count++;
        }
    }
    
    return token_count;
}



    
