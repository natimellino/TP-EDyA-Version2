#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <assert.h>
#include "prueba.h"
#include "tablahash.h"
#include "btree.h"

void poner_caracteres(wchar_t* palabra, int pos){
    wchar_t letras[] = L"áéíóúüabcdefghijklmnñopqrstuvwxyz";
    for (int j = 0; j < wcslen(letras); j++){
        poner_caracter(palabra, letras[j], pos);
        wprintf(L"%ls\n", palabra);
    }
}

int main(){
    wchar_t p[] = L"hola";
    poner_caracter(p, L'ú', 1);
    wprintf(L"ls\n", p);
    return 0;
}