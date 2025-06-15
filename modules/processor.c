#include <stdio.h>
#include <string.h>
#include "processor.h"
#include "validator.h"

// Funcion que lee el archivo de rutas
int leer_archivo(const char *nombre_archivo, Vuelo vuelos[], Ruta rutas_validas[], int *cantidad_rutas) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("Error: no se pudo abrir el archivo '%s'.\n", nombre_archivo);
        return -1;
    }

    char origen[MAX_CODIGO], destino[MAX_CODIGO];
    int distancia;
    int cantidad = 0;
    // Validar las rutas leídas para evitar errores y duplicados
    while (fscanf(archivo, "%s %s %d", origen, destino, &distancia) == 3) {
        if (!validar_linea(origen, destino, distancia, rutas_validas, cantidad)) {
            printf("Línea invalida o duplicada: %s %s %d\n", origen, destino, distancia);
            continue;
        }

        // Guardar vuelo válido
        strcpy(vuelos[cantidad].origen, origen);
        strcpy(vuelos[cantidad].destino, destino);
        vuelos[cantidad].distancia = distancia;

        // Registrar ruta como ya validada
        strcpy(rutas_validas[cantidad].origen, origen);
        strcpy(rutas_validas[cantidad].destino, destino);

        cantidad++;
        if (cantidad >= MAX_VUELOS) {
            printf("Advertencia: se alcanzó el máximo de vuelos permitidos (%d).\n", MAX_VUELOS);
            break;
        }
    }

    fclose(archivo);
    *cantidad_rutas = cantidad;
    return cantidad;
}

// Función para asignarle un índice a una ciudad 
int obtener_indice(char ciudades[][MAX_CODIGO], int *num_ciudades, const char *nombre) {
    for (int i = 0; i < *num_ciudades; i++) {
        if (strcmp(ciudades[i], nombre) == 0)
            return i;
    }
    strcpy(ciudades[*num_ciudades], nombre);
    (*num_ciudades)++;
    return (*num_ciudades) - 1;
}

// Funcion que construye el grafo a partir de los vuelos
void construir_grafo(int grafo[MAX_NODOS][MAX_NODOS], Vuelo vuelos[], int cantidad, char ciudades[][MAX_CODIGO], int *num_ciudades) {
    // Se reinicia el contador de ciudades
    *num_ciudades = 0;

    for (int i = 0; i < cantidad; i++) {
        // Obtenemos (o asignamos) el índice de cada ciudad
        int idx_origen = obtener_indice(ciudades, num_ciudades, vuelos[i].origen);
        int idx_destino = obtener_indice(ciudades, num_ciudades, vuelos[i].destino);

        // Asignamos la distancia en la matriz de adyacencia (grafo no dirigido)
        grafo[idx_origen][idx_destino] = vuelos[i].distancia;
        grafo[idx_destino][idx_origen] = vuelos[i].distancia;
    }
}

// Aplicar el algoritmo de Dijkstra para encontrar las distancias mínimas desde un nodo inicial
void dijkstra(int grafo[MAX_NODOS][MAX_NODOS], int num_nodos, int inicio, int distancias[], int predecesores[]) {
    bool visitado[MAX_NODOS] = {false};

    // Inicialización: todas las distancias son infinitas y no hay predecesores
    for (int i = 0; i < num_nodos; i++) {
        distancias[i] = INF;
        predecesores[i] = -1;
    }

    distancias[inicio] = 0;

    for (int i = 0; i < num_nodos; i++) {
        int u = -1;

        // Encuentra el nodo no visitado con la distancia mínima
        for (int j = 0; j < num_nodos; j++) {
            if (!visitado[j] && (u == -1 || distancias[j] < distancias[u])) {
                u = j;
            }
        }

        // Si u quedó sin asignar, no hay más nodos alcanzables
        if (u == -1) break;

        visitado[u] = true;

        // Revisa los vecinos del nodo actual
        for (int v = 0; v < num_nodos; v++) {
            // Si hay conexión y el nuevo camino es más corto
            if (grafo[u][v] > 0 && distancias[u] + grafo[u][v] < distancias[v]) {
                distancias[v] = distancias[u] + grafo[u][v];
                predecesores[v] = u;
            }
        }
    }
}