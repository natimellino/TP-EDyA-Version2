#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <locale.h>

#define MAX_CADENA 20

void leer_archivo_ingreso()
{
  int i = 0;
  char c;
  char *cadena = malloc(sizeof(char) * MAX_CADENA);
  FILE *input;
  input = fopen("entrada.txt", "r");
  if (input == NULL)
  {
    printf("Error al tratar de abrir el archivo.\n");
  }
  while ((c = fgetc(input)) != EOF)
  {
    if (c != '\r' && c != ',' && c != '!' && c != '?' && c != '.' && c != ':')
    {
      if (c == ' ' || c == '\n')
      {
        *(cadena + i) = '\0';
        // char *dato = malloc(sizeof(char) * (i + 1));
        // strcpy(dato, cadena);
        printf("%s\n", cadena);
        i = 0;
      }
      else
      {
        *(cadena + i) = c;
        i++;
      }
    }
  }
}

int main()
{
  leer_archivo_ingreso();
  return 0;
}