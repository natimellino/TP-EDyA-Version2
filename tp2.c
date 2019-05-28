#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <assert.h>
#include "prueba.h"
#include "tablahash.h"
#include "btree.h"
#define ARR_SIZE 1889
#define MAX_CADENA 20

// Estructura para la lista de sugerencias, donde se guardan:
// -) Un arreglo de wchar_t** para almacenar palabras.
// -) La capacidad total del arreglo (capacidad).
// -) El número de elementos que tiene el arreglo hasta el momento (nelems).

typedef struct _Sugerencias{
    wchar_t** arreglo;
    int capacidad;
    int nelems;
}Sugerencias;

typedef Sugerencias* ListaSugerencias;

// Crea una lista.

ListaSugerencias crear_lista(int capacidad){
    ListaSugerencias lista = malloc(sizeof(struct _Sugerencias));
    lista->arreglo = malloc(sizeof(wchar_t*)*capacidad);
    lista->capacidad = capacidad;
    lista->nelems = 0;
    return lista; 
}

// Busca un elemento en una lista.

int buscar_elemento(ListaSugerencias lista, wchar_t* palabra){
    int i = 0, encontrado = 0;
    while(i < lista->nelems && !encontrado){
        if (wcscmp(lista->arreglo[i], palabra) == 0){
            encontrado = 1;
        }
        i++;
    }
    return encontrado;
}

// Agrega un elemento a una lista.

ListaSugerencias agregar_elemento(ListaSugerencias lista, wchar_t* string){
    if (lista->nelems < lista->capacidad-1 && !buscar_elemento(lista, string)){
        lista->arreglo[lista->nelems] = malloc(sizeof(wchar_t)*(wcslen(string)+2));
        wcscpy(lista->arreglo[lista->nelems], string);
        lista->nelems++;
    }
    return lista; 
}

// Imprime una lista.

void imprimir_lista(ListaSugerencias lista){
    if(lista->nelems > 0){
       // printf("entre al if\n");
        for(int i = 0; i < lista->nelems; i++){
           // printf("entre al for\n");
            wprintf(L"%ls\n", lista->arreglo[i]);
        }
    }
    else{
        wprintf(L"No existen sugerencias para la palabra ingresada.\n");
    }
}

// Destruye una lista.

void destruir_lista(ListaSugerencias lista){
    for(int i = 0; i < lista->capacidad; i++){
        free(lista->arreglo[i]);
    }
    free(lista->arreglo);
    free(lista);
}

/* ------------------------------------------------------------------------------------------------------------- */
// Dados dos números x, y devuelve x^y.

int potencia(int x, int y){
    int pot = 1;
    for(int i = 1; i <= y; i++){
        pot*=x;
    }
    return pot;
}

// La función hash genera una clave multiplicando el valor ASCII
// de cada caracter de la palabra por: 26^(posición del caracter).

int funcionHash(void* palabra){
    wchar_t* aux = palabra;
    int len = wcslen(aux);
    int clave = 0;
    for(int i = 0; i < len; i++){
        clave += aux[i]*potencia(26, i);
    }
    return clave;
}

// Función que recibe una tabla hash y escribe en un archivo 
// nuevo la posición del arreglo y la cantidad de elementos 
// dentro de dicha posición.

void distribucion_tabla(TablaHash* tabla){
    FILE* fp = fopen("distribucion.txt", "w+");
    for (int i = 0; i < ARR_SIZE; i++){
        fprintf(fp, "%d %d\n", i, cant_nodos(tabla->tabla[i].nodoRaiz));
    }
}

// Lee un archivo donde se encuentra guardado un diccionario y 
// guarda todas las palabras en una tabla hash convirtiéndolas
// todas a minúsculas.

void leer_diccionario(char* nombreArchivo, TablaHash* tabla){
    FILE* fp = fopen(nombreArchivo, "r");
    assert(fp != NULL);
    wchar_t* linea = malloc(sizeof(wchar_t) * 100); 
    int len, i=0;
    while(!feof(fp)){
        //wprintf(L"Entre al while\n");
        fgetws(linea, 100, fp);
        //fwscanf(fp, linea, L"%ls");
        // Convertimos las palabras a minúsculas y ponermos el '\0' al final.
        len = wcslen(linea);
        if (linea[len-1] == '\n'){
            linea[len-1] = '\0';
            len--;
        }

        //wprintf(L"palabra: %ls, long: %d\n", linea, len);
        tablahash_insertar(tabla, linea);
        //wprintf(L"Inserte palabra: %d\n", i);
        i++;
    }
   // wprintf(L"ultima linea guardada: %ls,longitud: %d\n", linea, len);
    wprintf(L"Palabras leídas: %d\n", i);
    free(linea);
    fclose(fp);
}

