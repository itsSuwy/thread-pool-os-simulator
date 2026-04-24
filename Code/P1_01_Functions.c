//
// Created by suwy on 4/16/26.
//
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
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
    Hilo_Creado->ocupado=false;
    Hilo_Creado->sig=NULL;
    Hilo_Creado->inicio=NULL;
    Hilo_Creado->fin=NULL;
    return Hilo_Creado;
}
// ~~~~ Case 1 ~~~~~
// 00_Obtener el nombre del proceso
char *nombre(void) {
    char *name = (char *)calloc(20, sizeof(char));
    if (!name) {
        puts("Error de memoria!");
        exit(-1);
    }else{
        printf("Ingrese el nombre del proceso (20 caracteres): ");
            scanf( " %19s", name);
        limpiando_buffer();
        return name;
    }
}

// 01_Obtener la importancia del proceso
bool designar_importancia(void){
    char respuesta='\0';
    while (1) {
        printf("Es urgente? [Y/N]: ");
        scanf(" %c", &respuesta);
            limpiando_buffer();
        char modificado = toupper(respuesta);
        if (modificado == 'Y') {
            return true;
        }
        if (modificado == 'N') {
            return false;
        }
        else{
            puts("Entrada de datos invalida");
            puts("Vuelva a registrar su respuesta");
            printf("\n");
            continue;
        }
    }
}

struct process *proceso_empaquetado(char *name, bool urgency){
    struct process *proceso = (struct process *)calloc(1,sizeof(struct process));
    proceso->mem_addr=(char*)calloc(20,sizeof(char));
    if (!proceso) {
        puts("Error critico de memoria");
        puts("Cerrando el programa por seguridad");
        exit(-1);
    }
    proceso->name=name;
    sprintf(proceso->mem_addr, "%p", (void*)proceso);
    proceso->urgency=urgency;
    return proceso;
}

void asignar_hilo(struct thread *hilo_original, struct thread *hilo_auxiliar, struct process *process) {
    int validacion = busqueda_hilo_libre(hilo_auxiliar,process);
    if (validacion == 1){
        struct thread *hilo_menor = hilo_libre(hilo_original,hilo_original);
        asignacion_proceso_ocupado(hilo_menor,process);
    }else{
        return;
    }
}

int busqueda_hilo_libre(struct thread *hilo, struct process *process) {
    if (!hilo) { // Caso base A: Hilos vacios
        puts("Hilo vacio!");
        return 1;
    }
    if (hilo->ocupado==false) { // Caso base B: Hilo desocupado detectado
        asignacion_proceso(hilo,process);
        puts("Proceso asignado con exito");
        return 0;
    }
    else{
        return busqueda_hilo_libre(hilo->sig,process);
    }
}

void asignacion_proceso(struct thread *hilo, struct process *process) {
    if (hilo->ocupado==false) {
        hilo->inicio=process;
        hilo->fin=process;
        hilo->n_process++;
        hilo->ocupado=true;
        process->sig=NULL;
        process->id=0;
        return;
    }
}

struct thread *hilo_libre(struct thread *hilo_actual, struct thread *hilo_menor){
    if (!hilo_actual){
        return hilo_menor;
    }
    if (hilo_menor->n_process>hilo_actual->n_process) {
        hilo_menor=hilo_actual;
        }
    return hilo_libre(hilo_actual->sig,hilo_menor);
}

void asignacion_proceso_ocupado(struct thread *hilo,struct process *process){
    if (process->urgency==true){
        process->sig=hilo->inicio;
        hilo->inicio=process;
        hilo->n_process++;
    }else{
        struct process *aux = busqueda_proceso_final(hilo->inicio);
        aux->sig=process;
        process->sig=NULL;
        hilo->fin=process;
        hilo->n_process++;
    }
}

struct process *busqueda_proceso_final(struct process *process) {
    if (!process->sig) {
        return process;
    }else{
        return busqueda_proceso_final(process->sig);
    }
}

