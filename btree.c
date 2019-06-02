#include "btree.h"
#include <stdlib.h>
#include <wchar.h>

BTree btree_crear() { return NULL; }

void btree_destruir(BTree nodo) {
  if (nodo != NULL) {
    btree_destruir(nodo->left);
    btree_destruir(nodo->right);
    free(nodo->dato);
    free(nodo);
  }
}

BTree bstree_insertar(BTree arbol, wchar_t* palabra) {
  // wchar_t* aux = palabra;

  if (arbol == NULL) {
    arbol = malloc(sizeof(struct _BTNodo));
    arbol->dato = malloc(sizeof(wchar_t*) * wcslen(palabra));
    wcscpy((wchar_t*)arbol->dato, palabra);
    // wprintf(L"palabra: %ls, arbol->dato: %ls\n", palabra, arbol->dato);
    arbol->left = NULL;
    arbol->right = NULL;
  }

  else {
    // nos fijamos si debe ir a la izq o derecha, dependiendo
    // si es mayor o menor que la palabra en nodo->dato
    // en caso de que sean iguales la función no hace nada y
    // la palabra no se inserta ya que sino estaría repetida.
    if (wcscmp(palabra, (wchar_t*)arbol->dato) < 0) {  // es mas chico
      arbol->left = bstree_insertar(arbol->left, palabra);
    }
    if (wcscmp(palabra, (wchar_t*)arbol->dato) > 0) {
      arbol->right = bstree_insertar(arbol->right, palabra);
    }
  }
  return arbol;
}

int buscar_nodo(BTree nodo, wchar_t* palabra) {
  if (nodo == NULL) {
    return 0;
  }
  if (wcscmp((wchar_t*)nodo->dato, palabra) == 0) {
    return 1;
  } else {
    if (wcscmp(palabra, (wchar_t*)nodo->dato) < 0) {
      return buscar_nodo(nodo->left, palabra);
    } else {
      return buscar_nodo(nodo->right, palabra);
    }
  }
}

int cant_nodos(BTree nodo) {
  if (nodo == NULL) return 0;
  return 1 + cant_nodos(nodo->left) + cant_nodos(nodo->right);
}
