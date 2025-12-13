#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void parser(char text[100]){
    Token struct_tokens[100];
    int token_count;
    token_count = token_create(text, struct_tokens);
    int i;
    
    for(i=0;i<token_count;i++){
        printf("%s ",struct_tokens[i].substr);
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
        else if (strcmp(tokens[i],"+")==0){
            t.type = TOKEN_PLUS;
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