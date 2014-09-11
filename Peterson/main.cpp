/*
 * File:   main.cpp
 * Author: miguelerm
 *
 * Created on September 10, 2014, 11:40 PM
 */

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

WINDOW * winA;
WINDOW * winB;

WINDOW * winTop;
WINDOW * winBottom;

void retardar_unos_milisegundos(int velocidad) {
    for (int i = 0; i < velocidad && !cancelar; i++) {
        for (int j = 0; j < velocidad && !cancelar; j++) {
            /* no hace nada */
        }
    }
}

void retardo_aleatoreo()
{
    srand(time(NULL));
    
    int tiempo = (rand() % 100) * 50;
    
    retardar_unos_milisegundos(tiempo);
}

void ejecutar_seccion_critica_1() {
    waddstr(winA, "+");
    wrefresh(winA);
    retardar_unos_milisegundos(15000);
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

    waddstr(winA, "Ha terminado el proceso 1");
    wrefresh(winA);

}

void ejecutar_seccion_critica_2() {
    waddstr(winB, "*");
    wrefresh(winB);
    retardar_unos_milisegundos(5000);
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

    waddstr(winB, "Ha terminado el proceso 2");
    wrefresh(winB);
}

void inicializar_pantallas() {
    setlocale(LC_ALL, "spanish");
    initscr();
    erase();
    refresh();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);


    int h, w;
    getmaxyx(stdscr, h, w);

    winA = newwin(h - 2, (w / 2) - 1, 1, 0);
    winB = newwin(h - 2, (w / 2) - 1, 1, (w / 2));

    winTop = newwin(1, w, 0, 0);
    winBottom = newwin(1, w, h-1, 0);

    const char * titulo = "=== Peterson ===";

    wmove(winTop, 0, (w / 2) - strlen(titulo));
    waddstr(winTop, titulo);
    wrefresh(winTop);

}

void esperar_enter_del_usuario() {
    waddstr(winBottom, "Presione la tecla [Enter] para salir.");
    wrefresh(winBottom);
    getch();
    refresh();
}

int main(int argc, char** argv) {

    inicializar_pantallas();

    cancelar = false;
    proceso1_desea_entrar = false;
    proceso2_desea_entrar = false;
    proceso_favorecido = 1;

    thread p1(proceso1);
    thread p2(proceso2);


    esperar_enter_del_usuario();

    cancelar = true;

    p1.join();
    p2.join();

    endwin();
    return EXIT_SUCCESS;
}

