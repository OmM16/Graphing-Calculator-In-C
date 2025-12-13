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
    TOKEN_UNKNOWN     
} TokenType;

typedef struct {
    TokenType type;
    char substr[100];  
    double value;     
} Token;

Token token_create(char text[100]);
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
    token_create(text);
}

Token token_create(char text[100]){    
    Token t;

    int i=0;
    Token token;
    char temp[100];
    char tokens[100][100];
    Token struct_tokens[100];
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
    i=0;

    for(i=0;i<token_count;i++){
        t.type = TOKEN_UNKNOWN;
        strcpy(t.substr,"");
        t.value = 0;

        if (isdigit(tokens[i][0])){
            char *end;
            t.type = TOKEN_NUMBER;
            t.value = strtod(tokens[i],&end);
            //printf("%lf\n",t.value);
        }
        else if (strcmp(tokens[i],"x")==0){
            t.type = TOKEN_VARIABLE;
        }
        else if (strcmp(tokens[i],"+")==0){
            t.type = TOKEN_PLUS;
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
        else{
            t.type = TOKEN_UNKNOWN;
        }
        strcpy(t.substr,tokens[i]);
        printf("%s: %d\n",tokens[i],t.type);
        struct_tokens[i] = t;
    }

    for(i=0;i<token_count;i++){
        printf("%s ",struct_tokens[i].substr);
    }
    return t;
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