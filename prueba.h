#include <wchar.h>

typedef void (*funcionVisitante) (wchar_t* palabra, wchar_t c, int pos); 

void intercambiar(wchar_t* palabra, int i, int j);

void eliminar_caracter(wchar_t* palabra, int i);

void inserta_caracter(wchar_t* palabra, wchar_t c, int pos);

void separar(wchar_t* palabra, wchar_t* sub1, wchar_t* sub2, int pos);

void reemplazar(wchar_t* palabra, wchar_t c, int pos);

void insertar_palabra(wchar_t* palabra, wchar_t* palabraNueva, wchar_t pos);

void poner_acento(wchar_t* palabra, int pos);