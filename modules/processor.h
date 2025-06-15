#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdbool.h>
#include "validator.h"

#define MAX_CODIGO 10
#define MAX_NODOS 100
#define MAX_VUELOS 1000
#define INF 1000000000

// Struct de vuelo
typedef struct {
    char origen[MAX_CODIGO];
    char destino[MAX_CODIGO];
    int distancia;
} Vuelo;


// Retorna la cantidad de vuelos válidos, o -1 si hubo error.
int leer_archivo(const char *nombre_archivo, Vuelo vuelos[], Ruta rutas_validas[], int *cantidad_rutas);

// Asigna índice único a una ciudad
int obtener_indice(char ciudades[][MAX_CODIGO], int *num_ciudades, const char *nombre);

// Construye el grafo (matriz de adyacencia) desde los vuelos
void construir_grafo(int grafo[MAX_NODOS][MAX_NODOS], Vuelo vuelos[], int cantidad, char ciudades[][MAX_CODIGO], int *num_ciudades);

// Ejecuta el algoritmo de Dijkstra desde una ciudad
// Devuelve las distancias mínimas y los predecesores para reconstruir rutas
void dijkstra(int grafo[MAX_NODOS][MAX_NODOS], int num_nodos, int inicio, int distancias[], int predecesores[]);

#endif