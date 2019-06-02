#include <assert.h>
#include <ctype.h>
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include "btree.h"
#include "colas.h"
#include "tablahash.h"
#include "wstring.h"
#define ARR_SIZE 1889
#define MAX_CADENA 30

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
  for (int i = 0; i < lista->nelems; ++i) {
    free(lista->arreglo[i]);
  }
  free(lista->arreglo);
  free(lista);
}
//------------------------------------------------------------------------------

// Estructura auxiliar donde se va a guardar una palabra a corregir,
// la línea del archivo en la que se encuentra y su lista de sugrencias
// con palabras correctas.

typedef struct _Palabra {
  int linea;
  wchar_t *palabra;
  ListaSugerencias sugerencia;
} Palabra1;

typedef Palabra1 *Palabra;

Palabra crear_palabra(wchar_t *palabra, int linea) {
  Palabra pal = malloc(sizeof(struct _Palabra));
  pal->linea = linea;
  pal->palabra = malloc(sizeof(wchar_t) * (wcslen(palabra) + 1));
  wcscpy(pal->palabra, palabra);
  pal->sugerencia = crear_lista(100);
  return pal;
}

void palabra_destruir(Palabra palabra) {
  free(palabra->palabra);
  destruir_lista(palabra->sugerencia);
  free(palabra);
}

