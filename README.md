Integrantes: Valentin Malov y Christian Looff Harwardt

Este programa recibe un archivo txt con la siguiente estrucura de datos: CIUDAD_ORIGEN CIUDAD_DESTINO DISTANCIA_EN_KM. A partir de estos datos, se construye un grafo no dirigido, donde cada nodo representa una ciudad y cada arista representa una conexión directa entre ellas con un peso igual a la distancia. 

El objetivo del programa es determinar la ruta óptima (es decir, la más corta en términos de distancia) entre dos ciudades dadas, utilizando el algoritmo de Dijkstra.

Para utilizar el programa:

1) Asegurate de tener un archivo "rutas.txt" en la misma carpeta del ejecutable
2) Compila el programa con mingw32-make (o make)
3) Ejecuta el programa desde la terminal con el siguiente formato:

./tarea3 rutas.txt CIUDAD_ORIGEN CIUDAD_DESTINO    

Ejemplo:  

./tarea3 rutas.txt SCL MIA



ESTRUCTURA DEL PROYECTO

grupo11-T23/
├── main.c
├── Makefile
├── rutas.txt
├── README.md
├── .vscode/
│   ├── launch.json
│   └── tasks.json
└── modules/
    ├── processor.c
    ├── processor.h
    ├── validator.c
    └── validator.h