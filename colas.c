#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <assert.h>
#include "colas.h"

// typedef struct _CDList{
//     wchar_t* palabra;
//     struct _CDList* ant;
//     struct _CDList* sig;
// }CDList;

// typedef CDList* Cola;

/* Crea una cola. */

Cola cola_crear(){
	return NULL;
}

/* Determina si una cola es vacía. */

int cola_es_vacia(Cola cola){
	return cola == NULL;
}

/* Retorna el primer elemento de la cola. */

wchar_t* cola_primero(Cola cola){
	return cola->palabra;
}

/* Inserta un dato en una cola. */

Cola cola_encolar(Cola cola, wchar_t* palabra){
	Cola nuevoDato = malloc(sizeof(struct _CDList));
	assert(nuevoDato != NULL);
	nuevoDato->palabra = malloc(sizeof(wchar_t)*(wcslen(palabra)+1));
    wcscpy(nuevoDato->palabra, palabra);
	
	if (cola_es_vacia(cola)){
		nuevoDato->sig = nuevoDato;
		nuevoDato->ant = nuevoDato;
        cola = nuevoDato;	
	}
	else{
		Cola ultimo = cola->ant;
		nuevoDato->sig = cola;
		cola->ant = nuevoDato;
		nuevoDato->ant = ultimo;
		ultimo->sig = nuevoDato;
	}
    return cola;
}

/* Elimina el primer elemento de una cola. */

Cola cola_desencolar(Cola cola){
	if (!cola_es_vacia(cola)){
		
		if (cola->sig == cola->ant){ // Hay un solo elemento.
			free(cola);
		}
		else{
			Cola primero = cola;
			primero->ant->sig = primero->sig;
			primero->sig->ant = primero->ant;
			cola = primero->sig;
			free(primero);
		}
	}
    return cola;
}

/* Imprime una cola. */

void cola_imprimir(Cola cola){
	Cola i;
	if (!cola_es_vacia(cola)){
		for(i = cola; i != cola->ant; i = i->sig){
			wprintf(L"%ls\n", i->palabra);
		}
		wprintf(L"%ls\n", i->palabra);
 	}		
}

// int main(){
//     setlocale(LC_ALL, "");
//     Cola cola = cola_crear();
//     FILE* fp;
//     fp = fopen("palabras.txt", "r");
//     wchar_t* linea = malloc(sizeof(char)*25);
//     int len;
//     while(!feof(fp)){
//         //wprintf(L"Leí palabra\n");
//         fgetws(linea, 50, fp);
//         len = wcslen(linea);
//         linea[len-1] = L'\0';
//         cola = cola_encolar(cola, linea);
//         //wprintf(L"%ls\n", linea);
//     }
//     wprintf(L"Lectura finalizada.\n");
//     fclose(fp);
//     cola_imprimir(cola);
//     wprintf(L"\n");
//     cola = cola_desencolar(cola);
//     cola = cola_desencolar(cola);
//     cola_imprimir(cola);
//     return 0;
// }