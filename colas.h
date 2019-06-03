#include <wchar.h>

// Implementacion de colas con listas circulares doblemente enlazadas.

typedef struct _CDList {
  wchar_t* palabra;
  struct _CDList* ant;
  struct _CDList* sig;
} CDList;

typedef CDList* Cola;

// Devuelve una cola vacía.

Cola cola_crear();

// Determina si una cola es vacía.

int cola_es_vacia(Cola cola);

// Devuelve el primer elemento de la cola.

wchar_t* cola_primero(Cola cola);

// Agrega un elemento a la cola.

Cola cola_encolar(Cola cola, wchar_t* palabra);

// Elimina el primer elemento de la cola.

Cola cola_desencolar(Cola cola);

// Devuelve la cantidad de elementos de una cola.

int cola_cant_elementos(Cola cola);

// Destruye una cola.

void cola_destruir(Cola cola);

// Imprime una cola.

void cola_imprimir(Cola cola);