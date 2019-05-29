#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <ctype.h>
#include <assert.h>
#include <locale.h>


// Intercambia las posiciones i, j de la palabra dada.

void intercambiar(wchar_t* palabra, int i, int j){
    int len = wcslen(palabra);
    if (i < len && j < len){
        wchar_t temp = palabra[i];
        palabra[i] = palabra[j];
        palabra[j] = temp; 
    }
}

// Elimina el caracter de la posicion i.

void eliminar_caracter(wchar_t* palabra, int i){
    int len = wcslen(palabra);
    int j;
    for(j = i; j < len; j++){
        palabra[j] = palabra[j+1];
    }
    palabra[len-1] = '\0';
}

void reemplazar(wchar_t* palabra, wchar_t c, int pos){
    palabra[pos] = c;
} 
// Agrega al caracter c en la posición pos.

void inserta_caracter(wchar_t* palabra, wchar_t c, int pos){
    int len = wcslen(palabra);
    for(int i = len; i > pos; i--){
        palabra[i] = palabra[i-1];
    }
    palabra[pos] = c;
    palabra[len+1] = L'\0';
}
// Separa la palabra en dos agregando un espacio en palabra[pos].

void separar(wchar_t* palabra, wchar_t* sub1, wchar_t* sub2, int pos){
    int len = wcslen(palabra);
    if (pos != 0 && pos != len){

        int i, j = 0;
        for (i = 0; i < len; i++){
            wprintf(L"i: %d\n", i);
            if (i < pos){
                sub1[i] = palabra[i];
                wprintf(L"%lc\n", palabra[i]);
            }   
            if (i >= pos && j < len - pos){
                sub2[j] = palabra[i];
                j++;
            } 
        }
        //palabra[len] = '\0';
        wprintf(L"%ls, %ls\n", sub1, sub2);
        sub1[i-j] = '\0';
        sub2[j] = '\0';
        //poner_caracter(palabra, ' ', pos);
        
    } 
}





