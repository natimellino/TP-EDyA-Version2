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
#define ARR_SIZE 1889
#define MAX_CADENA 20

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

// Crea una lista.

ListaSugerencias crear_lista(int capacidad) {
  ListaSugerencias lista = malloc(sizeof(struct _Sugerencias));
  lista->arreglo = malloc(sizeof(wchar_t *) * capacidad);
  lista->capacidad = capacidad;
  lista->nelems = 0;
  return lista;
}

// Busca un elemento en una lista.

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

// Agrega un elemento a una lista.

ListaSugerencias agregar_elemento(ListaSugerencias lista, wchar_t *string) {
  if (lista->nelems < lista->capacidad - 1 && !buscar_elemento(lista, string)) {
    lista->arreglo[lista->nelems] =
        malloc(sizeof(wchar_t) * (wcslen(string) + 2));
    wcscpy(lista->arreglo[lista->nelems], string);
    lista->nelems++;
  }
  return lista;
}

// Imprime una lista.

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

// Destruye una lista.

void destruir_lista(ListaSugerencias lista) {
  for (int i = 0; i < lista->capacidad; i++) {
    free(lista->arreglo[i]);
  }
  free(lista->arreglo);
  free(lista);
}

/* -------------------------------------------------------------------------------------------------------------
 */
// Dados dos números x, y devuelve x^y.

int potencia(int x, int y) {
  int pot = 1;
  for (int i = 1; i <= y; i++) {
    pot *= x;
  }
  return pot;
}

// La función hash genera una clave multiplicando el valor ASCII
// de cada caracter de la palabra por: 26^(posición del caracter).

int funcionHash(void *palabra) {
  wchar_t *aux = palabra;
  int len = wcslen(aux);
  int clave = 0;
  for (int i = 0; i < len; i++) {
    clave += aux[i] * potencia(26, i);
  }
  return clave;
}

// Función que recibe una tabla hash y escribe en un archivo
// nuevo la posición del arreglo y la cantidad de elementos
// dentro de dicha posición.

void distribucion_tabla(TablaHash *tabla) {
  FILE *fp = fopen("distribucion.txt", "w+");
  for (int i = 0; i < ARR_SIZE; i++) {
    fprintf(fp, "%d %d\n", i, cant_nodos(tabla->tabla[i].nodoRaiz));
  }
}

// Lee un archivo donde se encuentra guardado un diccionario y
// guarda todas las palabras en una tabla hash convirtiéndolas
// todas a minúsculas.

void leer_diccionario(char *nombreArchivo, TablaHash *tabla) {
  FILE *fp = fopen(nombreArchivo, "r");
  assert(fp != NULL);
  wchar_t *linea = malloc(sizeof(wchar_t) * 100);
  int len, i = 0;
  while (!feof(fp)) {
    // wprintf(L"Entre al while\n");
    fgetws(linea, 100, fp);
    // fwscanf(fp, linea, L"%ls");
    // Convertimos las palabras a minúsculas y ponermos el '\0' al final.
    len = wcslen(linea);
    if (linea[len - 1] == '\n') {
      linea[len - 1] = '\0';
      len--;
    }

    // wprintf(L"palabra: %ls, long: %d\n", linea, len);
    tablahash_insertar(tabla, linea);
    // wprintf(L"Inserte palabra: %d\n", i);
    i++;
  }
  // wprintf(L"ultima linea guardada: %ls,longitud: %d\n", linea, len);
  wprintf(L"Palabras leídas: %d\n", i);
  free(linea);
  fclose(fp);
}

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

ListaSugerencias generar_sugerencia(wchar_t *palabra, ListaSugerencias lista,
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
    // wprintf(L"entre al while %d %d\n", i, lista->nelems);
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

void leer_archivo_ingreso() {
  int i = 0;
  wchar_t c;
  wchar_t *cadena = malloc(sizeof(wchar_t) * MAX_CADENA);
  FILE *input;
  input = fopen("entrada.txt", "r");
  if (input == NULL) {
    wprintf(L"Error al tratar de abrir el archivo.\n");
  }
  while ((c = fgetwc(input)) != EOF) {
    if (c != '\r' && c != ',' && c != '!' && c != '?' && c != '.' && c != ':') {
      if (c == ' ' || c == '\n') {
        *(cadena + i) = '\0';
        // wchar_t *dato = malloc(sizeof(wchar_t) * (i + 1));
        // strcpy(dato, cadena);
        wprintf(L"%ls\n", cadena);
        i = 0;
      } else {
        *(cadena + i) = c;
        i++;
      }
    }
  }
  fclose(input);
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

int main() {
  setlocale(LC_ALL, "");
  char diccionario[30];
  wprintf(L"Nombre del diccionario (no más de 30 caracteres):\n");
  scanf("%s", diccionario);
  strcat(diccionario, ".txt");

  TablaHash *palabras = tablahash_crear(ARR_SIZE, funcionHash);
  leer_diccionario(diccionario, palabras);
  // distribucion_tabla(palabras);

  // wchar_t* p2 = malloc(sizeof(wchar_t)*20);
  // FILE* f = fopen("hola.txt", "r");
  // fgetws(p1, 20, f);
  // int l = wcslen(p1);
  // p1[l-1] = '\0';
  // wcscpy(p2, p1);
  // fclose(f);
  // int l = wcslen(p1);

  // wprintf(L"%ls %d\n\n", p1, l);
  // reemplazar(p1, L'í', 5);
  wchar_t p[] = L"lvandx";
  wchar_t p1[30];
  wcscpy(p1, p);

  wprintf(L"Lectura finalizada.\n");
  // fclose(fp);

  ListaSugerencias sugerencias = crear_lista(100);
  Cola cola = cola_crear();
  wprintf(L"cree la lista de sugerencias\n");
  // cola = sugerencia(p, p1, cola);
  // generar_sugerencias(p1, sugerencias, palabras, 6);
  wprintf(L"\n\n");
  // wprintf(L"Sugerencias para %ls:\n", linea);
  // cola_imprimir(cola);
  sugerencias = generar_sugerencia(p1, sugerencias, palabras, cola);
  imprimir_lista(sugerencias);

  /*wchar_t textoEntrada[30];
  printf("Nombre del archivo de texto (no más de 30 caracteres):\n");
  scanf("%s", textoEntrada);
  strcat(textoEntrada, ".txt");*/

  // tablahash_destruir(palabras);
  return 0;
}