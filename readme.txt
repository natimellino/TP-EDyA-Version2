Estructuras de Datos y Algoritmos I: Trabajo práctico N° 2.

Integrantes del grupo: Francisco Alcacer, Natalia Mellino.

Compilación y ejecución del programa: 

Para compilar el programa utilizamos un makefile, para
compilar el mismo utilizamos en consola el comando "make programa", y luego para
ejecutar escribimos el comando "./programa".
Cuando el programa es ejecutado se le pide al usuario 3 ingresos por teclado:

El primero consiste en el nombre del diccionario, el segundo corresponde a
el nombre del archivo de entrada que se quiere corregir y el tercero es el
nombre del archivo de salida donde se desean escribir las sugerencias para las
palabras mal escritas del archivo de entrada.

Observación: cuando se ingresa el nombre de los 3 archivos por consola, los mismos
deben ir sin extensión '.txt', sólo debe ir el nombre del archivo. Por ejemplo si tengo
el archivo "palabras.txt" en consola sólo se debe ingresar "palabras".

Estructuras de datos elegidas para el trabajo y otras observaciones sobre el código: 

-) Para guardar las palabras del diccionario elegimos una TablaHash que consiste en
un arreglo de 1889 casillas donde en cada casilla hay un árbol binario, de esta forma,
cuando a una palabra se le asigna su índice mediante la función de hash, la palabra es 
insertada en el árbol binario y así de esta forma agilizamos la búsqueda de palabras en
el diccionario. En resumen, cuando dos o más palabras "colisionan" en la misma casilla del
arreglo de la tabla hash, resolvemos el problema insertando todas en un árbol binario.

-) Para las funciones que trabajan con strings hicimos uso de la librería "wchar.h" que nos
permitió facilitar el problema de caracteres especiales tales como las palabras acentuadas, la 'ñ', etc.

-) Idea para la generación de sugerencias:
Cuando una palabra leída desde el archivo de entrada no es encontrada en el diccionario
debemos generar distintas sugerencias posibles para esa palabra. La idea para realizar esto
es la siguiente:

Tomamos la palabra a corregir, generamos todas las posibles sugerencias insertando, reemplazando, 
intercambiando y eliminando caracteres; también agregamos espacios entre la palabra. Por cada una 
de esas sugerencias que se generan realizando las 5 operaciones, agregamos todas ellas a una cola
(implementada con listas circulares doblemente enlazadas), y al mismo tiempo preguntamos por cada 
sugerencia si la misma se encuentra presente en el diccionario. En caso de encontrarse la agregamos
a una Lista de Sugerencias. Una vez realizado este proceso tenemos en la cola todas las palabras a 
distancia 1 de la palabra a corregir. Si con todas esas sugerencias generadas no se llega a 5 sugerencias
correctas, se vuelven a generar sugerencias sobre todas esas palabras a distancia 1 y de esa forma obtenemos
las palabras a distancia 2 de la palabra a corregir. Este proceso es repetido hasta que en la Lista de 
Sugerencias haya al menos 5 palabras (si hay más de 5 sugerencias a distancia 1, todas ellas
son agregadas).

Observación: debido a la enorme cantidad de palabras que se generan con las sugerencias
es probable que suceda que si al programa se le da una palabra larga (con longitud mayor a 7 u 8 
aproximadamente), el mismo tarde en generar las sugerencias o incluso puede pasar que no se
pueda terminar la ejecución debido a la falta de memoria que es necesaria para generar y guardar
tantas palabras.

Una vez que generamos todas las sugerencias para cada palabra "incorrecta" del archivo
de entrada, las mismas son escritas en un archivo de salida indicando la línea donde se
cometió el error y las posibles sugerencias para esa palabra.