#include "sugerencias.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

ListaSugerencias crear_lista(int capacidad) {
  ListaSugerencias lista = malloc(sizeof(struct _Sugerencias));
  lista->arreglo = malloc(sizeof(wchar_t *) * capacidad);
  lista->capacidad = capacidad;
  lista->nelems = 0;
  return lista;
}

int buscar_elemento(ListaSugerencias lista, wchar_t *palabra) {
  int i = 0, encontrado = 0;
  while (i < lista->nelems && !encontrado) {
    if (wcscmp(lista->arreglo[i], palabra) == 0) {
      encontrado = 1;
    }
    i++;
  }
  return encontrado;
}

ListaSugerencias agregar_elemento(ListaSugerencias lista, wchar_t *string) {
  if (lista->nelems < lista->capacidad - 1 && !buscar_elemento(lista, string)) {
    lista->arreglo[lista->nelems] =
        malloc(sizeof(wchar_t) * (wcslen(string) + 2));
    wcscpy(lista->arreglo[lista->nelems], string);
    lista->nelems++;
  }
  return lista;
}

void imprimir_lista(ListaSugerencias lista) {
  if (lista->nelems > 0) {
    // printf("entre al if\n");
    for (int i = 0; i < lista->nelems; i++) {
      // printf("entre al for\n");
      wprintf(L"%ls\n", lista->arreglo[i]);
    }
  } else {
    wprintf(L"No existen sugerencias para la palabra ingresada.\n");
  }
}

void destruir_lista(ListaSugerencias lista) {
  for (int i = 0; i < lista->nelems; ++i) {
    free(lista->arreglo[i]);
  }
  free(lista->arreglo);
  free(lista);
}