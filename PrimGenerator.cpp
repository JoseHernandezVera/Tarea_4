#include "PrimGenerator.h"
#include <vector>
#include <utility>
#include <random>
#include <algorithm>
#include <ctime>

namespace {
    std::mt19937 rng(static_cast<unsigned>(time(nullptr)));

    const std::vector<std::pair<int, int>> directions = {
        {0, -1}, {1, 0}, {0, 1}, {-1, 0}
    };

    int directionIndex(const std::pair<int, int>& dir) {
        for (int i = 0; i < 4; ++i)
            if (dir == directions[i]) return i;
        return -1;
    }

    void placeSpecialRooms(Map& map, int treasures = 1, int shops = 1, int traps = 2) {
        std::vector<Room*> candidates;
        for (int y = 0; y < map.getHeight(); ++y)
            for (int x = 0; x < map.getWidth(); ++x) {
                Room& r = map.getRoom(x, y);
                if (r.visited && r.type == NORMAL)
                    candidates.push_back(&r);
            }

        std::shuffle(candidates.begin(), candidates.end(), rng);
        int i = 0;

        for (int j = 0; j < treasures && i < candidates.size(); ++j)
            candidates[i++]->type = TREASURE;
        for (int j = 0; j < shops && i < candidates.size(); ++j)
            candidates[i++]->type = SHOP;
        for (int j = 0; j < traps && i < candidates.size(); ++j)
            candidates[i++]->type = TRAP;
    }
}

void PrimGenerator::generate(Map& map, int maxRooms) {
    int cx = map.getWidth() / 2;
    int cy = map.getHeight() / 2;

    map.getRoom(cx, cy).type = START;
    map.getRoom(cx, cy).visited = true;

    std::vector<std::pair<int, int>> frontier;

    for (const auto& d : directions) {
        int nx = cx + d.first;
        int ny = cy + d.second;
        if (map.inBounds(nx, ny))
            frontier.emplace_back(nx, ny);
    }

    int roomsCreated = 1;

    while (!frontier.empty() && roomsCreated < maxRooms) {
        std::uniform_int_distribution<> dist(0, frontier.size() - 1);
        int idx = dist(rng);
        auto [x, y] = frontier[idx];
        std::swap(frontier[idx], frontier.back());
        frontier.pop_back();

        if (map.getRoom(x, y).visited) continue;

        std::vector<std::pair<int, int>> neighbors;
        for (const auto& d : directions) {
            int nx = x + d.first;
            int ny = y + d.second;
            if (map.inBounds(nx, ny) && map.getRoom(nx, ny).visited)
                neighbors.emplace_back(nx, ny);
        }

        if (!neighbors.empty()) {
            std::uniform_int_distribution<> ndist(0, neighbors.size() - 1);
            auto [px, py] = neighbors[ndist(rng)];

            int dirTo = directionIndex({x - px, y - py});
            int dirFrom = (dirTo + 2) % 4;

            map.getRoom(px, py).connections[dirTo] = true;
            map.getRoom(x, y).connections[dirFrom] = true;
        }

        map.getRoom(x, y).visited = true;
        map.getRoom(x, y).type = NORMAL;
        ++roomsCreated;

        for (const auto& d : directions) {
            int nx = x + d.first;
            int ny = y + d.second;
            if (map.inBounds(nx, ny) && !map.getRoom(nx, ny).visited)
                frontier.emplace_back(nx, ny);
        }
    }

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