/* -----------------------------------------------------------------------------
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
  fclose(fp);
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
  wprintf(L"Diccionario cargado: palabras leídas: %d\n", i);
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

int buscar_palabra_espaciada(wchar_t *palabra, TablaHash *diccionario) {
  int esta = 1;
  wchar_t *aux;
  wchar_t *buffer;
  aux = wcstok(palabra, L" ", &buffer);
  while (aux != NULL) {
    esta *= tablahash_buscar(diccionario, aux);
    aux = wcstok(NULL, L" ", &buffer);
  }
  return esta;
}

// Dada una lista, un diccionario y una palabra que no se encuentra en el
// diccionario se generan sugerencias posibles para esa palabra intercambiando
// caracteres de lugar, insertando letras, separando la palabra, poniendo
// tildes, eliminando caracteres y reemplazando caracteres.

Cola sugerencia(wchar_t *palabra, wchar_t *temp, ListaSugerencias lista,
                TablaHash *diccionario, Cola cola, int vuelta) {
  int len = wcslen(palabra);
  // wchar_t c1;
  for (int i = 0; i <= len && (lista->nelems < 5 || vuelta == 1); i++) {
    // 1) Intercambiar caracteres.
    intercambiar(temp, i, i + 1);
    cola = cola_encolar(cola, temp);
    if (tablahash_buscar(diccionario, temp)) {
      lista = agregar_elemento(lista, temp);
    }  // Revierto la operación.
    wcscpy(temp, palabra);
    if (i < len) {
      // 2) Eliminar caracteres.
      eliminar_caracter(temp, i);
      cola = cola_encolar(cola, temp);
      if (tablahash_buscar(diccionario, temp)) {
        lista = agregar_elemento(lista, temp);
      }  // Revierto la operación.
      wcscpy(temp, palabra);
      // 3) Reemplazar caracteres.
      cola = reemplazar_caracteres(palabra, temp, cola, diccionario, lista, i);
      if (tablahash_buscar(diccionario, temp)) {
        lista = agregar_elemento(lista, temp);
      }
      wcscpy(temp, palabra);
    }
    // 4) Insertar caracteres.
    cola = insertar_caracteres(palabra, temp, cola, diccionario, lista, i);
    if (tablahash_buscar(diccionario, temp)) {
      lista = agregar_elemento(lista, temp);
    }
    wcscpy(temp, palabra);
    // 5) Separar la palabra en 2.
    separar(temp, i);
    cola = cola_encolar(cola, temp);
    if (buscar_palabra_espaciada(temp, diccionario)) {
      lista = agregar_elemento(lista, temp);
    }
    wcscpy(temp, palabra);
  }
  return cola;
}

ListaSugerencias generar_sugerencia(wchar_t *palabra, ListaSugerencias lista,
                                    TablaHash *diccionario) {
  Cola cola = cola_crear();
  int len = wcslen(palabra);
  wchar_t temp[len + 2];
  wcscpy(temp, palabra);
  // Generamos todas las posibles sugerencias a distancia 1 y las agregamos a
  // una cola.
  cola = sugerencia(palabra, temp, lista, diccionario, cola, 1);
  cola_imprimir(cola);
  int i = 0;
  wchar_t pal[len + 2];
  // Si no llegamos a 5 sugerencias con las palabras a distancia 1 seguimos,
  // generando con las palabras a una distancia mayor hasta que consigamos 5
  // sugerencias válidas.
  while (lista->nelems < 5) {
    // Guardamos en 'pal' el primer elemento de la cola.
    wcscpy(pal, cola_primero(cola));
    wcscpy(temp, pal);
    // Generamos todas las posibles sugerencias para pal y desencolamos.
    cola = sugerencia(pal, temp, lista, diccionario, cola, 2);
    cola = cola_desencolar(cola);
    i++;
  }
  cola_destruir(cola);
  return lista;
}

int es_alfabetica(wchar_t *cadena) {
  int i = 0, len = wcslen(cadena), esAlfa = 1;
  while (i < len && esAlfa) {
    if (iswalpha(cadena[i]) == 0) {
      esAlfa = 0;
    }
    i++;
  }
  return esAlfa;
}

void leer_archivo_ingreso(TablaHash *diccionario, char *nombreArchivo,
                          char *archivoSalida) {
  int i = 0, linea = 1;
  wchar_t c;
  wchar_t *cadena = malloc(sizeof(wchar_t) * MAX_CADENA);
  FILE *input;
  input = fopen(nombreArchivo, "r");
  FILE *fp = fopen(archivoSalida, "w+");
  assert(input != NULL);
  // int n;
  while ((c = fgetwc(input)) != EOF) {
    c = towlower(c);
    // if (c != '\r' && c != ',' && c != '!' && c != '?' && c != '.' && c !=
    // ':') {
    if (c == ' ' || c == '\n') {  // Termino de leer palabra
      *(cadena + i) = '\0';
      // n = wcslen(cadena);
      if (iswalpha(c)) c = fgetwc(input);
      // wprintf(L"cadena: %ls %d\n", cadena, n);

      // Por cada palabra leída preguntamos si se encuentra en
      // el diccionario, en caso de que no esté le generamos sugerencias.
      if (es_alfabetica(cadena) && wcslen(cadena) > 0) {
        if (!tablahash_buscar(diccionario, cadena)) {
          Palabra pal = crear_palabra(cadena, linea);
          pal->sugerencia =
              generar_sugerencia(cadena, pal->sugerencia, diccionario);
          fwprintf(fp,
                   L"Línea %d: %ls no está en el diccionario, quizás quizo "
                   L"decir:\n",
                   pal->linea, pal->palabra);
          for (int i = 0; i < pal->sugerencia->nelems; i++) {
            fwprintf(fp, L"%ls\n", pal->sugerencia->arreglo[i]);
          }
          palabra_destruir(pal);
        }
      }

      if (c == '\n') linea++;
      i = 0;
    } else {  // sigue leyendo caracteres
      *(cadena + i) = c;
      i++;
    }
    //}
  }
  free(cadena);
  fclose(input);
  fclose(fp);
}

int main() {
  setlocale(LC_ALL, "");
  char diccionario[30];
  wprintf(L"Nombre del diccionario (no más de 30 caracteres):\n");
  scanf("%s", diccionario);
  strcat(diccionario, ".txt");

  TablaHash *palabras = tablahash_crear(ARR_SIZE, funcionHash);
  leer_diccionario(diccionario, palabras);

  wchar_t p[] = L"hola";
  // wchar_t p1[30];
  // wcscpy(p1, p);
  char textoEntrada[30], archivoSalida[30];
  wprintf(L"Nombre del archivo de entrada (no más de 30 caracteres):\n");
  scanf("%s", textoEntrada);
  strcat(textoEntrada, ".txt");
  wprintf(L"Nombre del archivo de salida (no más de 30 caracteres):\n");
  scanf("%s", archivoSalida);
  strcat(archivoSalida, ".txt");
  leer_archivo_ingreso(palabras, textoEntrada, archivoSalida);
  tablahash_destruir(palabras);
  separar(p, 1);
  wprintf(L"%ls\n", p);
  return 0;
}