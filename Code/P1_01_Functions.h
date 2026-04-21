//
// Created by suwy on 4/16/26.
//

#ifndef THREAD_POOL_OS_SIMULATOR_P1_01_FUNCTIONS_H
#define THREAD_POOL_OS_SIMULATOR_P1_01_FUNCTIONS_H

struct process { // <- Esto es una cajita, se supone que aqui solo tiene que vivir el proceso que entre
    int id;
    char *name;
    char *hardware;
    bool urgency;
    struct process *sig;
};

struct thread { // El hilo que almacenara la cola
    int id;
    int n_process;
    bool ocupado;
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
void impresion(struct thread *Hilo);
int procesado_de_entrada(char input);
void limpiando_buffer(void);
void ordenar_CPU(struct cpu *CPU); // WIP
void ordenar_CPU_auxiliar(struct cpu *CPU, struct thread *hilo); // WIP
char *nombre(void);
bool designar_importancia(void);
struct process *proceso_empaquetado(char *name, bool urgency);
void busqueda_hilo_libre(struct thread *hilo, struct process *process);
struct process *busqueda_proceso_final(struct process *process);
void asignacion_proceso(struct thread *hilo, struct process *process);
struct thread *hilo_libre(struct thread *hilo_actual, struct thread *hilo_menor);
void asignacion_proceso_ocupado(struct thread *hilo,struct process *process);

#endif //THREAD_POOL_OS_SIMULATOR_P1_01_FUNCTIONS_H
