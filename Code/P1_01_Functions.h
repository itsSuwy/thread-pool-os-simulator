//
// Created by suwy on 4/16/26.
//

#ifndef THREAD_POOL_OS_SIMULATOR_P1_01_FUNCTIONS_H
#define THREAD_POOL_OS_SIMULATOR_P1_01_FUNCTIONS_H

struct process { // <- Esto es una cajita, se supone que aqui solo tiene que vivir el proceso que entre
    int id;
    char name[20];
    bool urgent;
    struct dato *sig;
};

struct thread { // El hilo que almacenara la cola
    int id;
    int n_process;
    char name[20];
    struct thread *sig;
    struct process *inicio; // El primer proceso
    struct process *fin; // El ultimo proceso
};

struct cpu { // El cpu que almacena los hilos
    struct thread *inicio; // El primer hilo que entre
    struct thread *fin; // El ultimo hilo en entrar
};

int determinarHilos(void);
void Crear_CPU(struct cpu *CPU, int hilos);
struct thread *Crear_Hilos(int hilos);

#endif //THREAD_POOL_OS_SIMULATOR_P1_01_FUNCTIONS_H
