//
// Created by suwy on 4/16/26.
//
#include <unistd.h>
#include <stdbool.h>

struct thread {
    int id;
    char name[20];
    bool urgent;
    struct dato *sig;
};

int determinarHilos(void) {
    int hilos = sysconf(_SC_NPROCESSORS_ONLN); // Se calcula el numero de Hilos reales con el que cuenta el sistema operativo que ejecute el codigo
    return hilos;
}

void asignacionHilos(struct thread *hilo, int n) {
    if (n==1) { // Caso base A

    }
    if (n==0) { // Caso base B
        return;
    }else{
        hilo->id=;
    }

}