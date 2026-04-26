//
// Created by suwy on 4/16/26.
//

#ifndef THREAD_POOL_OS_SIMULATOR_P1_01_FUNCTIONS_H
#define THREAD_POOL_OS_SIMULATOR_P1_01_FUNCTIONS_H

struct process { // <- Esto es una cajita, se supone que aqui solo tiene que vivir el proceso que entre
    int id;
    char *name;
    char *mem_addr;
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
    struct stack *pendientes;
};

struct stack {
    struct process *inicio;
    struct process *final;
};

struct pila {
    struct process *tope;
    int n_elementos;
};

int determinarHilos(void);
void Crear_CPU(struct cpu *CPU, int hilos); // Recursiva
struct thread *Crear_Hilos(int hilos);
char *nombre(void);
bool designar_importancia(void);;
struct process *proceso_empaquetado(char *name, bool urgency);
void cola_global(struct cpu *CPU, struct process *proceso_nuevo);
struct process *proceso_final(struct process *proceso_cola); // Recursiva
int repeticion(void);
void carga_proceso(struct cpu *CPU);
void acceso_CPU(struct cpu *CPU); // Recursiva
struct process *extraer_proceso_inicial(struct stack *pendientes);
struct thread *extraer_hilo(struct thread *Hilo, struct thread *Hilo_respaldo); // Recursiva
void asignar_Proceso(struct thread *Hilo, struct process *proceso);
void proceso_urgente(struct thread *Hilo, struct process *proceso);
void proceso_comun(struct thread *Hilo, struct process *proceso);
void impresion(struct thread *Hilo); // Recursiva
void visualizar_procesos(struct thread *hilo); // Recursiva
void extraer_proceso(struct process *process); // Recursiva
struct pila *crear_pila(struct cpu *CPU);
void extraccion_hilos(struct thread *hilo, struct pila *output, struct process *proceso); // Recursiva
void push(struct thread *hilo, struct pila *output);
void actualizar_hilo(struct thread *hilo);
void impresion_pila(struct process *proceso); // Recursiva
void subir_archivo(struct process *proceso);
void procesar_archivo(struct process *proceso, FILE *fp); // Recursiva
void limpiar_pila(struct pila *pila); // Recursiva
void limpieza_proceso(struct process *proceso);
int procesado_de_entrada(char input);
void limpiando_buffer(void);
void ordenar_CPU(struct cpu *CPU); // WIP
void ordenar_CPU_auxiliar(struct cpu *CPU, struct thread *hilo); // WIP

#endif //THREAD_POOL_OS_SIMULATOR_P1_01_FUNCTIONS_H
