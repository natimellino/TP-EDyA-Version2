#ifndef __BTREE_H__
#define __BTREE_H__
#include <wchar.h>

typedef void (*FuncionVisitante) (void* dato);

typedef void (*FuncionVisitanteExtra) (int dato, void *extra);

typedef enum {
  BTREE_RECORRIDO_IN,
  BTREE_RECORRIDO_PRE,
  BTREE_RECORRIDO_POST
} BTreeOrdenDeRecorrido;

typedef struct _BTNodo {
  void* dato;
  struct _BTNodo *left;
  struct _BTNodo *right;
} BTNodo;

typedef BTNodo *BTree;

/**
 * Devuelve un arbol vacío.
 */
BTree btree_crear();

BTree bstree_insertar(BTree arbol, wchar_t* palabra);

int buscar_nodo(BTree nodo, wchar_t* palabra);
/**
 * Destruccion del árbol.
 */
void btree_destruir(BTree nodo);

int cant_nodos(BTree);

/**
 * Crea un nuevo arbol, con el dato dado en el nodo raiz, y los subarboles dados
 * a izquierda y derecha.
 */
BTree btree_unir(int dato, BTree left, BTree right);

/**
 * Recorrido del arbol, utilizando la funcion pasada.
 */
void btree_recorrer(BTree arbol, BTreeOrdenDeRecorrido orden,
                    FuncionVisitante visit);

void btree_recorrer_extra(BTree arbol, FuncionVisitanteExtra visit, void *extra);

#endif /* __BTREE_H__ */
