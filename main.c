#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

//void parser(char text[100]);

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
    TOKEN_POW,      
    TOKEN_LOG,   
    TOKEN_LN,
    TOKEN_POW,     
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
    //input eq
    printf("Enter the equation: ");
    scanf("%[^\n]", text);    
    printf("%s\n",text);
    
    //sending to function to get values
    parser(text);
    
    //printf("%d\n",eq);
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
    return (t == TOKEN_SIN || t == TOKEN_COS || t == TOKEN_TAN || t == TOKEN_SEC || t == TOKEN_CSC || t == TOKEN_COT || t == TOKEN_LOG || t == TOKEN_LN);
}

void parser(char text[100]){
    Token struct_tokens[100];
    int token_count = token_create(text, struct_tokens);
    int i;

    int has_variable = 0;
    for (i = 0; i < token_count; ++i) {
        if (struct_tokens[i].type == TOKEN_VARIABLE) { has_variable = 1; break; }
    }
    double x_value = 0.0;

    /* Shunting-yard: convert to RPN */
    Token output_queue[200];
    int out_count = 0;
    Token op_stack[200];
    int op_top = -1;

    for (i = 0; i < token_count; ++i) {
        Token tk = struct_tokens[i];
        if (tk.type == TOKEN_NUMBER || tk.type == TOKEN_VARIABLE){
            output_queue[out_count] = tk;
            out_count++;
        }

        else if (is_function(tk.type)) {
            op_stack[++op_top] = tk;
        }

        else if (tk.type == TOKEN_PLUS || tk.type == TOKEN_MINUS || tk.type == TOKEN_MULTIPLY || tk.type == TOKEN_DIVIDE || tk.type == TOKEN_POW){
            while (op_top >= 0){
                Token top = op_stack[op_top];
                if (top.type == TOKEN_LPARENTHESIS){
                    break;
                }

                int p_top = precedence(top.type);
                int p_tok = precedence(tk.type);
                int right_assoc = (tk.type == TOKEN_POW);

                if ((p_top>p_tok) || (p_top==p_tok && !right_assoc) || is_function(top.type)){
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
        if (op_stack[op_top].type == TOKEN_LPARENTHESIS || op_stack[op_top].type == TOKEN_RPARENTHESIS){ 
            printf("Mismatched parentheses\n"); 
            return; 
        }
        output_queue[out_count++] = op_stack[op_top--];
    }

    if (has_variable) {
        for (int k = -100; k <= 100; ++k) {
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
                else if (tk.type == TOKEN_PLUS || tk.type == TOKEN_MINUS || tk.type == TOKEN_MULTIPLY || tk.type == TOKEN_DIVIDE || tk.type == TOKEN_POW) {
                    if (val_top < 1){ 
                        val_top = -2; break; 
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
                    if (val_top < 0) { val_top = -2; break; }
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
                else {
                }
            }
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
            else if (tk.type == TOKEN_PLUS || tk.type == TOKEN_MINUS || tk.type == TOKEN_MULTIPLY || tk.type == TOKEN_DIVIDE || tk.type == TOKEN_POW) {
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
                if (val_top < 0) { printf("Insufficient operands for function %s\n", tk.substr); return; }
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
            else {
                /* ignore unknown */
            }
        }
    }
}


int token_create(char text[100], Token struct_tokens[100]){    
    Token t;

    int i=0;
    Token token;
    char temp[100];
    char tokens[100][100];
    strcpy(temp,text);

    //printf("tdxt: %s",text);
    //printf("temp: %s",temp);
    //printf("reached tokenisation\n");

    char *tokptr = strtok(temp, " ");

    while (tokptr != NULL) {
        //printf("%s\n", tokptr);
        strcpy(tokens[i],tokptr);
        //printf("%s",tokens[i]);
        tokptr = strtok(NULL, " ");
        i++;
    }

    //printf("reached struct declaration\n");

    int token_count = i;
    //printf("token_count : %d", token_count);

    for(i=0;i<token_count;i++){
        char *endptr;
        t.type = TOKEN_UNKNOWN;
        strcpy(t.substr, "");
        t.value = strtod(tokens[i], &endptr);

        if (*endptr == '\0' && endptr != tokens[i]){
            t.type = TOKEN_NUMBER;
        }
        else if (strcmp(tokens[i],"x")==0){
            t.type = TOKEN_VARIABLE;
        }
        
        else if (strcmp(tokens[i],"-")==0){
            t.type = TOKEN_MINUS;
        }
        else if (strcmp(tokens[i],"*")==0){
            t.type = TOKEN_MULTIPLY;
        }
        else if (strcmp(tokens[i],"/")==0){
            t.type = TOKEN_DIVIDE;
        }
        else if (strcmp(tokens[i],"(")==0){
            t.type = TOKEN_LPARENTHESIS;
        }
        else if (strcmp(tokens[i],")")==0){
            t.type = TOKEN_RPARENTHESIS;
        }
        else if (strcmp(tokens[i],"sin")==0){
            t.type = TOKEN_SIN;
        }
        else if (strcmp(tokens[i],"cos")==0){
            t.type = TOKEN_COS;
        }
        else if (strcmp(tokens[i],"tan")==0){
            t.type = TOKEN_TAN;
        }
        else if (strcmp(tokens[i],"sec")==0){
            t.type = TOKEN_SEC;
        }
        else if (strcmp(tokens[i],"csc")==0){
            t.type = TOKEN_CSC;
        }
        else if (strcmp(tokens[i],"cot")==0){
            t.type = TOKEN_COT;
        }
        else if (strcmp(tokens[i],"^")==0){
            t.type = TOKEN_POW;
        }
        else if (strcmp(tokens[i],"log")==0){
            t.type = TOKEN_LOG;
        }
        else if (strcmp(tokens[i],"ln")==0){
            t.type = TOKEN_LN;
        }
        else{
            t.type = TOKEN_UNKNOWN;
        }
        strcpy(t.substr,tokens[i]);
        printf("%s: %d\n",tokens[i],t.type);
        struct_tokens[i] = t;
    }
    return token_count;
}

/*
//parser function
void parser(char text[100]){
    int i=0,a;
    int val=0;
    int x[201],y[100];
    
    for (int k=0;k<=200;k++){
        x[k]=k-100;
    }
    

    for (int k=0;k<=200;k++){
        printf("%d ",x[k]);
    }


    for (int k=0;k<=200;k++){
        a=x[k];
        //printf("%d",a);
        while(text[i]!='\0'){
            if (text[i]==' '){
                continue;
            }
            if (text[i]=='x'){
                if (text[i-1]>='0' && text[i-1]<='9'){
                    if (text[i-2]=='+'){
                        val+=a*(text[i-1]-'0');
                        printf("%d",val);
                    }
                    else if (text[i-2]=='-'){
                        val-=a*(text[i-1]-'0');
                    }
                }
                else if (text[i-1]=='+'){
                    val+=a;
                }
                else if (text[i-1]=='-'){
                    val-=a;
                }
            }
            i+=1;
    
        }
        printf("%d ",val);
        y[k]=val;
        val=0;
        i=0;
    }

    for (int k=0;k<=200;k++){
        printf("%d ",y[k]);
    }
*/