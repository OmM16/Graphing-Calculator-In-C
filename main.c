#include <stdio.h>
void parser(char text[100]);
int main(){
    char text[100];
    //input eq
    printf("Enter the equation: ");
    int i=0;
    char ch;
    while(ch!='\n'){
        ch = getchar();
        text[i]=ch;
        i++;
    }
    text[i-1]='\0';
    
    printf("%s\n",text);
    
    //sending to function to get values
    int eq;
    parser(text);

    //printf("%d\n",eq);
    return 0;
}

//parser function
void parser(char text[100]){
    int i=0,a;
    int val=0;
    int x[201],y[100];
    
    for (int k=0;k<=200;k++){
        x[k]=k-100;
    }
    
    /*
    for (int k=0;k<=200;k++){
        printf("%d ",x[k]);
    }
    */

    for (int k=0;k<=200;k++){
        a=x[k];
        //printf("%d",a);
        while(text[i]!='\0'){
            if (text[i]=='x'){
                if (text[i-1]=='+'){
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

    /*
    for (int k=0;k<=200;k++){
        printf("%d ",y[k]);
    }
    */
}