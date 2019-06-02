#include <assert.h>
#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

// Intercambia las posiciones i, j de la palabra dada.

void intercambiar(wchar_t* palabra, int i, int j) {
  if (palabra[i] != palabra[j]) {
    int len = wcslen(palabra);
    if (i < len && j < len) {
      wchar_t temp = palabra[i];
      palabra[i] = palabra[j];
      palabra[j] = temp;
    }
  }
}

// Elimina el caracter de la posicion i.

void eliminar_caracter(wchar_t* palabra, int i) {
  int len = wcslen(palabra);
  int j;
  for (j = i; j < len; j++) {
    palabra[j] = palabra[j + 1];
  }
  palabra[len - 1] = '\0';
}

void reemplazar(wchar_t* palabra, wchar_t c, int pos) {
  if (palabra[pos] != c) {
    palabra[pos] = c;
  }
}
// Agrega al caracter c en la posición pos.

void inserta_caracter(wchar_t* palabra, wchar_t c, int pos) {
  int len = wcslen(palabra);
  if (palabra[pos] != c)
    for (int i = len; i > pos; i--) {
      palabra[i] = palabra[i - 1];
    }
  palabra[pos] = c;
  palabra[len + 1] = L'\0';
}
// Separa la palabra en dos agregando un espacio en palabra[pos].

void separar(wchar_t* palabra, int pos) {
  int len = wcslen(palabra);
  if (pos > 0 && pos < len && palabra[pos] != ' ') {
    inserta_caracter(palabra, ' ', pos);
  }
}
