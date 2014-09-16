> * **Universidad:** Mariano Gálvez de Guatemala
> * **Catedrático:** Ing. MA. Melvin Darío Sosa Domínguez
> * **Cátedra:** Sistemas Operativos; **Sección:** "A"; **Jornada:** Diario Vespertina
> * **Estudiante:**
>   * **Nombre:** Miguel Eduardo Román Martínez
>   * **Carné:** 090-03-4873

# Implementación de los algoritmos de Dekker y Peterson en c++ #

[TOC]

## Introducción ##

> **Nota:** Una versión digital del presente documento y el código fuente en c++ de las aplicaciones podrá encontrarse en el repositorio de github [https://github.com/miguelerm/concurrencia](https://github.com/miguelerm/concurrencia).

### Acerca de los algoritmos de Dekker ###

El algoritmo de Dekker es un algoritmo de programación concurrente para exclusión mutua, que permite a dos procesos o hilos de ejecución compartir un recurso sin conflictos.

Existen 5 versiones del algoritmo de dekker de las cuales la versión 5 es la que tiene mayor eficiencia y es tomado de la unión de la versión 1 y la versión 4.

### Acerca del algoritmo de Peterson ###

El algoritmo de Peterson, es un algoritmo de programación concurrente para exclusión mutua, que permite a dos o más procesos o hilos de ejecución compartir un recurso sin conflictos, utilizando solo memoria compartida para la comunicación.

Peterson desarrolló el algoritmo como una simplificación del algoritmo de Dekker para dos procesos. Posteriormente este algoritmo fue generalizado para N procesos.

## Generalidades del código fuente del proyecto ##

El proyecto consiste en 6 aplicaciones desarrolladas en `c++ (v11)` que implementan los algoritmos de Dekker y Peterson para compartir un recurso (en este caso el buffer de escritura en pantalla) uno de los procesos escribe signos más (`+`) y el otro escribe asteriscos (`*`).

### Dependencias ###

Para poder dividir la pantalla de la consola en dos áreas y que cada proceso escriba en una área en particular, se utilizó la librería [ncurses](http://math.hws.edu/orr/s04/cpsc225/curses.html) que se encuentra previamente instalada en la mayoría de sistemas operativos Linux o en el caso de Windows puede utilizarse un "port" de esta librería llamado [pdcurses](http://pdcurses.sourceforge.net/).

### Compilación ###

Existe un archivo `Makefile` para poder compilar fácilmente los programas solamente ubicando el shell de linux en la carpeta de cada proyecto y ejecutando el siguiente comando:

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

## Algoritmos de Dekker ##

## Dekker I ##

La primer versión del algoritmo de Dekker es llamado **Alternancia Estricta**, es llamado de esta manera ya que obliga a que cada proceso tenga un turno, osea que hay un cambio de turno cada vez que un proceso sale de la sección crítica, por lo tanto si un proceso es lento atrasara a otros procesos que son rápidos.

### Características ###

* Garantiza la exclusión mutua
* Su sincronización es forzada
* Acopla fuertemente a los procesos (procesos lentos atrasan a procesos rápidos)
* No garantiza la progresión, ya que si un proceso por alguna razón es bloqueado dentro o fuera de la sección puede bloquear a los otros procesos.

### Implementación en c++ ###

```c++

#include <cstdlib>
#include <iostream>
#include <thread>
#include <time.h>
#include <curses.h>
#include <stdio.h>
#include <string.h>

using namespace std;

int turno;
bool cancelar;

/* lineas eliminadas para facilitar lectura */

void ejecutar_seccion_critica_1() {
    /* lineas eliminadas para facilitar lectura */
}

void proceso1() {
    while (!cancelar) {
        while (turno == 2 && !cancelar) {
            /* esperar */
        }
        if (cancelar) break;
        ejecutar_seccion_critica_1();
        turno = 2;
    }
}

void ejecutar_seccion_critica_2() {
    /* lineas eliminadas para facilitar lectura */
}

void proceso2() {
    while (!cancelar) {
        while (turno == 1 && !cancelar) {
            /* esperar */
        }
        if (cancelar) break;
        ejecutar_seccion_critica_2();
        turno = 1;
    }
}

/* lineas eliminadas para facilitar lectura */

int main(int argc, char** argv) {

	// Genera un número aleatorio para definir cual es el turno inicial.
    srand(time(NULL));
    cancelar = false;
    turno = rand() % 100 + 1 <= 50 ? 1 : 2;

	// Se inician los dos hilos para ejecutar proceso1 y proceso2 en paralelo.
    thread p1(proceso1);
    thread p2(proceso2);

    // Se espera hasta que el usuario decida finalizar el programa.
    esperar_enter_del_usuario();

    cancelar = true;

	// Se espera a que finalicen los procesos.
    p1.join();
    p2.join();

    return EXIT_SUCCESS;
}
```

### Descripción ###

* Cuando un proceso es ejecutado verifica si es su turno, si no es su turno se queda en espera por medio de un ciclo while.
* De lo contrario si es su turno avanza a la sección crítica.
* Cuando el proceso sale de la sección crítica cambia de turno.
* Se incluye una variable `cancelar` que se establece a `true` cuando el usuario presiona la tecla `Enter` para poder finalizar los procesos en cualquier momento.


## Dekker II ##

Segunda versión del algoritmo de Dekker es llamado Problema de Interbloqueo, su nombre se debe a que si en cada ráfaga de CPU, cada proceso queda en el mismo estado, en el estado donde se le asigna que puede entrar a la sección crítica. Entonces estando los dos procesos con opción a entrar, a la siguiente ráfaga de CPU ambos procesos verificaran si el proceso alterno puede entrar, viendo que el proceso alterno tiene la opción de entrar, los procesos quedan bloqueados ya que se quedaran enciclados bloqueándose mutuamente ya que no podrán entrar nunca a la sección crítica.

### Características ###

* Garantiza la exclusión mutua
* No garantiza espera limitada

### Implementación en c++ ###

```c++
#include <cstdlib>
#include <iostream>
#include <thread>
#include <time.h>
#include <curses.h>
#include <stdio.h>
#include <string.h>

using namespace std;

bool proceso1_puede_entrar;
bool proceso2_puede_entrar;
bool cancelar;

/* lineas eliminadas para facilitar lectura */

void ejecutar_seccion_critica_1() {
    /* lineas eliminadas para facilitar lectura */
}

void proceso1() {
    while (!cancelar) {
        proceso1_puede_entrar = true;
        while (proceso2_puede_entrar && !cancelar) {
            /* esperar */
        }
        if (cancelar) break;
        ejecutar_seccion_critica_1();
        proceso1_puede_entrar = false;
    }
}

void ejecutar_seccion_critica_2() {
    /* lineas eliminadas para facilitar lectura */
}

void proceso2() {
    while (!cancelar) {
        proceso2_puede_entrar = true;
        while (proceso1_puede_entrar && !cancelar) {
            /* esperar */
        }
        if (cancelar) break;
        ejecutar_seccion_critica_2();
        proceso2_puede_entrar = false;
    }
}

/* lineas eliminadas para facilitar lectura */

int main(int argc, char** argv) {

	// se inicializan las variables.
    cancelar = false;
    proceso1_puede_entrar = false;
    proceso2_puede_entrar = false;

	// se genera dos hilos para ejecutar los procesos en paralelo.
    thread p1(proceso1);
    thread p2(proceso2);

	// se espera a que el usuario finalice la ejecución.
    esperar_enter_del_usuario();

    cancelar = true;

	// se espera a que finalicen los procesos.
    p1.join();
    p2.join();

    return EXIT_SUCCESS;
}
```

### Descripción ###

* Cuando un proceso puede entrar verifica si otro proceso tiene la opción de poder entrar, si otro proceso también tiene la opción de poder entrar se da un interbloqueo. De lo contrario el proceso avanza a la sección crítica.
* Al salir de la sección crítica el proceso cambia su opción. Y permite al otro proceso avanzar a la sección crítica.
* Se incluye una variable `cancelar` que se establece a `true` cuando el usuario presiona la tecla `Enter` para poder finalizar los procesos en cualquier momento.


## Dekker III ##

La Tercera versión del algoritmo de Dekker es llamado Colisión región crítica no garantiza la exclusión mutua, como su nombre lo indica se da una colisión en la región crítica por la forma en que son colocados por así decirlo los permisos, ya que primero se comprueba si otro proceso está dentro y luego se indica que el proceso en el que se está actualmente cambia diciendo que está dentro. Y el problema se da cuando los procesos después de haber tenido sus ráfagas de CPU pasan de la fase de comprobación y se tiene libre el camino para entrar a la región crítica, generando esto una colisión.

### Características ###

* No garantiza la exclusión mutua
* Colisión en la región crítica

### Implementación en c++ ###

```c++
#include <cstdlib>
#include <iostream>
#include <thread>
#include <time.h>
#include <curses.h>
#include <stdio.h>
#include <string.h>

using namespace std;

bool proceso1_esta_dentro;
bool proceso2_esta_dentro;
bool cancelar;

/* lineas eliminadas para facilitar lectura */

void ejecutar_seccion_critica_1() {
    /* lineas eliminadas para facilitar lectura */
}

void proceso1() {
    while (!cancelar) {
        while (proceso2_esta_dentro && !cancelar) {
            /* esperar */
        }
        proceso1_esta_dentro = true;
        if (cancelar) break;
        ejecutar_seccion_critica_1();
        proceso1_esta_dentro = false;
    }
}

void ejecutar_seccion_critica_2() {
    /* lineas eliminadas para facilitar lectura */
}

void proceso2() {
    while (!cancelar) {
        while (proceso1_esta_dentro && !cancelar) {
            /* esperar */
        }
        proceso2_esta_dentro = true;
        if (cancelar) break;
        ejecutar_seccion_critica_2();
        proceso2_esta_dentro = false;
    }
}

/* lineas eliminadas para facilitar lectura */

int main(int argc, char** argv) {

    cancelar = false;
    proceso1_esta_dentro = false;
    proceso2_esta_dentro = false;

	// Se crean dos hilos para ejecutar los procesos de forma concurrente.
    thread p1(proceso1);
    thread p2(proceso2);

	// se espera hasta que el usuario presione Enter.
    esperar_enter_del_usuario();

    cancelar = true;

	// Se espera a que finalicen los procesos.
    p1.join();
    p2.join();

    return EXIT_SUCCESS;
}
```

### Descripción ###

* Al ejecutarse el proceso, verifica si otro proceso está dentro de la sección critica.
* Si el otro proceso está dentro entonces espera a que salga de la sección critica. De lo contrario pasa la fase de comprobación y cambia su estado a que está dentro.
* Luego de pasar la sección critica cambia su estado.
* Se incluye una variable `cancelar` que se establece a `true` cuando el usuario presiona la tecla `Enter` para poder finalizar los procesos en cualquier momento.


## Dekker IV ##

Cuarta versión del algoritmo de Dekker es llamado Postergación Indefinida, su nombre se debe a que en una parte del código es colocado un retardo con un tiempo aleatorio, y el retardo puede ser muy grande que no se sabe hasta cuándo entrara a la sección critica.

### Características ###

* Garantiza la exclusión mutua.
* Un proceso o varios se quedan esperando a que suceda un evento que tal vez nunca suceda.

### Implementación en c++ ###

```c++
#include <cstdlib>
#include <iostream>
#include <thread>
#include <time.h>
#include <curses.h>
#include <stdio.h>
#include <string.h>

using namespace std;

bool proceso1_puede_entrar;
bool proceso2_puede_entrar;
bool cancelar;

/* lineas eliminadas para facilitar lectura */

void ejecutar_seccion_critica_1() {
    /* lineas eliminadas para facilitar lectura */
}

void proceso1() {
    while (!cancelar) {
        proceso1_puede_entrar = true;
        while (proceso2_puede_entrar && !cancelar) {
            proceso1_puede_entrar = false;
            retardo_aleatoreo();
            proceso1_puede_entrar = true;
        }
        if (cancelar) break;
        ejecutar_seccion_critica_1();
        proceso1_puede_entrar = false;
    }
}

void ejecutar_seccion_critica_2() {
    /* lineas eliminadas para facilitar lectura */
}

void proceso2() {
    while (!cancelar) {
        proceso2_puede_entrar = true;
        while (proceso1_puede_entrar && !cancelar) {
            proceso2_puede_entrar = false;
            retardo_aleatoreo();
            proceso2_puede_entrar = true;
        }
        if (cancelar) break;
        ejecutar_seccion_critica_2();
        proceso2_puede_entrar = false;
    }
}

/* lineas eliminadas para facilitar lectura */

int main(int argc, char** argv) {

    cancelar = false;
    proceso1_puede_entrar = false;
    proceso2_puede_entrar = false;

	// Se crean dos hilos para ejecutar los procesos en paralelo
    thread p1(proceso1);
    thread p2(proceso2);

	// se espera hasta que el usuario presione enter.
    esperar_enter_del_usuario();

    cancelar = true;

	// Se espera a que finalicen los procesos.
    p1.join();
    p2.join();

    return EXIT_SUCCESS;
}
```

### Descripción ###

* Luego de realizar sus tareas iniciales el procesos solicita poder entrar en la sección crítica, si el otro proceso no puede entrar ya que su estado es `false` entonces el proceso entra sin problema a la sección crítica.
* De lo contrario si el otro proceso también puede entrar entonces se entra al ciclo donde el proceso actual se niega el paso así mismo y con un retardo de x tiempo siendo este aleatorio se pausa el proceso, para darle vía libre a los otros procesos.
* Luego de terminar su pausa entonces el proceso actual nuevamente puede entrar y nuevamente si el otro proceso puede entrar se repite el ciclo y si no hay otro proceso, entonces el proceso puede entrar en la sección crítica.
* Finalmente cambia su estado.
* Se incluye una variable `cancelar` que se establece a `true` cuando el usuario presiona la tecla `Enter` para poder finalizar los procesos en cualquier momento.


## Dekker V ##

Quinta versión del algoritmo de Dekker Algoritmo Optimo, este algoritmo es una combinación del algoritmo 1 y 4.

### Características ###

* Garantiza la exclusión mutua.
* Progreso
* Espera limitada

### Implementación en c++ ###

```c++
#include <cstdlib>
#include <iostream>
#include <thread>
#include <time.h>
#include <curses.h>
#include <stdio.h>
#include <string.h>

using namespace std;

bool proceso1_puede_entrar;
bool proceso2_puede_entrar;
int turno;
bool cancelar;

/* lineas eliminadas para facilitar lectura */

void ejecutar_seccion_critica_1() {
    /* lineas eliminadas para facilitar lectura */
}

void proceso1() {
    while (!cancelar) {
        proceso1_puede_entrar = true;
        while (proceso2_puede_entrar && !cancelar) {
            if (turno == 2) {
                proceso1_puede_entrar = false;
                while (turno == 2 && !cancelar) {
                    /* esperar */
                }
                proceso1_puede_entrar = true;
            }
        }
        if (cancelar) break;
        ejecutar_seccion_critica_1();
        turno = 2;
        proceso1_puede_entrar = false;
    }
}

void ejecutar_seccion_critica_2() {
    /* lineas eliminadas para facilitar lectura */
}

void proceso2() {
    while (!cancelar) {
        proceso2_puede_entrar = true;
        while (proceso1_puede_entrar && !cancelar) {
            if (turno == 1) {
                proceso2_puede_entrar = false;
                while (turno == 1 && !cancelar) {
                    /* esperar */
                }
                proceso2_puede_entrar = true;
            }
        }
        if (cancelar) break;
        ejecutar_seccion_critica_2();
        turno = 1;
        proceso2_puede_entrar = false;
    }
}

/* lineas eliminadas para facilitar lectura */

int main(int argc, char** argv) {

    cancelar = false;
    proceso1_puede_entrar = false;
    proceso2_puede_entrar = false;
    turno = 1;

	// Se crean los dos hilos para ejecutar los procesos en paralelo.
    thread p1(proceso1);
    thread p2(proceso2);

	// se espera a que el usuario presione enter.
    esperar_enter_del_usuario();

    cancelar = true;

	// se espera a que finalicen los procesos.
    p1.join();
    p2.join();

    return EXIT_SUCCESS;
}
```

### Descripción ###

* Al ejecutarse se verifica si hay otro procesos que puede entrar, si lo hay se entra al ciclo y si es el turno de algún otro proceso cambia su estado a ya no poder entrar a la sección crítica y nuevamente verifica si es el turno de algún otro proceso, si lo es, se queda enciclado hasta que se da un cambio de turno, luego nuevamente retoma su estado de poder entrar a la sección crítica, regresa al ciclo y verifica si hay otro proceso que puede entrar entonces nuevamente se encicla, de lo contrario entra a la sección crítica.
* Al salir de la sección crítica el proceso cambia su turno, cambia su estado y realiza sus tareas finales.
* Se incluye una variable `cancelar` que se establece a `true` cuando el usuario presiona la tecla `Enter` para poder finalizar los procesos en cualquier momento.


## Peterson ##

El algoritmo de Peterson, también conocido como solución de Peterson, es un algoritmo de programación concurrente para exclusión mutua, que permite a dos o más procesos o hilos de ejecución compartir un recurso sin conflictos, utilizando sólo memoria compartida para la comunicación.

### Características ###
* Simplificación del algoritmo de Dekker para dos procesos.
* Generalizado para N procesos.

### Implementación en c++ ###

```c++
#include <cstdlib>
#include <iostream>
#include <thread>
#include <time.h>
#include <curses.h>
#include <stdio.h>
#include <string.h>

using namespace std;

bool proceso1_desea_entrar;
bool proceso2_desea_entrar;
int proceso_favorecido;
bool cancelar;

/* lineas eliminadas para facilitar lectura */

void ejecutar_seccion_critica_1() {
    /* lineas eliminadas para facilitar lectura */
}

void proceso1() {
    while (!cancelar) {
        proceso1_desea_entrar = true;
        proceso_favorecido = 2;
        while (proceso2_desea_entrar && proceso_favorecido == 2 && !cancelar) {
            /* esperar */
        }
        if (cancelar) break;
        ejecutar_seccion_critica_1();
        proceso1_desea_entrar = false;
    }
}

void ejecutar_seccion_critica_2() {
    /* lineas eliminadas para facilitar lectura */
}

void proceso2() {
    while (!cancelar) {
        proceso2_desea_entrar = true;
        proceso_favorecido = 1;
        while (proceso1_desea_entrar && proceso_favorecido == 1 && !cancelar) {
            /* esperar */
        }
        if (cancelar) break;
        ejecutar_seccion_critica_2();
        proceso2_desea_entrar = false;
    }
}

/* lineas eliminadas para facilitar lectura */

int main(int argc, char** argv) {

    cancelar = false;
    proceso1_desea_entrar = false;
    proceso2_desea_entrar = false;
    proceso_favorecido = 1;

	// Se crean dos hilos para ejecutar los procesos en paralelo
    thread p1(proceso1);
    thread p2(proceso2);

	// se espera a que el usuario presione enter.
    esperar_enter_del_usuario();

    cancelar = true;

	// se espera a que finalicen los procesos.
    p1.join();
    p2.join();

    return EXIT_SUCCESS;
}
```

### Descripción ###

* Al iniciar el proceso indica que desea entrar a su sección crítica, pero a la vez favorece al otro proceso.
* Se crea un ciclo de espera hasta que otro proceso haya salido de su sección crítica y que el proceso favorecido sea el proceso actual.
* Se ejecuta la sección crítica.
* Se indica que el proceso ya salió de la sección crítica (`procesoX_desea_entrar=false`).
* Se incluye una variable `cancelar` que se establece a `true` cuando el usuario presiona la tecla `Enter` para poder finalizar los procesos en cualquier momento.


## Referencias ##

* Guía de programación de NCurses: [http://goo.gl/OA4zSu](http://hughm.cs.ukzn.ac.za/~murrellh/os/notes/ncurses.html)
* Algoritmos de Dekker en el blog de Aprendiendo Software: [http://goo.gl/cj047L](http://aprendiendo-software.blogspot.com/2011/12/algoritmo-de-dekker.html)
  * Dekker 1: [http://goo.gl/4nvTUv](http://aprendiendo-software.blogspot.com/2011/12/algoritmo-de-dekker-version-1.html)
  * Dekker 2: [http://goo.gl/qKLvU1](http://aprendiendo-software.blogspot.com/2011/12/algoritmo-de-dekker-version-2-problema.html)
  * Dekker 3: [http://goo.gl/nlg302](http://aprendiendo-software.blogspot.com/2011/12/algoritmo-de-dekker-version-3-colision.html)
  * Dekker 4: [http://goo.gl/E32k11](http://aprendiendo-software.blogspot.com/2011/12/algoritmo-de-dekker-version-4.html)
  * Dekker 5: [http://goo.gl/1Fh2qE](http://aprendiendo-software.blogspot.com/2011/12/algoritmo-de-dekker-version-5.html)
* Algoritmo de Dekker en Wikipedia: [http://goo.gl/9xgAWi](http://es.wikipedia.org/wiki/Algoritmo_de_Dekker)
* Algoritmo de Peterson En Wikipedia: [http://goo.gl/acKYB9](http://es.wikipedia.org/wiki/Algoritmo_de_Dekker)
