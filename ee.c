#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prueba.h"
#include <ctype.h>

void convertir_minuscula(char* palabra){
    char a = 'a', e = 'e', i = 'i', o = 'o', u = 'u';
    int n = strlen(palabra);
    int j;
    for (j = 0; j < n; j++){
        printf("%d\n", palabra[j]);
        if (palabra[j] != -61){
            //printf("%d\n", palabra[j]);
            palabra[j] = tolower(palabra[j]);
        }
        else{
            switch(palabra[j+1]){
                printf("%d\n", palabra[j+1]);
            case -127:
                eliminar_caracter(palabra, j);
                eliminar_caracter(palabra, j);
                poner_caracter(palabra, palabra, a, j); 
                break;
            case -119:
                eliminar_caracter(palabra, j);
                eliminar_caracter(palabra, j);
                poner_caracter(palabra, palabra, a, j);
                break;
            case -83:
                eliminar_caracter(palabra, j);
                eliminar_caracter(palabra, j);
                poner_caracter(palabra, palabra, a, j);   
                break;
            case -77:
                eliminar_caracter(palabra, j);
                eliminar_caracter(palabra, j);
                poner_caracter(palabra, palabra, a, j);
                break;
            case -70:
                eliminar_caracter(palabra, j);
                eliminar_caracter(palabra, j);
                poner_caracter(palabra, palabra, a, j);
                break;
            }
        }
    } 
}

int main(){
    
    //wchar_t =  
    printf("%s\n", p1);
    return 0;
}