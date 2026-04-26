// Elaborado por: Diego Velazquez Sanchez - Estructura de Datos

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
                printf("\n");
                ejecutar_procesos(CPU);
                break;
            case 53: // Valor ascii de 5
                liberar_memoria_final(CPU);
                exit(0);
            default:
                puts("Entrada invalida!");
                break;
        }
    }
    return 0;
}