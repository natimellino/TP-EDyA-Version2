#include <wchar.h>

// Implementacion de colas con listas circulares doblemente enlazadas.

typedef struct _CDList{
    wchar_t* palabra;
    struct _CDList* ant;
    struct _CDList* sig;
}CDList;

typedef CDList* Cola;

Cola cola_crear();

int cola_es_vacia(Cola cola);

wchar_t* cola_primero(Cola cola);

Cola cola_encolar(Cola cola, wchar_t* palabra);

Cola cola_desencolar(Cola cola);

int cola_cant_elementos(Cola cola);

void cola_destruir(Cola cola);

void cola_imprimir(Cola cola);