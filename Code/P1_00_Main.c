/*
    - Desarrollar un sistema que emule el comportamiento de un procesador de tareas (programación concurrente). Debera realizar lo siguiente:
    - ***Colas***
    - Tareas pendientes (una cola de prioridad)
    - Estructura de datos a utilizar: Cola
    - Representar la cola de tareas que recibe el sistema.
    - Cada vez que llegue una nueva tarea al sistema (como por ejemplo Proceso_1, Proceso_2, Proceso_3, etc.),
        entrará al final de la cola mediante la operación encolar. El sistema tomará siempre la tarea que esté al frente para procesarla,
        a menos que llegue alguna tarea con mayor prioridad.
    - Se pueden manejar dos niveles: normal y urgente.
    - ***Listas***
    - Pool de hilos (thread pool):
    - Estructura de datos a utilizar: Lista enlazada
    - Representa el pool de hilos disponibles para procesar tareas. Cada hilo cuenta con dos atributos: id y estado(ocupado/libre).
    - Se debe crear una lista enlazada donde cada nodo es un hilo (Hilo_1, Hilo_2, Hilo_3, etc.) El sistema debe recorrer la lista buscando el primer hilo que este marcado como libre. Al encontrarlo, le asigna la tarea de la cola y el hilo  cambia su estado a “ocupado”.
    - El recorrido de la lista para buscar un espacio libre, debe realizarse mediante una función recursiva.
    - ***Pilas***
    - Historial de ejecución:
    - Estructura de datos a utilizar: Pila
    - Representa los resultados devueltos tras completarse una tarea, organizados como un historial de ejecución.
    - Cuando un hilo termina su tarea, el resultado se inserta en la cima de la pila mediante la operación push. Si el sistema falla y necesita saber cuál fue la última tarea ejecutada (a fin de realizar deshacer), simplemente hará pop sobre este historial.
    - El historial (log) debe guardarse en un archivo.
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "P1_01_Functions.h"

int main(void) {
    //struct process procesos;
    struct cpu *CPU = (struct cpu*)calloc(1,sizeof(struct cpu));
    char opcion='\0';

    puts("Bienvenido al gestor de procesos de DiegOS");
    puts("Escaneando los hilos reales de este equipo...");
        int Hilos_reales = determinarHilos();
    printf("Esta computadora cuenta con %i hilos de trabajo utilizables\n", Hilos_reales);
        Crear_CPU(CPU,Hilos_reales);
    puts("CPU simulada creada con exito!");
    while (1) {
        puts("\t\n\n~~~Por favor ingrese una opcion~~~");
        puts("1) Ingresar un proceso");
        puts("2) Visualizar especificaciones de los Hilos");
        puts("3) Visualizar el esquema de procesos actuales");
        puts("4) Ejecutar los procesos actuales y visualizar sus outputs");
        puts("5) Salir");
        printf("Opcion: ");
            scanf(" %c", &opcion);
            limpiando_buffer();
            int entrada_ascii = procesado_de_entrada(opcion);
        switch (entrada_ascii) {
            case 49: // Valor ascii de 1
                carga_proceso(CPU);
                acceso_CPU(CPU);
                break;
            case 50: // Valor ascii de 2
                printf("\n");
                impresion(CPU->inicio);
                break;
            case 51: // Valor ascii de 3
                printf("\n");
                visualizar_procesos(CPU->inicio);
                break;
            case 52: // Valor ascii de 4
                struct pila *pila_de_procesos = crear_pila(CPU);
                puts("Visualizacion delos procesos ejecutados");
                impresion_pila(pila_de_procesos->tope);
                puts("Guardando el archivo en un .txt");
                subir_archivo(pila_de_procesos->tope);
                puts("Archivo generado con exito!");
                puts("Limpiando la pila de procesos...");
                limpiar_pila(pila_de_procesos);
                break;
            case 53: // Valor ascii de 5
                free(CPU);
                exit(0);
            default:
                puts("Entrada invalida!");
                break;
        }
    }
    free(CPU);
    return 0;
}