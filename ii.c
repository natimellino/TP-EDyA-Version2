#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>
#include <assert.h>
#include "colas.h"
#include "prueba.h"
#include "tablahash.h"
#include "btree.h"

// void separar(wchar_t* palabra, wchar_t* sub1, wchar_t* sub2, int pos){
//     int len = wcslen(palabra);
//     wchar_t p[len];
//     wcscpy(p, palabra);
//     //wprintf(L"Len palabra: %d\n", len);
//     int j, k = 0;
//     for(int i = len, j = 0; i >= pos; i--, j++){
//        // wprintf(L"Entre al for\n");
//         palabra[i] = palabra[i-1];
//         if(j < pos){
//            sub1[j] = p[j]; 
//         }
//         else{
//             sub2[k] = p[j];
//             k++;
//         }
//     }
//     //wprintf(L"Sali del for\n");
//     palabra[pos] = L' ';
//     palabra[len+1] = L'\0';
//     sub2[k+1] = L'\0';
//     sub1[pos] = L'\0';
// }

// void insertar_caracter(wchar_t* palabra, wchar_t c, int pos){
//     int len = wcslen(palabra);
//     for(int i = len; i > pos; i--){
//         palabra[i] = palabra[i-1];
//     }
//     palabra[pos] = c;
//     palabra[len+1] = L'\0';
// }

Cola reemplazar_caracteres(wchar_t* palabra, wchar_t* temp, Cola cola, int pos){
    wchar_t letras[] = L"áéíóúüabcdefghijklmnñopqrstuvwxyz";
    for(int i = 0; i < wcslen(letras); i++){
        reemplazar(temp, letras[i], pos);
        cola = cola_encolar(cola, temp);
        wcscpy(temp, palabra);
    }
    return cola;
}

Cola insertar_caracteres(wchar_t* palabra, wchar_t* temp, Cola cola, int pos){
    wchar_t letras[] = L"áéíóúüabcdefghijklmnñopqrstuvwxyz";
    for(int i = 0; i < wcslen(letras); i++){
        inserta_caracter(temp, letras[i], pos);
        cola = cola_encolar(cola, temp);
        wcscpy(temp, palabra);
    }
    return cola;
}

int main(){
    setlocale(LC_ALL, "");
    wchar_t p[] = L"cajita";
    wchar_t* p1 = malloc(sizeof(wchar_t)*15);
    wchar_t* p2 = malloc(sizeof(wchar_t)*15);
    wchar_t* aux1 = malloc(sizeof(wchar_t)*15);
    wchar_t* aux2 = malloc(sizeof(wchar_t)*15);
    wcscpy(p1, p);
    wcscpy(p2, p);
    //int lon = wcslen(p1);
    // for (int i = 0; i <= lon; i++){
    //     insertar_caracter(p1, L'ü', i);
    //     wprintf(L"%ls\n", p1);
    //     wcscpy(p1, p);
    // }
    Cola cola = cola_crear();
    cola = insertar_caracteres(p1, p2, cola, 1);
    cola_imprimir(cola);
     
    return 0;
}