void leer_archivo_ingreso()
{
  int i = 0;
  wchar_t c;
  wchar_t *cadena = malloc(sizeof(wchar_t) * MAX_CADENA);
  FILE *input;
  input = fopen("entrada.txt", "r");
  if (input == NULL)
  {
    wprintf(L"Error al tratar de abrir el archivo.\n");
  }
  while ((c = fgetwc(input)) != EOF)
  {
    if (c != '\r' && c != ',' && c != '!' && c != '?' && c != '.' && c != ':')
    {
      if (c == ' ' || c == '\n')
      {
        *(cadena + i) = '\0';
        // wchar_t *dato = malloc(sizeof(wchar_t) * (i + 1));
        // strcpy(dato, cadena);
        wprintf(L"%ls\n", cadena);
        i = 0;
      }
      else
      {
        *(cadena + i) = c;
        i++;
      }
    }
  }
  fclose(input);
}

void poner_caracteres_especiales(wchar_t* palabra, wchar_t* temp, int i, TablaHash* diccionario, ListaSugerencias lista, funcionVisitante f){
                wcscpy(temp, palabra);
                f(temp, L'á', i);
                wprintf(L"%ls\n", temp);
                if(tablahash_buscar(diccionario, temp)){
                lista = agregar_elemento(lista, temp);
                }

                wcscpy(temp, palabra);
                wprintf(L"%ls\n", temp);
                f(temp, L'é', i);
                wprintf(L"%ls\n", temp);
                if(tablahash_buscar(diccionario, temp)){
                lista = agregar_elemento(lista, temp);
                }

                wcscpy(temp, palabra);
                wprintf(L"%ls\n", temp);
                f(temp, L'í', i);
                wprintf(L"%ls\n", temp);
                if(tablahash_buscar(diccionario, temp)){
                lista = agregar_elemento(lista, temp);
                }

                wcscpy(temp, palabra);
                wprintf(L"%ls\n", temp);
                f(temp, L'ó', i);
                wprintf(L"%ls\n", temp);
                if(tablahash_buscar(diccionario, temp)){
                lista = agregar_elemento(lista, temp);
                }

                wcscpy(temp, palabra);
                wprintf(L"%ls\n", temp);
                f(temp, L'ú', i);
                wprintf(L"%ls\n", temp);
                if(tablahash_buscar(diccionario, temp)){
                lista = agregar_elemento(lista, temp);
                }

                wcscpy(temp, palabra);
                wprintf(L"%ls\n", temp);
                // ñ y diresis
                f(temp, L'ñ', i);
                wprintf(L"%ls\n", temp);
                if(tablahash_buscar(diccionario, temp)){
                lista = agregar_elemento(lista, temp);
                }
                wcscpy(temp, palabra);
                wprintf(L"%ls\n", temp);
                f(temp, L'ü', i);
                wprintf(L"%ls\n", temp);
                if(tablahash_buscar(diccionario, temp)){
                lista = agregar_elemento(lista, temp);
                }
                wcscpy(temp, palabra);
}

// Dada una lista, un diccionario y una palabra que no se encuentra en el
// diccionario se generan sugerencias posibles para esa palabra intercambiando
// caracteres de lugar, insertando letras, separando la palabra, poniendo tildes,
// eliminando caracteres y reemplazando caracteres.

