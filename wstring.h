#include <wchar.h>

// Observacion:
//              Estas funciones asumen que la cadena que reciben como argumento
//              tiene espacio suficiente para realizar las operaciones.

// Intercambia los caracteres en las posiciones i, j de la palabra.

void intercambiar(wchar_t* palabra, int i, int j);

// Elimina el caracter de la posición i de la palabra.

void eliminar_caracter(wchar_t* palabra, int i);

// Inserta en la palabra un caracter dado en una posición determinada.

void inserta_caracter(wchar_t* palabra, wchar_t c, int pos);

// Inserta en la palabra un espacio en la posición dada.

void separar(wchar_t* palabra, int pos);

// Reemplaza en la palabra el caracter en la posición dada por otro.

void reemplazar(wchar_t* palabra, wchar_t c, int pos);
