#ifndef __BOARD__
#define __BOARD__

#include <string>
#include "globals.h"
#include "edge.h"
#include "tile.h"

using namespace std;

class Board {
    vector<unique_ptr<Vertex>> vertices;
    vector<unique_ptr<Edge>> edges;
    vector<unique_ptr<Tile>> tiles;

    int robberPos = 4;
public:
    Board();
    ~Board();

    string getStrVertex(int i) const;
    string getStrEdge(int i) const;
    string getStrTile(int i) const;

    Tile* getTile(int i) const;
    Vertex* getVertex(int i) const;
    Edge* getEdge(int i) const;

    vector<vector<int>> diceRolled(int num) const;
    int getRobberPos() const;
    vector<bool> moveRobber(int dest);
};

#endif
