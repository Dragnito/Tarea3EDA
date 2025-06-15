#include <string.h>
#include "validator.h"

// Valida si las rutas son duplicadas, considerando que el grafo es no dirigido.
bool misma_ruta(const char *a1, const char *a2, const char *b1, const char *b2) {
    return (strcmp(a1, b1) == 0 && strcmp(a2, b2) == 0) ||
           (strcmp(a1, b2) == 0 && strcmp(a2, b1) == 0);
}

// Valida si la fila del archivo no tiene datos faltantes y no se encuentra duplicada la ruta
bool validar_linea(const char *origen, const char *destino, int distancia, Ruta rutas[], int cantidad) {
    if (strlen(origen) == 0 || strlen(destino) == 0 || distancia <= 0)
        return false;

    for (int i = 0; i < cantidad; i++) {
        if (misma_ruta(origen, destino, rutas[i].origen, rutas[i].destino))
            return false;
    }
    return true;
}