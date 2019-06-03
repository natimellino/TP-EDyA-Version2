#ifndef __BTREE_H__
#define __BTREE_H__
#include <wchar.h>

typedef struct _BTNodo {
  void* dato;
  struct _BTNodo* left;
  struct _BTNodo* right;
} BTNodo;

typedef BTNodo* BTree;

/* Devuelve un árbol vacío. */

BTree btree_crear();

/* Inserta una cadena en un árbol. */

BTree bstree_insertar(BTree arbol, wchar_t* palabra);

/* Dada una cadena, devuelve si la misma está o no en el árbol.*/

int buscar_nodo(BTree nodo, wchar_t* palabra);

/* Destruccion del árbol. */

void btree_destruir(BTree nodo);

/* Devuelve la cantidad de nodos de un árbol. */

int cant_nodos(BTree);

/* Crea un nuevo arbol, con el dato dado en el nodo raiz, y los subarboles dados
 a izquierda y derecha.*/

BTree btree_unir(int dato, BTree left, BTree right);

#endif /* __BTREE_H__ */
