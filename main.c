#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 100
#define MAX_CODIGO 10
#define MAX_NODOS 100
typedef struct {
    char origen[MAX_CODIGO];
    char destino[MAX_CODIGO];
    int distancia;
} Vuelo;


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


// Algoritmo de Dijkstra
void dijkstra(int grafo[MAX_NODOS][MAX_NODOS], int num_nodos, int inicio) {
    int dist[MAX_NODOS], visitado[MAX_NODOS] = {0};
    for (int i = 0; i < num_nodos; i++) dist[i] = 1e9;
    dist[inicio] = 0;

    for (int i = 0; i < num_nodos; i++) {
        int u = -1;
        for (int j = 0; j < num_nodos; j++)
            if (!visitado[j] && (u == -1 || dist[j] < dist[u])) u = j;
        if (dist[u] == 1e9) break;
        visitado[u] = 1;
        for (int v = 0; v < num_nodos; v++)
            if (grafo[u][v] && dist[u] + grafo[u][v] < dist[v])
                dist[v] = dist[u] + grafo[u][v];
    }

    printf("Distancias más cortas desde el nodo %d:\n", inicio);
    for (int i = 0; i < num_nodos; i++)
        printf("A %d: %d\n", i, dist[i]);
}


int main() {
    FILE *archivo = fopen("rutas.txt", "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return 1;
    }

    Vuelo *vuelos = NULL;
    int capacidad = 10, cantidad = 0;
    vuelos = malloc(capacidad * sizeof(Vuelo));
    if (vuelos == NULL) {
        printf("Error de memoria.\n");
        fclose(archivo);
        return 1;
    }

    char origen[MAX_CODIGO], destino[MAX_CODIGO];
    int distancia;

    while (fscanf(archivo, "%s %s %d", origen, destino, &distancia) == 3) {
        if (cantidad == capacidad) {
            capacidad *= 2;
            vuelos = realloc(vuelos, capacidad * sizeof(Vuelo));
            if (vuelos == NULL) {
                printf("Error de memoria.\n");
                fclose(archivo);
                return 1;
            }
        }
        strcpy(vuelos[cantidad].origen, origen);
        strcpy(vuelos[cantidad].destino, destino);
        vuelos[cantidad].distancia = distancia;
        cantidad++;


        char ciudades[MAX_NODOS][MAX_CODIGO];
    int num_ciudades = 0;
    int grafo[MAX_NODOS][MAX_NODOS] = {0};

    // Construir el grafo
    for (int i = 0; i < cantidad; i++) {
        int idx_origen = obtener_indice(ciudades, &num_ciudades, vuelos[i].origen);
        int idx_destino = obtener_indice(ciudades, &num_ciudades, vuelos[i].destino);
        grafo[idx_origen][idx_destino] = vuelos[i].distancia;
        // Si es bidireccional, también:
        // grafo[idx_destino][idx_origen] = vuelos[i].distancia;
    }
    // Ejecutar Dijkstra desde el primer nodo
    if (num_ciudades > 0) {
        printf("Ciudades:\n");
        for (int i = 0; i < num_ciudades; i++) {
            printf("%s ", ciudades[i]);
        }
        printf("\n");
        dijkstra(grafo, num_ciudades, 0); // Cambiar 0 por el índice del nodo de inicio deseado
    } else {
        printf("No se encontraron ciudades.\n");
    }
    free(vuelos);
    vuelos = NULL;  
    
    return 0;
}
 
    

   
    free(vuelos);
    fclose(archivo);
    return 0;
}