ListaSugerencias generar_sugerencia(wchar_t* palabra, ListaSugerencias lista, TablaHash* diccionario){
    int len = wcslen(palabra);
    wchar_t* temp = malloc(sizeof(wchar_t)*(len+1));
    wcscpy(temp, palabra);
    // Variables temporales sólo para la funcion 'separar'.
    wchar_t* aux1 = malloc(sizeof(wchar_t)*(len+3));
    wchar_t* aux2 = malloc(sizeof(wchar_t)*(len+3)); 

    for (int i = 0; i <= len; i++){
        if(i < len){
            // 1) Primera operación.
            // wprintf(L"1: %ls\n", temp);
            // intercambiar(temp, i, i+1);
            // if (tablahash_buscar(diccionario, temp)){
            //     lista = agregar_elemento(lista, temp);
            // }
            // wprintf(L"1: %ls\n", temp);
            // wcscpy(temp, palabra);
            // wprintf(L"1: %ls\n", temp);
            //wprintf(L"1: %ls\n", temp);
            //2) Segunda operación.
            // eliminar_caracter(temp, i);
            // wprintf(L"2: %ls\n", temp); 
            // if (tablahash_buscar(diccionario, temp)){
            //     lista = agregar_elemento(lista, temp);
            // }
            // wcscpy(temp, palabra);
            // wprintf(L"2: %ls\n", temp); 
            // 3) Tercera operación.
            // separar(temp, aux1, aux2, i);
            // wprintf(L"3: %ls\n", temp);
            // //int a = wcslen(aux1);
            // wprintf(L"3: %ls\n", aux1);
            // wprintf(L"3: %ls\n\n", aux2);
            // //printf("%s, %s, %s\n", temp, aux1, aux2);
            // if (tablahash_buscar(diccionario, aux1) && tablahash_buscar(diccionario, aux2)){
            //     lista = agregar_elemento(lista, temp);
            }
            // wcscpy(temp, palabra);
            // wprintf(L"3: %ls\n", temp);
            //poner_caracteres_especiales(palabra, temp, i, diccionario, lista, reemplazar);
            //poner_caracteres_especiales(palabra, temp, i, diccionario, lista, poner_caracter);
            wcscpy(temp, palabra);
        // }
        wchar_t letras[] = L"áéíóúüabcdefghijklmnñopqrstuvwxyz";
         for (int j = 0; j < wcslen(letras); j++){
            wcscpy(temp, palabra);
        //     // 5) Quinta operación.
        //     if (i <= len){
        //         reemplazar(temp, j, i);
        //        // wprintf(L"%ls\n", temp);
        //         if(tablahash_buscar(diccionario, temp)){
        //         lista = agregar_elemento(lista, temp);
        //         }
        //         wcscpy(temp, palabra);
        //         // Ponemos las tildes, ñ y dieresis.

                
        //     }
        //     wcscpy(temp, palabra);
        //     // 6) Sexta operación.
            poner_caracter(temp, letras[j], i);
            wprintf(L"%ls\n", temp);
             if(tablahash_buscar(diccionario, temp)){
                 lista = agregar_elemento(lista, temp);
            }
         }
        
    }
 }
    free(temp);
    free(aux1);
    free(aux2);
    return lista;
}

int main(){
    setlocale(LC_ALL, "");
    char diccionario[30];
    wprintf(L"Nombre del diccionario (no más de 30 caracteres):\n");
    scanf("%s", diccionario);
    strcat(diccionario, ".txt");

    TablaHash* palabras = tablahash_crear(ARR_SIZE, funcionHash);
    leer_diccionario(diccionario, palabras);
    distribucion_tabla(palabras);

    // wchar_t* p1 = malloc(sizeof(wchar_t)*20);
    // wchar_t* p2 = malloc(sizeof(wchar_t)*20);
    // FILE* f = fopen("hola.txt", "r");
    // fgetws(p1, 20, f);
    // int l = wcslen(p1);
    // p1[l-1] = '\0';
    // wcscpy(p2, p1);
    // fclose(f);
    //int l = wcslen(p1);

    //wcscpy(p1, (wchar_t*)palabras->tabla[11].nodoRaiz->dato);
    //wprintf(L"%ls %d\n\n", p1, l);
    //reemplazar(p1, L'í', 5);
    wchar_t p[] = L"cancion";
    int l1 = wcslen(p);
    wprintf(L"%ls %d\n\n", p, l1);
    //wprintf(L"%ls", p1);

    // BORRAR DESPUÉS.
    // wchar_t p[] = "lacanción";
    // wchar_t* p1 = malloc(sizeof(wchar_t)*11);
    // strcpy((wchar_t*) p1, p);

    ListaSugerencias sugerencias = crear_lista(50);
    sugerencias = generar_sugerencia(p, sugerencias, palabras);
    wprintf(L"\n\n");
    wprintf(L"Sugerencias para %ls:\n", p);
    imprimir_lista(sugerencias);

    /*wchar_t textoEntrada[30];
    printf("Nombre del archivo de texto (no más de 30 caracteres):\n");
    scanf("%s", textoEntrada);
    strcat(textoEntrada, ".txt");*/
    
    tablahash_destruir(palabras);
    return 0;
}