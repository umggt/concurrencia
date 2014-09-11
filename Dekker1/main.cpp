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

using namespace std;

int turno;
bool cancelar;

void ejecutar_seccion_critica_1() {
    
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

    cout << "Ha terminado el proceso 1" << endl;

}


void ejecutar_seccion_critica_2() {
    
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

    cout << "Ha terminado el proceso 2" << endl;

}

/*
 * 
 */
int main(int argc, char** argv) {

    setlocale(LC_ALL, "spanish");

    srand(time(NULL));
    cancelar = false;
    turno = rand() % 100 + 1 <= 50 ? 1 : 2;

    cout << "Ejecutando procesos (prioridad al " << turno << ")..." << endl;

    thread p1(proceso1);
    thread p2(proceso2);
    
    cout << "Presione cualquier tecla para salir." << endl;
    cin.get();
    
    cancelar = true;
    
    p1.join();
    p2.join();
    
    cout << "Todos los procesos han finalizado." << endl;
    
    cout.flush();
    
    return EXIT_SUCCESS;
}

