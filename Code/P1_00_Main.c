/*-	Desarrollar un sistema que emule el comportamiento de un procesador de tareas (programación concurrente). Debera realizar lo siguiente:
    - Tareas pendientes (una cola de prioridad)
    - Estructura de datos a utilizar: Cola
    - Representar la cola de tareas que recibe el sistema.
    - Cada vez que llegue una nueva tarea al sistema (como por ejemplo Proceso_1, Proceso_2, Proceso_3, etc.),
        entrará al final de la cola mediante la operación encolar. El sistema tomará siempre la tarea que esté al frente para procesarla,
        a menos que llegue alguna tarea con mayor prioridad.
    - Se pueden manejar dos niveles: normal y urgente.
    - Pool de hilos (thread pool):
    - Estructura de datos a utilizar: Lista enlazada
    - Representa el pool de hilos disponibles para procesar tareas. Cada hilo cuenta con dos atributos: id y estado(ocupado/libre).
    - Se debe crear una lista enlazada donde cada nodo es un hilo (Hilo_1, Hilo_2, Hilo_3, etc.) El sistema debe recorrer la lista buscando el primer hilo que este marcado como libre. Al encontrarlo, le asigna la tarea de la cola y el hilo  cambia su estado a “ocupado”.
    - El recorrido de la lista para buscar un espacio libre, debe realizarse mediante una función recursiva.

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
    puts("Bienvenido al gestor de procesos de DiegOS");
    int Hilos = determinarHilos();
    printf("Esta computadora cuenta con %i hilos de trabajo utilizables\n", Hilos);
    int a=0, *ptr_a=&a;
    printf("%p",ptr_a);
    return 0;
}