#include <assert.h>
#include <ctype.h>
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "btree.h"
#include "colas.h"
#include "prueba.h"
#include "tablahash.h"
// reemplaza por todos los posibles caracteres en una palabra dada una posicion.
Cola reemplazar_caracteres(wchar_t *palabra, wchar_t *temp, Cola cola,
                           TablaHash *diccionario, ListaSugerencias lista,
                           int pos) {
  wchar_t letras[] = L"áéíóúüabcdefghijklmnñopqrstuvwxyz";
  for (int i = 0; i < wcslen(letras) && lista->nelems < 5; i++) {
    reemplazar(temp, letras[i], pos);
    if (tablahash_buscar(diccionario, temp))
      lista = agregar_elemento(lista, temp);
    cola = cola_encolar(cola, temp);
    wcscpy(temp, palabra);
  }
  return cola;
}

// inserta todos los posibles caracteres en una palabra dada una posicion.
Cola insertar_caracteres(wchar_t *palabra, wchar_t *temp, Cola cola,
                         TablaHash *diccionario, ListaSugerencias lista,
                         int pos) {
  wchar_t letras[] = L"áéíóúüabcdefghijklmnñopqrstuvwxyz";
  for (int i = 0; i < wcslen(letras) && lista->nelems < 5; i++) {
    inserta_caracter(temp, letras[i], pos);
    if (tablahash_buscar(diccionario, temp))
      lista = agregar_elemento(lista, temp);

    cola = cola_encolar(cola, temp);
    wcscpy(temp, palabra);
  }
  return cola;
}
// Dada una lista, un diccionario y una palabra que no se encuentra en el
// diccionario se generan sugerencias posibles para esa palabra intercambiando
// caracteres de lugar, insertando letras, separando la palabra, poniendo
// tildes, eliminando caracteres y reemplazando caracteres.

Cola sugerencia(wchar_t *palabra, wchar_t *temp, ListaSugerencias lista,
                TablaHash *diccionario, Cola cola) {
  int len = wcslen(palabra);
  wchar_t aux1[len + 1], aux2[len + 1];
  for (int i = 0; i <= len && lista->nelems < 5; i++) {
    // 1 (funciona bien)
    intercambiar(temp, i, i + 1);
    cola = cola_encolar(cola, temp);
    if (tablahash_buscar(diccionario, temp)) {
      lista = agregar_elemento(lista, temp);
    }
    wcscpy(temp, palabra);
    if (i < len) {
      // 2 (funciona bien)
      eliminar_caracter(temp, i);
      cola = cola_encolar(cola, temp);
      if (tablahash_buscar(diccionario, temp)) {
        lista = agregar_elemento(lista, temp);
      }
      wcscpy(temp, palabra);
      // 3  (funciona bien)
      cola = reemplazar_caracteres(palabra, temp, cola, diccionario, lista, i);
      if (tablahash_buscar(diccionario, temp)) {
        lista = agregar_elemento(lista, temp);
      }
      wcscpy(temp, palabra);
    }
    // 4 (funciona bien)
    cola = insertar_caracteres(palabra, temp, cola, diccionario, lista, i);
    if (tablahash_buscar(diccionario, temp)) {
      lista = agregar_elemento(lista, temp);
    }
    wcscpy(temp, palabra);
    // 5 separar
    separar(temp, aux1, aux2, i);
    cola = insertar_caracteres(palabra, temp, cola, diccionario, lista, i);
    if ((tablahash_buscar(diccionario, aux1) &&
         (tablahash_buscar(diccionario, aux2)))) {
      lista = agregar_elemento(lista, temp);
    }
  }
  return cola;
}

ListaSugerencias generar_sugerencia(wchar_t *palabra, ListaSugerencias *lista,
                                    TablaHash *diccionario, Cola cola) {
  int len = wcslen(palabra);
  wchar_t temp[len + 2];
  wcscpy(temp, palabra);
  // Variables temporales sólo para la funcion 'separar'.
  // wchar_t* aux1 = malloc(sizeof(wchar_t)*(len+3));
  // wchar_t* aux2 = malloc(sizeof(wchar_t)*(len+3));

  // Generamos todas las posibles sugerencias a distancia 1 y las agregamos a
  // una cola.
  cola = sugerencia(palabra, temp, lista, diccionario, cola);
  int i = 0, palabrasDistanciaUno = cola_cant_elementos(cola);
  wchar_t pal[len + 2];
  // Mientras no tengamos al menos 5 palabras correctas o
  // no hayamos terminado de verificar todas las palabras a
  // distancia 1 seguimos iterando.
  while (lista->nelems < 5 || i < palabrasDistanciaUno) {
    wprintf(L"entre al while %d %d\n", i, lista->nelems);
    // Guardamos en 'pal' el primer elemento de la cola.
    wcscpy(pal, cola_primero(cola));
    wcscpy(temp, pal);
    // Si la 'pal' está en el diccionario la agregamos a sugerencias.
    // if (tablahash_buscar(diccionario, pal)){
    //     lista = agregar_elemento(lista, pal);
    // }
    // Generamos todas las posibles sugerencias para pal y desencolamos.
    cola = sugerencia(pal, temp, lista, diccionario, cola);
    cola = cola_desencolar(cola);
    i++;
  }
  return lista;
}

