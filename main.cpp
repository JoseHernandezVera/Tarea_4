// Codigo hecho por José Fernando Hernández Vera
#include "Map.h"
#include "DFSGenerator.h"
#include <iostream>

int contarHabitaciones(const Map& map) {
    int count = 0;
    for (int y = 0; y < map.getHeight(); ++y)
        for (int x = 0; x < map.getWidth(); ++x)
            if (map.getRoom(x, y).visited)
                ++count;
    return count;
}

int contarTipo(const Map& map, RoomType tipo) {
    int count = 0;
    for (int y = 0; y < map.getHeight(); ++y)
        for (int x = 0; x < map.getWidth(); ++x)
            if (map.getRoom(x, y).type == tipo)
                ++count;
    return count;
}

int main() {
    int width, height, valor;

    std::cout << "Ancho del mapa: ";
    std::cin >> width;

    std::cout << "Alto del mapa: ";
    std::cin >> height;

    std::cout << "Profundidad max (DFS): ";
    std::cin >> valor;

    std::cout << "\n--- Generando mapa con DFS ---\n";
    Map mapDFS(width, height);
    DFSGenerator::generate(mapDFS, valor);
    mapDFS.print();

    std::cout << "Habitaciones totales: " << contarHabitaciones(mapDFS) << "\n";
    std::cout << "Tesoros: " << contarTipo(mapDFS, TREASURE) << "\n";
    std::cout << "Trampas: " << contarTipo(mapDFS, TRAP) << "\n";
    std::cout << "Camino principal (profundidad): " << valor << "\n";

    return 0;
}
