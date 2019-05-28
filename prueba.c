#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <ctype.h>
#include <assert.h>
#include <locale.h>


// Intercambia las posiciones i, j de la palabra dada.

void intercambiar(wchar_t* palabra, int i, int j){
    int len = wcslen(palabra);
    if (i < len && j < len){
        wchar_t temp = palabra[i];
        palabra[i] = palabra[j];
        palabra[j] = temp; 
    }
}

// Elimina el caracter de la posicion i.

void eliminar_caracter(wchar_t* palabra, int i){
    int len = wcslen(palabra);
    int j;
    for(j = i; j < len; j++){
        palabra[j] = palabra[j+1];
    }
    palabra[len-1] = '\0';
}

// Agrega al caracter c en la posición pos.

void poner_caracter(wchar_t* palabra, wchar_t c, int pos){
    int len = wcslen(palabra);
    for(int i = len; i >= pos; i--){
        palabra[i] = palabra[i-1];
    }
    palabra[pos] = c;
    wprintf(L"p[pos]: %lc\n", palabra[pos]);
}
// Separa la palabra en dos agregando un espacio en palabra[pos].

void separar(wchar_t* palabra, wchar_t* sub1, wchar_t* sub2, int pos){
    int len = wcslen(palabra);
    if (pos != 0 && pos != len){

        int i, j = 0;
        for (i = 0; i < len; i++){
            wprintf(L"i: %d\n", i);
            if (i < pos){
                sub1[i] = palabra[i];
                wprintf(L"%lc\n", palabra[i]);
            }   
            if (i >= pos && j < len - pos){
                sub2[j] = palabra[i];
                j++;
            } 
        }
        //palabra[len] = '\0';
        wprintf(L"%ls, %ls\n", sub1, sub2);
        sub1[i-j] = '\0';
        sub2[j] = '\0';
        //poner_caracter(palabra, ' ', pos);
        
    } 
}

// Reemplaza el caracter de la posición pos de la palabra por el char c.

void reemplazar(wchar_t* palabra, wchar_t c, int pos){
    palabra[pos] = c;
} 

// Inserta una palabra en otra dada, en una determinada posición.

void insertar_palabra(wchar_t* palabra, wchar_t* palabraNueva, int pos){
    int len1 = wcslen(palabra);
    int len2 = wcslen(palabraNueva);
    //palabra = realloc(palabra, sizeof(char)*(len1+len2));
    for (int i = len1+len2; i >= pos; i--){
        palabra[i] = palabra[i-len2];
    }
    for(int j = pos, k = 0; k < len2; j++, k++){
        palabra[j] = palabraNueva[k];
    }
}

//void poner_acento(wchar_t* palabra, int pos, ListaSugerencias lista)

// void poner_acento(wchar_t* palabra, int pos){
//     wchar_t a='á', e='é', i='í', o='ó', u='ú', n='ñ';
//     if (pos < wcslen(palabra)){
//         wchar_t caracter = palabra[pos];
//         switch(caracter){
//             case 
//         }
//     }
// }


// Acentúa una palabra en una posición dada, si es que el caracter en esa
// posición es una vocal, sino, no hace nada.

// void poner_acento(char* palabra, int pos){
//     if (pos < strlen(palabra)){
//         char vocal = palabra[pos];
//         char a[] = "á", e[] = "é", i[] = "í", o[] = "ó", u[] = "ú";
//         switch(vocal){
//         case 'a':
//             eliminar_caracter(palabra, pos);
//             insertar_palabra(palabra, a, pos); 
//             break;
//         case 'e':
//             eliminar_caracter(palabra, pos);
//             insertar_palabra(palabra, e, pos);
//             break;
//         case 'i':
//             eliminar_caracter(palabra, pos);
//             insertar_palabra(palabra, i, pos);   
//             break;
//         case 'o':
//             eliminar_caracter(palabra, pos);
//             insertar_palabra(palabra, o, pos);
//             break;
//         case 'u':
//             eliminar_caracter(palabra, pos);
//             insertar_palabra(palabra, u, pos);
//             break;
//         }
//     }
// }

