#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <stdbool.h>

#define MAX_CODIGO 10
#define MAX_VUELOS 1000

// Struct para representar una ruta validada previamente
typedef struct {
    char origen[MAX_CODIGO];
    char destino[MAX_CODIGO];
} Ruta;

// Valida que ninguna ruta sea duplicada, considerando que el grafo es no dirigido.
bool misma_ruta(const char *a1, const char *a2, const char *b1, const char *b2);

// Valida si una línea no tiene valores faltantes y no duplicada
bool validar_linea(const char *origen, const char *destino, int distancia, Ruta rutas[], int cantidad);

#endif