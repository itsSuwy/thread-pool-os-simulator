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
        struct stack *cola_global = (struct stack *)calloc(1, sizeof(struct stack));
        CPU->pendientes=cola_global;
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
// ~~~~ Case 1 ~~~~~ (

// LOGICA PARA GENERAR PROCESOS
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

// 02_Crear el proceso
struct process *proceso_empaquetado(char *name, bool urgency){
    struct process *proceso = (struct process *)calloc(1,sizeof(struct process));
    if (!proceso) {
        puts("Error critico de memoria");
        puts("Cerrando el programa por seguridad");
        exit(-1);
    }
    proceso->mem_addr=(char*)calloc(20,sizeof(char)); // Memoria dinamica para almacenar la direccion del apuntador
    proceso->name=name; // Nombre del proceso
    sprintf(proceso->mem_addr, "%p", (void*)proceso); // Escribir la direccion del apuntador en la memoria dinamica previamente asignada
    proceso->urgency=urgency; // La urgencia definida por el usuario
    return proceso;
}

// 03_Generar una cola de procesos que deben ser asignados a hilos libres
void cola_global(struct cpu *CPU, struct process *proceso_nuevo){
    if (!CPU->pendientes->inicio) {
        CPU->pendientes->inicio=proceso_nuevo;
        CPU->pendientes->final=proceso_nuevo;
        return;
    }
    struct process *aux = proceso_final(CPU->pendientes->inicio);
    aux->sig=proceso_nuevo;
    CPU->pendientes->final=proceso_nuevo;
    return;
}
// 04_Funcion auxiliar para recorrer la cola de procesos hasta llegar al ultimo
struct process *proceso_final(struct process *proceso_cola){
    if (!proceso_cola->sig) {
        return proceso_cola;
    }else{
        return proceso_final(proceso_cola->sig);
    }
}

// 05_Funcion auxiliar para determinar si el usuario quiere adjuntar otro proceso
int repeticion(void) {
    puts("Desea ingresar otro proceso?[Y/N]");
    char respuesta='\0';
    while (1) {
        scanf(" %c", &respuesta);
        limpiando_buffer();
        int entrada_ascii = procesado_de_entrada(respuesta);
        if (entrada_ascii==89 || entrada_ascii == 121) { // El usuario ingresa Y / y
            printf("\n");
            return 1;
        }
        if (entrada_ascii == 78 || entrada_ascii == 110) { // El usuario ingresa N / n
            printf("\n");
            return 0;
        }else {
            puts("Entrada no valida!");
        }
    }
}

// 06_Subir el proceso a la cola de procesos
void carga_proceso(struct cpu *CPU) {
    char *name = nombre();
    bool urgencia = designar_importancia();
    struct process *proceso_fabricado = proceso_empaquetado(name,urgencia);
    cola_global(CPU,proceso_fabricado);
    int respuesta = repeticion();
    if (respuesta==1) {
        return carga_proceso(CPU);
    }else {
        return;
    }
}

// FIN DE LA LOGICA PARA GENERAR PROCESOS
// ~~~~ Case 1 ~~~~~ (Asignar un proceso a los hilos de la CPU)

// 00_Funcion principal para asginar el proceso al hilo asignado
void acceso_CPU(struct cpu *CPU){
    if (!CPU->pendientes->inicio) { // Fin de los procesos a asginar
        return;
    }
    struct process *proceso_cabeza = extraer_proceso_inicial(CPU->pendientes); // Extraer el primer hilo
    struct thread *hilo_encontrado = extraer_hilo(CPU->inicio, CPU->inicio); // Extraer un Hilo disponible
    asignar_Proceso(hilo_encontrado, proceso_cabeza);
    return acceso_CPU(CPU);
}

// 01_Obtener el primer proceso de la cola general
struct process *extraer_proceso_inicial(struct stack *pendientes) {
    struct process *aux = pendientes->inicio; // Se saca el proceso inicial
    pendientes->inicio=aux->sig;
    aux->sig=NULL; // Se tiene que desconectar el sig 
    return aux;
}

// 02_Obtener aquel hilo con la menor cantidad de procesos asignados
struct thread *extraer_hilo(struct thread *Hilo, struct thread *Hilo_respaldo){
    if (!Hilo) { // El hilo al que se accede es NULL, ya no hay a donde mas desplazarse
        return Hilo_respaldo;
    }
    if (Hilo->n_process <= Hilo_respaldo->n_process){ // El hilo mas chiquito, tiene mas procesos que el hilo actual
        Hilo_respaldo = Hilo;
        return extraer_hilo(Hilo->sig, Hilo_respaldo);
    }
    return extraer_hilo(Hilo->sig, Hilo_respaldo);
}

