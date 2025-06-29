#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

enum RoomType {
    EMPTY,
    NORMAL,
    START,
    BOSS,
    TREASURE,
    SHOP,
    TRAP
};

struct Room {
    int x, y;
    RoomType type = EMPTY;
    bool visited = false;
    bool connections[4] = {false, false, false, false};
};

class Map {
public:
    Map(int width, int height);
    void reset();
    void setRoom(int x, int y, RoomType type);
    Room& getRoom(int x, int y);
    const Room& getRoom(int x, int y) const;
    bool inBounds(int x, int y) const;
    void print() const;
    int getWidth() const;
    int getHeight() const;

private:
    int width, height;
    std::vector<std::vector<Room>> grid;
};

#endif
