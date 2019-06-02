#include "tablahash.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "btree.h"

/**
 * Crea una nueva tabla Hash vacía, con la capacidad dada.
 */
TablaHash* tablahash_crear(unsigned capacidad, FuncionHash hash) {
  // Pedimos memoria para la estructura principal y las casillas.
  TablaHash* tabla = malloc(sizeof(TablaHash));
  tabla->hash = hash;
  tabla->capacidad = capacidad;
  tabla->tabla = malloc(sizeof(CasillaHash) * capacidad);
  tabla->numElems = 0;

  // Inicializamos las casillas con datos nulos.
  for (unsigned idx = 0; idx < capacidad; ++idx) {
    // tabla->tabla[idx].clave = NULL;
    tabla->tabla[idx].nodoRaiz = btree_crear();
  }

  return tabla;
}

/**
 * Inserta el dato en la tabla, asociado a la clave dada.
 */
void tablahash_insertar(TablaHash* tabla, void* palabra) {
  // Calculamos la posición de la clave dada, de acuerdo a la función hash.
  unsigned idx = tabla->hash(palabra);
  idx = idx % tabla->capacidad;
  tabla->numElems++;
  tabla->tabla[idx].nodoRaiz =
      bstree_insertar(tabla->tabla[idx].nodoRaiz, (wchar_t*)palabra);
}

/**
 * Busca un elemento dado en la tabla, y retorna un puntero al mismo.
 * En caso de no existir, se retorna un puntero nulo.
 */
int tablahash_buscar(TablaHash* tabla, void* palabra) {
  // Calculamos la posición de la clave dada, de acuerdo a la función hash.
  unsigned idx = tabla->hash(palabra);
  idx = idx % tabla->capacidad;

  // Si el lugar esta vacío, retornamos un puntero nulo.
  return buscar_nodo(tabla->tabla[idx].nodoRaiz, (wchar_t*)palabra);
}

/* Destruye la tabla. */

void tablahash_destruir(TablaHash* tabla) {
  for (int i = 0; i < tabla->capacidad; i++) {
    btree_destruir(tabla->tabla[i].nodoRaiz);
  }
  free(tabla->tabla);
  free(tabla);
}
