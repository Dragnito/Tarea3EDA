#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modules/processor.h"

void imprimir_ruta(int predecesores[], int destino, char ciudades[][MAX_CODIGO]) {
    // Ruta invertida: primero guardamos desde el destino hasta el origen
    int ruta[MAX_NODOS];
    int largo = 0;
    int actual = destino;

    while (actual != -1) {
        ruta[largo++] = actual;
        actual = predecesores[actual];
    }

    // Imprimir desde el origen al destino
    for (int i = largo - 1; i >= 0; i--) {
        printf("%s", ciudades[ruta[i]]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s rutas.txt CIUDAD_ORIGEN CIUDAD_DESTINO\n", argv[0]);
        return 1;
    }

    const char *archivo = argv[1];
    const char *nombre_origen = argv[2];
    const char *nombre_destino = argv[3];

    // Cargar vuelos y validar
    Vuelo vuelos[MAX_VUELOS];
    Ruta rutas_validas[MAX_VUELOS];
    int cantidad_vuelos = 0;

    if (leer_archivo(archivo, vuelos, rutas_validas, &cantidad_vuelos) == -1) {
        return 1;
    }

    // Construcción del grafo
    char ciudades[MAX_NODOS][MAX_CODIGO];
    int grafo[MAX_NODOS][MAX_NODOS] = {0};
    int num_ciudades = 0;

    construir_grafo(grafo, vuelos, cantidad_vuelos, ciudades, &num_ciudades);

    // Buscar índices de ciudades
    int origen = -1, destino = -1;
    for (int i = 0; i < num_ciudades; i++) {
        if (strcmp(ciudades[i], nombre_origen) == 0) origen = i;
        if (strcmp(ciudades[i], nombre_destino) == 0) destino = i;
    }

    if (origen == -1 || destino == -1) {
        printf("Error: una o ambas ciudades no estan en los datos.\n");
        return 1;
    }

    // Dijkstra
    int distancias[MAX_NODOS];
    int predecesores[MAX_NODOS];
    dijkstra(grafo, num_ciudades, origen, distancias, predecesores);

    if (distancias[destino] == INF) {
        printf("No hay ruta desde %s a %s.\n", nombre_origen, nombre_destino);
    } else {
        printf("Ruta optima: ");
        imprimir_ruta(predecesores, destino, ciudades);
        printf("Distancia total: %d km\n", distancias[destino]);
    }

    return 0;
}