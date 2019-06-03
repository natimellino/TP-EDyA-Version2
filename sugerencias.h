#include <stdio.h>
#include <wchar.h>

// Estructura para la lista de sugerencias, donde se guardan:
// -) Un arreglo de wchar_t** para almacenar palabras.
// -) La capacidad total del arreglo (capacidad).
// -) El número de elementos que tiene el arreglo hasta el momento (nelems).

typedef struct _Sugerencias {
  wchar_t **arreglo;
  int capacidad;
  int nelems;
} Sugerencias;

typedef Sugerencias *ListaSugerencias;

// Esta funcion recibe una capacidad y devuelve una lista de sugerencias.
ListaSugerencias crear_lista(int capacidad);

// Esta funcion recibe una lista y una cadena y devuelve 1 si la palabra se
// encuentra en la lista y 0 caso contrario.
int buscar_elemento(ListaSugerencias lista, wchar_t *palabra);

// Esta funcion recibe una lista, una cadena y agrega dicha cadena a la lista
// de sugerencias.
ListaSugerencias agregar_elemento(ListaSugerencias lista, wchar_t *string);

// Esta funcion imprime una lista.
void imprimir_lista(ListaSugerencias lista);

// Esta funcion destruye la lista.
void destruir_lista(ListaSugerencias lista);