// ~~~~ Caso 2 ~~~~
// 00_Imprimir los procesos actuales asignados a cada hilo
void impresion(struct thread *Hilo){
    if (!Hilo) {
        puts("CPU vacia");
        return;
    }
    printf("Numero de Hilo: %i\n",Hilo->id);
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

void visualizar_procesos(struct thread *hilo){
    if (!hilo) {
        puts("~~~Lista de procesos vacia~~~");
        return;
    }
    printf("No. de hilo: %i\n", hilo->id);
    printf("No. de procesos asignados: %i\n\n", hilo->n_process);
    extraer_proceso(hilo->inicio);
    visualizar_procesos(hilo->sig);

}
void extraer_proceso(struct process *process) {
    if (!process){
        printf("\n\n");
        return;
    }
    printf("No. Proceso: %i\n", process->id);
    printf("Nombre del proceso: %s\n", process->name);
    printf("Direccion de memoria del proceso %s\n", process->mem_addr);
    if (process->urgency == true) {
        puts("Es urgente el proceso");
    }else {
        puts("No es urgente el proceso");
    }
    printf("\n");
    extraer_proceso(process->sig);
}

// ~~~~ Case 4 ~~~~~
// Proceso de mobilizacion hacia la pila y dejar vacio cada hilo

// 00_Crear una pila
struct pila *crear_pila(struct cpu *CPU){
    struct pila *output = (struct pila *)calloc(1, sizeof(struct pila));
    if (!output) {
        puts("Error critico de memoria, cerrando el programa por seguridad");
        exit(-1);
    }
    extraccion_hilos(CPU->inicio,output, CPU->inicio->inicio);
    //puts("Visualizacion de procesos ejecutados");
    //impresion_pila(output->tope);
    return output;
}

// 01_Recorrer cada hilo hasta llegar a uno NULL
void extraccion_hilos(struct thread *hilo, struct pila *output, struct process *proceso) {
    if (!proceso) { // Se llego al fin de los procesos
        actualizar_hilo(hilo);
        if (!hilo->sig) {
            return; // No hay a donde mas acceder
        }
        return extraccion_hilos(hilo->sig, output, hilo->sig->inicio);
    }
    push(hilo,output);
    return extraccion_hilos(hilo,output,hilo->inicio);
}

// 02_Extraer los elementos del hilo original
void push(struct thread *hilo, struct pila *output){
    struct process *aux = hilo->inicio;
    hilo->inicio=hilo->inicio->sig;
    aux->sig=output->tope;
    output->tope=aux;
    output->n_elementos++;
    hilo->n_process--;
}

void actualizar_hilo(struct thread *hilo) {
    hilo->inicio=NULL;
    hilo->fin=NULL;
    hilo->ocupado=false;
}

void impresion_pila(struct process *proceso){
    if (!proceso) { // Se llego al final
        return;
    }
    printf("ID asignado al proceso: %i\n", proceso->id);
    printf("Nombre del proceso: %s\n", proceso->name);
    printf("Direccion de memoria del proceso: %s\n", proceso->mem_addr);
    if (proceso->urgency == true) {
        puts("Es un proceso urgente");
    }else{
        puts("Proceso carente de urgencia");
    }
    return impresion_pila(proceso->sig);
}
void subir_archivo(struct process *proceso) {
    FILE *fp = fopen("../Logs/log.txt","w");
    if (!fp) {
        puts("Error critico de memoria!");
        exit(-1);
    }
    procesar_archivo(proceso,fp);
    fclose(fp);
    return;
}

void procesar_archivo(struct process *proceso, FILE *fp) {
    if (!proceso) {
        fprintf(fp,"Todos los procesos han sido anotados con exito\n");
        return;
    }
    fprintf(fp,"ID asignado: %i\n", proceso->id);
    fprintf(fp,"Nombre del proceso: %s\n", proceso->name);
    fprintf(fp, "Direccion de memoria asignada: %s\n", proceso->mem_addr);
    if (proceso->urgency == true) {
        fprintf(fp,"Proceso marcado como urgente\n");
    }else{
        fprintf(fp,"Proceso marcado como no urgente\n");
    }
    fprintf(fp,"\n\n");
    return procesar_archivo(proceso->sig,fp);
}

void limpiando_buffer(void) {
    while (getchar() !='\n'){}
}
/* There is a bug ahead!
void ordenar_CPU(struct cpu *CPU) {
    if (!CPU->inicio->sig){
        CPU->inicio = CPU->fin;
        CPU->fin = NULL;
        return;
    }
    struct thread *aux = CPU->inicio;
    CPU->inicio=CPU->inicio->sig;
    ordenar_CPU(CPU);
    ordenar_CPU_auxiliar(CPU, aux);
}
void ordenar_CPU_auxiliar(struct cpu *CPU, struct thread *hilo) {
    if (!CPU->inicio->sig) {

        return;
    }else{

    }
}
*/