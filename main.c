#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 100
#define MAX_CODIGO 10
#define MAX_NODOS 100

// Estructura para almacenar la información de cada vuelo
typedef struct {
    char origen[MAX_CODIGO];
    char destino[MAX_CODIGO];
    int distancia;
} Vuelo;

// Asigna un índice único a cada ciudad (si ya existe, retorna su índice)
int obtener_indice(char ciudades[][MAX_CODIGO], int *num_ciudades, const char *nombre) {
    for (int i = 0; i < *num_ciudades; i++) {
        if (strcmp(ciudades[i], nombre) == 0)
            return i;
    }
    // Si la ciudad no existe, la agrega y retorna el nuevo índice
    strcpy(ciudades[*num_ciudades], nombre);
    (*num_ciudades)++;
    return (*num_ciudades) - 1;
}

// Lee los vuelos desde el archivo y los almacena en un arreglo dinámico
int leer_vuelos(const char *filename, Vuelo **vuelos) {
    FILE *archivo = fopen(filename, "r");
    if (!archivo) {
        perror("Error al abrir el archivo");
        return -1;
    }

    int capacidad = 10;
    int cantidad = 0;
    *vuelos = malloc(capacidad * sizeof(Vuelo));
    if (!*vuelos) {
        perror("Error al asignar memoria para los vuelos");
        fclose(archivo);
        return -1;
    }

    char linea[MAX_LINE];
    while (fgets(linea, sizeof(linea), archivo)) {
        if (linea[0] == '\n' || linea[0] == '\r') continue; // Ignora líneas vacías
        if (cantidad == capacidad) { // Redimensiona el arreglo si es necesario
            capacidad *= 2;
            Vuelo *tmp = realloc(*vuelos, capacidad * sizeof(Vuelo));
            if (!tmp) {
                perror("Error al reasignar memoria para los vuelos");
                free(*vuelos);
                fclose(archivo);
                return -1;
            }
            *vuelos = tmp;
        }
        // Lee los datos de la línea
        int leidos = sscanf(linea, "%s %s %d",
            (*vuelos)[cantidad].origen,
            (*vuelos)[cantidad].destino,
            &(*vuelos)[cantidad].distancia);
        if (leidos == 3) {
            cantidad++;
        }
    }

    fclose(archivo);
    return cantidad;
}

// Algoritmo de Dijkstra para encontrar la ruta más corta y reconstruir el camino
void dijkstra(int grafo[MAX_NODOS][MAX_NODOS], int num_nodos, int inicio, int fin, int predecesor[MAX_NODOS], int *distancia_final) {
    int dist[MAX_NODOS], visitado[MAX_NODOS] = {0};
    for (int i = 0; i < num_nodos; i++) {
        dist[i] = 1e9;         // Inicializa todas las distancias como "infinito"
        predecesor[i] = -1;    // Sin predecesor al inicio
    }
    dist[inicio] = 0;          // La distancia al nodo de inicio es 0

    for (int i = 0; i < num_nodos; i++) {
        int u = -1;
        // Busca el nodo no visitado con menor distancia
        for (int j = 0; j < num_nodos; j++)
            if (!visitado[j] && (u == -1 || dist[j] < dist[u])) u = j;
        if (dist[u] == 1e9) break; // Si no hay más nodos alcanzables, termina
        visitado[u] = 1;           // Marca el nodo como visitado
        // Actualiza las distancias a los vecinos del nodo actual
        for (int v = 0; v < num_nodos; v++) {
            if (grafo[u][v] && dist[u] + grafo[u][v] < dist[v]) {
                dist[v] = dist[u] + grafo[u][v];
                predecesor[v] = u; // Guarda el predecesor para reconstruir la ruta
            }
        }
    }
    *distancia_final = dist[fin]; // Guarda la distancia final al destino
}

// Imprime la ruta óptima desde el nodo de inicio al nodo de fin
void imprimir_ruta(int predecesor[], int idx_inicio, int idx_fin, char ciudades[][MAX_CODIGO]) {
    int ruta[MAX_NODOS];
    int len = 0;
    int actual = idx_fin;
    // Reconstruye la ruta desde el destino hasta el origen usando el arreglo de predecesores
    while (actual != -1) {
        ruta[len++] = actual;
        if (actual == idx_inicio) break;
        actual = predecesor[actual];
    }
    if (actual != idx_inicio) {
        printf("No hay ruta óptima.\n");
        return;
    }
    printf("Ruta óptima: ");
    // Imprime la ruta en orden correcto (de origen a destino)
    for (int i = len - 1; i >= 0; i--) {
        printf("%s", ciudades[ruta[i]]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    // Verifica que se reciban los argumentos correctos
    if (argc != 4) {
        printf("Uso: %s archivo_rutas ciudad_origen ciudad_destino\n", argv[0]);
        return 1;
    }

    Vuelo *vuelos = NULL;
    // Lee los vuelos desde el archivo especificado
    int cantidad = leer_vuelos(argv[1], &vuelos);
    if (cantidad <= 0) {
        printf("No se encontraron vuelos en el archivo.\n");
        return 1;
    }

    char ciudades[MAX_NODOS][MAX_CODIGO];
    int num_ciudades = 0;
    int grafo[MAX_NODOS][MAX_NODOS] = {0};

    // Construye el grafo de adyacencia usando los vuelos leídos
    for (int i = 0; i < cantidad; i++) {
        int idx_origen = obtener_indice(ciudades, &num_ciudades, vuelos[i].origen);
        int idx_destino = obtener_indice(ciudades, &num_ciudades, vuelos[i].destino);
        grafo[idx_origen][idx_destino] = vuelos[i].distancia;
    }

    // Busca los índices de la ciudad de origen y destino
    int idx_inicio = -1, idx_fin = -1;
    for (int i = 0; i < num_ciudades; i++) {
        if (strcmp(ciudades[i], argv[2]) == 0) idx_inicio = i;
        if (strcmp(ciudades[i], argv[3]) == 0) idx_fin = i;
    }
    if (idx_inicio == -1 || idx_fin == -1) {
        printf("Ciudad de origen o destino no encontrada.\n");
        free(vuelos);
        return 1;
    }

    int predecesor[MAX_NODOS];
    int distancia_final;
    // Ejecuta Dijkstra para encontrar la ruta más corta
    dijkstra(grafo, num_ciudades, idx_inicio, idx_fin, predecesor, &distancia_final);

    // Imprime la ruta y la distancia total
    if (distancia_final == 1e9) {
        printf("No hay ruta optima.\n");
    } else {
        imprimir_ruta(predecesor, idx_inicio, idx_fin, ciudades);
        printf("Distancia total: %d km\n", distancia_final);
    }

    free(vuelos);
    return 0;
}