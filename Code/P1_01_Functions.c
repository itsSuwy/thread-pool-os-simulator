//
// Created by suwy on 4/16/26.
//
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "P1_01_Functions.h"

int determinarHilos(void) {
    int hilos = sysconf(_SC_NPROCESSORS_ONLN); // Se calcula el numero de Hilos reales con el que cuenta el sistema operativo que ejecute el codigo
    return hilos;
}
void Crear_CPU(struct cpu *CPU, int hilos) { // Funcion para construir la CPU
    hilos--;
    if (!CPU) {
        puts("Error de memoria\nCerrando el programa por seguridad");
        exit(-1);
    }
    if (hilos<0) { // Cuando ya no hay mas hilos que sacar
        return;
    }
    struct thread *hilo_generado= Crear_Hilos(hilos); // Se genera un hilo
    if (CPU->inicio== NULL && CPU->fin==NULL) { // Si apuntan a un NULL (La cola esta vacia)
        CPU->inicio=hilo_generado;
        CPU->fin=hilo_generado;
    }else {
        struct thread *aux= CPU->fin;
        CPU->fin=hilo_generado;
        aux->sig=CPU->fin;
    }
    return Crear_CPU(CPU, hilos);
}
struct thread *Crear_Hilos(int hilos) {
    struct thread *Hilo_Creado = (struct thread*)calloc(1,sizeof(struct thread));
    if (!Hilo_Creado) {
        return NULL;
    }
    Hilo_Creado->id=hilos;
    Hilo_Creado->n_process=0;
    sprintf(Hilo_Creado->name, "%p", (void*)Hilo_Creado); // Para que cada hilo cuente con un nombre propio
    Hilo_Creado->sig=NULL;
    Hilo_Creado->inicio=NULL;
    Hilo_Creado->fin=NULL;
    return Hilo_Creado;
}
void impresion(struct thread *Hilo){
    if (!Hilo) {
        puts("CPU vacia");
        return;
    }
    printf("ID asignado: %i\n",Hilo->id);
    printf("Cantidad de procesos asignados: %i\n",Hilo->n_process);
    printf("Nombre de hardware del hilo: %s\n", Hilo->name);
    printf("\n");
    return impresion(Hilo->sig);
}
int procesado_de_entrada(char input) {
    int respuesta=0;
    respuesta = (int)input;
    return respuesta;
}
void limpiando_buffer(void) {
    while (getchar() !='\n'){}
}