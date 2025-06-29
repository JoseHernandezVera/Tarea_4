#include "Map.h"
#include <iostream>
#include <iomanip>

Map::Map(int width, int height) : width(width), height(height) {
    grid.resize(height, std::vector<Room>(width));
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x) {
            grid[y][x].x = x;
            grid[y][x].y = y;
        }
}

void Map::reset() {
    for (auto& row : grid) {
        for (auto& room : row) {
            room.type = EMPTY;
            room.visited = false;
            for (int i = 0; i < 4; ++i) room.connections[i] = false;
        }
    }
}

void Map::setRoom(int x, int y, RoomType type) {
    if (inBounds(x, y)) {
        grid[y][x].type = type;
    }
}

Room& Map::getRoom(int x, int y) {
    return grid[y][x];
}

const Room& Map::getRoom(int x, int y) const {
    return grid[y][x];
}

bool Map::inBounds(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

int Map::getWidth() const {
    return width;
}

int Map::getHeight() const {
    return height;
}

void Map::print() const {
    for (int y = 0; y < height; ++y) {
        // Primera línea: habitaciones y conexiones horizontales
        for (int x = 0; x < width; ++x) {
            const Room& r = grid[y][x];
            char symbol = 'O';
            switch (r.type) {
                case START: symbol = 'S'; break;
                case BOSS: symbol = 'B'; break;
                case TREASURE: symbol = 'T'; break;
                case SHOP: symbol = '$'; break;
                case TRAP: symbol = 'X'; break;
                case NORMAL: symbol = '#'; break;
                default: break;
            }
            std::cout << symbol;

            // Conexión a la derecha: SOLO si la sala derecha es válida
            if (x < width - 1 && r.connections[1]) {
                const Room& right = grid[y][x + 1];
                if (right.visited && right.type != EMPTY)
                    std::cout << "---";
                else
                    std::cout << "   ";
            } else {
                std::cout << "   ";
            }
        }
        std::cout << "\n";

        // Conexión hacia abajo: SOLO si la sala de abajo es válida
        for (int x = 0; x < width; ++x) {
            const Room& r = grid[y][x];
            if (y < height - 1 && r.connections[2]) {
                const Room& down = grid[y + 1][x];
                if (down.visited && down.type != EMPTY)
                    std::cout << "|   ";
                else
                    std::cout << "    ";
            } else {
                std::cout << "    ";
            }
        }
        std::cout << "\n";
    }
}