void generar_sugerencias(wchar_t *palabra, ListaSugerencias lista,
                         TablaHash *diccionario, int distancia) {
  wchar_t caracteres[] = L"áéíóúüabcdefghijklmnñopqrstuvwxyz";
  int len = wcslen(palabra), len2 = wcslen(caracteres);
  // Caso base:
  if (distancia == 0) {
    if (tablahash_buscar(diccionario, palabra))
      agregar_elemento(lista, palabra);
    // wprintf(L"%d\n", tablahash_buscar(diccionario, palabra));
    // imprimir_lista(lista);
    // return lista;
  } else {
    // Primera operación: reemplazar.
    for (int pos = 0; pos < len; pos++) {
      wchar_t c = palabra[pos];
      for (int idx = 0; idx < len2; idx++) {
        reemplazar(palabra, caracteres[idx], pos);
        // wprintf(L"Realice operacion 1\n");
        generar_sugerencias(palabra, lista, diccionario, distancia - 1);
        reemplazar(palabra, c, pos);
      }
    }
    // Segunda operación: insertar.
    for (int pos = 0; pos <= len; pos++) {
      for (int idx = 0; idx < len2; idx++) {
        inserta_caracter(palabra, caracteres[idx], pos);
        // wprintf(L"Realice operacion 2\n");
        generar_sugerencias(palabra, lista, diccionario, distancia - 1);
        eliminar_caracter(palabra, pos);
      }
    }
    // Tercera operacion: eliminar.
    for (int pos = 0; pos < len; pos++) {
      wchar_t ch = palabra[pos];
      eliminar_caracter(palabra, pos);
      generar_sugerencias(palabra, lista, diccionario, distancia - 1);
      inserta_caracter(palabra, ch, pos);
    }
    // Cuarta operación: intercambiar.
    for (int pos = 0; pos < len - 1; pos++) {
      intercambiar(palabra, pos, pos + 1);
      generar_sugerencias(palabra, lista, diccionario, distancia - 1);
      intercambiar(palabra, pos, pos + 1);
    }
  }
}
// void separar(wchar_t* palabra, wchar_t* sub1, wchar_t* sub2, int pos){
//     int len = wcslen(palabra);
//     wchar_t p[len];
//     wcscpy(p, palabra);
//     //wprintf(L"Len palabra: %d\n", len);
//     int j, k = 0;
//     for(int i = len, j = 0; i >= pos; i--, j++){
//        // wprintf(L"Entre al for\n");
//         palabra[i] = palabra[i-1];
//         if(j < pos){
//            sub1[j] = p[j];
//         }
//         else{
//             sub2[k] = p[j];
//             k++;
//         }
//     }
//     //wprintf(L"Sali del for\n");
//     palabra[pos] = L' ';
//     palabra[len+1] = L'\0';
//     sub2[k+1] = L'\0';
//     sub1[pos] = L'\0';
// }

// void insertar_caracter(wchar_t* palabra, wchar_t c, int pos){
//     int len = wcslen(palabra);
//     for(int i = len; i > pos; i--){
//         palabra[i] = palabra[i-1];
//     }
//     palabra[pos] = c;
//     palabra[len+1] = L'\0';
// }

Cola reemplazar_caracteres(wchar_t *palabra, wchar_t *temp, Cola cola,
                           int pos) {
  wchar_t letras[] = L"áéíóúüabcdefghijklmnñopqrstuvwxyz";
  for (int i = 0; i < wcslen(letras); i++) {
    reemplazar(temp, letras[i], pos);
    cola = cola_encolar(cola, temp);
    wcscpy(temp, palabra);
  }
  return cola;
}

Cola insertar_caracteres(wchar_t *palabra, wchar_t *temp, Cola cola, int pos) {
  wchar_t letras[] = L"áéíóúüabcdefghijklmnñopqrstuvwxyz";
  for (int i = 0; i < wcslen(letras); i++) {
    inserta_caracter(temp, letras[i], pos);
    cola = cola_encolar(cola, temp);
    wcscpy(temp, palabra);
  }
  return cola;
}

int main() {
  setlocale(LC_ALL, "");
  wchar_t p[] = L"cajita";
  wchar_t *p1 = malloc(sizeof(wchar_t) * 15);
  wchar_t *p2 = malloc(sizeof(wchar_t) * 15);
  wchar_t *aux1 = malloc(sizeof(wchar_t) * 15);
  wchar_t *aux2 = malloc(sizeof(wchar_t) * 15);
  wcscpy(p1, p);
  wcscpy(p2, p);
  // int lon = wcslen(p1);
  // for (int i = 0; i <= lon; i++){
  //     insertar_caracter(p1, L'ü', i);
  //     wprintf(L"%ls\n", p1);
  //     wcscpy(p1, p);
  // }
  Cola cola = cola_crear();
  cola = insertar_caracteres(p1, p2, cola, 1);
  cola_imprimir(cola);

  return 0;
}