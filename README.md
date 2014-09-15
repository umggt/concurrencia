> * **Universidad:** Mariano Gálvez de Guatemala>
> * **Catedrático:** Ing. MA. Melvin Darío Sosa Domínguez>
> * **Cátedra:** Sistemas Operativos; **Sección:** "A"; **Jornada:** Diario Vespertina>
> * **Estudiante:**
>   * **Nombre:** Miguel Eduardo Román Martínez
>   * **Carné:** 090-03-4873

# Implementación de los algoritmos de Dekker y Peterson en c++ #

[TOC]

## Introducción ##

### Acerca de los algoritmos de Dekker ###

El algoritmo de Dekker es un algoritmo de programación concurrente para exclusión mutua, que permite a dos procesos o hilos de ejecución compartir un recurso sin conflictos.

Existen 5 versiones del algoritmo de dekker de las cuales la versión 5 es la que tiene mayor eficiencia y es tomado de la unión de la version 1 y la versión 4.

### Acerca del algoritmo de Peterson ###

El algoritmo de Peterson, es un algoritmo de programación concurrente para exclusión mutua, que permite a dos o más procesos o hilos de ejecución compartir un recurso sin conflictos, utilizando sólo memoria compartida para la comunicación.

Peterson desarrolló el algoritmo como una simplificación del algoritmo de Dekker para dos procesos. Posteriormente este algoritmo fue generalizado para N procesos.

## Generalidades del código fuente del proyecto ##

El proyecto consiste en 6 aplicaciones desarrolladas en `c++ (v11)` que implementan los algoritmos de Dekker y Peterson para compartir un recurso (en este caso el buffer de escritura en pantalla) uno de los procesos escribe signos más (`+`) y el otro escribe asteriscos (`*`).

> **Nota:** Una versión digital del presente documento y el código fuente en c++ de las aplicaciones podrá encontrarse en el repositorio de github [https://github.com/miguelerm/concurrencia](https://github.com/miguelerm/concurrencia).

### Dependencias ###

Para poder dividir la pantalla de la consola en dos áreas y que cada proceso escriba en una área en particular, se utilizó la librería [ncurses](http://math.hws.edu/orr/s04/cpsc225/curses.html) que se encuentra previamente instalada en la mayoría de sistemas operativos Linux o en el caso de Windows puede utilizarse un "port" de esta librería llamado [pdcurses](http://pdcurses.sourceforge.net/).

### Compilación ###

Existe un archivo `Makefile` para poder compilar facilmente los programas solamente ubicando el shell de linux en la carpeta de cada proyecto y ejecutando el siguiente comando:

```sh
make
```

Sin embargo si se quiere compilar manualmente, el comando utilizado en linux para compilar cada una de las aplicaciones es el siguiente (en este ejemplo se compila el archivo main.cpp de la carpeta dekker1 y se genera el ejecutable `dekker1` en la carpeta build):

```sh
g++ -g -std=c++11 -o ./build/dekker1 main.cpp -pthread -lncurses
```

> **Donde**: 
> * `-g` le indica al compilador que genere información de debugging.
> * `-std=c++11` le indica al compilador que utilice la versión 11 de c++ ya que solamente en esta permite compilar con threads.
> * `-o ./build/dekker1` es la ruta destino del programa compilado, una carpeta llamada build en la misma ubicación de donde se ejecuta el programa y que genere un ejecutable llamado dekker1.
> * `main.cpp` es el nombre del archivo de código fuente.
> * `-pthread` es necesario, ya que para poder ejecutar los procedimientos en paralelo se utilizan threads.
> * `-lncurses` se utiliza para indicarle al compilador que incluya las librerias [ncurses](http://math.hws.edu/orr/s04/cpsc225/curses.html).

