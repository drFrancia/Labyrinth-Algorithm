#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>
#include <cmath> // Para sqrt y pow

using namespace std;

// Función para calcular la distancia euclidiana
double calcularDistancia(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

bool resolverLaberinto(int** matriz, int x, int y, int filas, int columnas, int salidaX, int salidaY) {
    // Caso base: Si el jugador ha llegado a la salida
    if (matriz[y][x] == 3) { // 3 marca la salida
        matriz[y][x] = 2; // Marca la salida como parte del camino
        return true;
    }

    // Verifica si la posición actual es válida
    if (x >= 0 && x < columnas && y >= 0 && y < filas && (matriz[y][x] == 0 || matriz[y][x] == 3)) {
        int temp = matriz[y][x];
        matriz[y][x] = 2; // Marca el camino

        // Imprimir el laberinto después de cada paso
        system("clear"); // usa "cls" en Windows
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < columnas; j++) {
                if (matriz[i][j] == 1) {
                    cout << "#";
                } else if (matriz[i][j] == 2) {
                    cout << "P";
                } else if (matriz[i][j] == 3) {
                    cout << "S";
                } else {
                    cout << ":";
                }
            }
            cout << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(100)); // Pausa de 100 ms para animación

        // Generar una lista de direcciones con distancias a la salida
        vector<pair<double, pair<int, int>>> direcciones = {
            {calcularDistancia(x + 1, y, salidaX, salidaY), {1, 0}},
            {calcularDistancia(x, y + 1, salidaX, salidaY), {0, 1}},
            {calcularDistancia(x - 1, y, salidaX, salidaY), {-1, 0}},
            {calcularDistancia(x, y - 1, salidaX, salidaY), {0, -1}}
        };

        // Ordenar las direcciones según la distancia a la salida
        sort(direcciones.begin(), direcciones.end());

        for (auto dir : direcciones) {
            int nuevoX = x + dir.second.first;
            int nuevoY = y + dir.second.second;
            if (resolverLaberinto(matriz, nuevoX, nuevoY, filas, columnas, salidaX, salidaY))
                return true;
        }

        // Desmarca el camino (backtracking)
        matriz[y][x] = temp;

        // Imprimir el laberinto después de cada paso de backtracking
        system("clear"); // usa "cls" en Windows
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < columnas; j++) {
                if (matriz[i][j] == 1) {
                    cout << "#";
                } else if (matriz[i][j] == 2) {
                    cout << "P";
                } else if (matriz[i][j] == 3) {
                    cout << "S";
                } else {
                    cout << ":";
                }
            }
            cout << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(100)); // Pausa de 100 ms para animación

        return false;
    }

    return false;
}

void generarLaberinto(int filas, int columnas, float densidad) {
    int** matriz;
    matriz = new int* [filas];

    int FParedes = densidad * 8;
    densidad = (filas * columnas * densidad) / 4;

    for (int i = 0; i < filas; i++) {
        matriz[i] = new int[columnas];
    }
    // valores
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            if (i == 0 || j == 0 || i == filas - 1 || j == columnas - 1) {
                matriz[i][j] = 1;
            } else {
                matriz[i][j] = 0;
            }
        }
    }
    // laberinto
    srand(time(NULL));
    for (int i = 0; i < densidad; i++) {
        // Elegimos el punto que será pintado en 1 para construir las paredes
        int x = rand() % (columnas - 4) + 2; // 2 18
        x = (x / 2) * 2;
        int y = rand() % (filas - 4) + 2;
        y = (y / 2) * 2;
        matriz[y][x] = 1;
        for (int j = 0; j < FParedes; j++) {
            // Elegimos los cuatro puntos para ver si pintamos la pared o no
            int mx[4] = { x, x, x + 2, x - 2 }; // Cuando se trabaje sobre x, y será más dos y menos dos
            int my[4] = { y + 2, y - 2, y, y }; // Cuando se trabaje sobre y, x será más dos y menos dos
            int r = rand() % 4;

            // Verificamos que los índices están dentro de los límites de la matriz
            if (matriz[my[r]][mx[r]] == 0) {
                matriz[my[r]][mx[r]] = 1;
                matriz[my[r] + (y - my[r]) / 2][mx[r] + (x - mx[r]) / 2] = 1;
            }
        }
    }

    // Generar una salida aleatoria
    int salidaX, salidaY;
    while (true) {
        salidaX = rand() % (columnas - 2) + 1;
        salidaY = rand() % (filas - 2) + 1;
        if (matriz[salidaY][salidaX] == 0) {
            matriz[salidaY][salidaX] = 3; // Marca la salida con un 3
            break;
        }
    }

    // Resolver el laberinto
    if (resolverLaberinto(matriz, 1, 1, filas, columnas, salidaX, salidaY)) {
        cout << "¡Felicidades! Se ha encontrado una solución." << endl;
    } else {
        cout << "No se ha encontrado una solución." << endl;
    }

    // Liberar memoria
    for (int i = 0; i < filas; i++) {
        delete[] matriz[i];
    }
    delete[] matriz;
}

int main() {
    generarLaberinto(29, 19, 0.5);
    return 0;
}