// Codigo hecho por José Fernando Hernández Vera
#include "DFSGenerator.h"
#include <vector>
#include <utility>
#include <random>
#include <algorithm>
#include <ctime>

namespace {
    std::mt19937 rng(static_cast<unsigned>(time(nullptr)));

    const std::vector<std::pair<int, int>> directions = {
        {0, -1}, // arriba
        {1, 0},  // derecha
        {0, 1},  // abajo
        {-1, 0}  // izquierda
    };

    int directionIndex(const std::pair<int, int>& dir) {
        if (dir == directions[0]) return 0;
        if (dir == directions[1]) return 1;
        if (dir == directions[2]) return 2;
        if (dir == directions[3]) return 3;
        return -1;
    }

    void dfs(Map& map, int x, int y, int depth, int maxDepth) {
        if (depth > maxDepth) return;

        Room& room = map.getRoom(x, y);
        room.visited = true;
        room.type = (depth == 0) ? START : NORMAL;

        std::vector<std::pair<int, int>> dirs = directions;
        std::shuffle(dirs.begin(), dirs.end(), rng);

        for (const auto& dir : dirs) {
            int nx = x + dir.first;
            int ny = y + dir.second;
            if (!map.inBounds(nx, ny)) continue;

            Room& neighbor = map.getRoom(nx, ny);
            if (!neighbor.visited) {
                dfs(map, nx, ny, depth + 1, maxDepth);

                if (neighbor.visited && neighbor.type != EMPTY) {
                    int dirIdx = directionIndex(dir);
                    int opposite = (dirIdx + 2) % 4;

                    room.connections[dirIdx] = true;
                    neighbor.connections[opposite] = true;
                }
            }
        }
    }

    void placeSpecialRooms(Map& map) {
        std::vector<Room*> candidates;
        for (int y = 0; y < map.getHeight(); ++y)
            for (int x = 0; x < map.getWidth(); ++x) {
                Room& r = map.getRoom(x, y);
                if (r.visited && r.type == NORMAL)
                    candidates.push_back(&r);
            }

        std::shuffle(candidates.begin(), candidates.end(), rng);
        size_t i = 0;

        std::uniform_int_distribution<> treasures_dist(1, 4);
        std::uniform_int_distribution<> traps_dist(1, 4);
        int treasures = treasures_dist(rng);
        int traps = traps_dist(rng);
        int shops = 1;

        for (int j = 0; j < treasures && i < candidates.size(); ++j)
            candidates[i++]->type = TREASURE;
        for (int j = 0; j < shops && i < candidates.size(); ++j)
            candidates[i++]->type = SHOP;
        for (int j = 0; j < traps && i < candidates.size(); ++j)
            candidates[i++]->type = TRAP;
    }
}

void DFSGenerator::generate(Map& map, int maxDepth) {
    int cx = map.getWidth() / 2;
    int cy = map.getHeight() / 2;
    dfs(map, cx, cy, 0, maxDepth);

    Room* bossRoom = nullptr;
    int maxDist = -1;
    for (int y = 0; y < map.getHeight(); ++y)
        for (int x = 0; x < map.getWidth(); ++x) {
            Room& r = map.getRoom(x, y);
            if (r.visited && r.type == NORMAL) {
                int dist = abs(x - cx) + abs(y - cy);
                if (dist > maxDist) {
                    maxDist = dist;
                    bossRoom = &r;
                }
            }
        }

    if (bossRoom) bossRoom->type = BOSS;

    placeSpecialRooms(map);
}