// 03_Asignar el primer proceso de la cola general al hilo obtenido
void asignar_Proceso(struct thread *Hilo, struct process *proceso){
    if (!Hilo->inicio){ // Hilo vacio
        Hilo->inicio=proceso;
        Hilo->fin=proceso;
        Hilo->n_process++;
        Hilo->ocupado=true;
        return;
    }
    if (proceso->urgency==true) {
        proceso_urgente(Hilo, proceso);
        return;
    }else{
        proceso_comun(Hilo,proceso);
        return;
    }
}

// 04_Configuracion en caso de que el proceso sea urgente -> se asigna como primer proceso
void proceso_urgente(struct thread *Hilo, struct process *proceso){
    struct process *aux = Hilo->inicio; // Extraer el proceso inicial
    Hilo->inicio=proceso;
    proceso->sig=aux;
    Hilo->n_process++;
    Hilo->ocupado=true;
}

// 05_Configuracion en caso de que el proceso sea comun -> se asigna a la cola de procesos
void proceso_comun(struct thread *Hilo, struct process *proceso){
    struct process *aux = Hilo->fin;
    aux->sig=proceso;
    Hilo->fin=proceso;
    Hilo->n_process++;
    Hilo->ocupado=true;
}

// FIN DE LA LOGICA DE ASIGNACION DE PROCESOS

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

// ~~~~ Caso 3 ~~~~

// 00_Observar la cola de procesos actuales asignados a cada hilo
void visualizar_procesos(struct thread *hilo){
    if (!hilo) { // En caso de que haber llegado al hilo final
        puts("~~~Lista de procesos vacia~~~");
        return;
    }
    printf("No. de hilo: %i\n", hilo->id);
    printf("No. de procesos asignados: %i\n\n", hilo->n_process);
    extraer_proceso(hilo->inicio);
    visualizar_procesos(hilo->sig);
}

// 01_Historial de procesos asignados al hilo
void extraer_proceso(struct process *process) {
    if (!process){ // Proceso vacio, no hay mas que mostrar
        printf("\n");
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
    struct pila *output = (struct pila *)calloc(1, sizeof(struct pila)); // Se crea una pila
    if (!output) {
        puts("Error critico de memoria, cerrando el programa por seguridad");
        exit(-1);
    }
    extraccion_hilos(CPU->inicio,output, CPU->inicio->inicio);
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
    hilo->inicio=aux->sig;
    aux->sig=output->tope;
    output->tope=aux;
    output->n_elementos++;
    hilo->n_process--;
}

// 03_Funcion para reiniciar los apuntadores y el estado actual del hilo
void actualizar_hilo(struct thread *hilo) {
    hilo->inicio=NULL;
    hilo->fin=NULL;
    hilo->ocupado=false;
}

// 04_Mostrar en terminal los procesos asignados a la pila
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

// 05_Preparar el apuntador para subir el historial de procesos a un .txt
void subir_archivo(struct process *proceso) {
    FILE *fp = fopen("../Logs/log.txt","w"); // Ruta y tipo de escritura
    if (!fp) {
        puts("Error critico de memoria!");
        exit(-1);
    }
    procesar_archivo(proceso,fp);
    fclose(fp);
    return;
}

// 06_Adjuntar la informacion de cada proceso al .txt
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
    fprintf(fp,"\n");
    return procesar_archivo(proceso->sig,fp);
}

void limpiar_pila(struct pila *pila) {
    if (!pila->tope) {
        free(pila);
        puts("Pila limpiada con exito");
        return;
    }else {
        struct process *proceso_a_eliminar = pila->tope;
        pila->tope = proceso_a_eliminar->sig;
        limpieza_proceso(proceso_a_eliminar);
        free(proceso_a_eliminar);
        return limpiar_pila(pila);
    }
}

void limpieza_proceso(struct process *proceso) {
    free(proceso->name);
    free(proceso->mem_addr);
    return;
}

void ejecutar_procesos(struct cpu *CPU) {
    struct pila *pila_de_procesos = crear_pila(CPU); // Se crea la pila
    if (!pila_de_procesos->tope) { // El tope no tiene elementos
        puts("No hay procesos asignados!");
        puts("Volviendo al menu principal");
        free(pila_de_procesos);
        return;
    }
    puts("Visualizacion de los procesos ejecutados");
    impresion_pila(pila_de_procesos->tope);
    puts("Guardando el archivo en un .txt");
    subir_archivo(pila_de_procesos->tope);
    puts("Archivo generado con exito!");
    puts("Limpiando la pila de procesos...");
    limpiar_pila(pila_de_procesos);
    return;
}

// Funciones auxiliares <- No pertenecen a un caso especifico del flujo, aparecen en multiples partes
void limpiando_buffer(void) {
    while (getchar() !='\n'){}
}

int procesado_de_entrada(char input) {
    int respuesta=0;
    respuesta = (int)input;
    return respuesta;
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