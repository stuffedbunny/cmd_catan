#ifndef __TILE__
#define __TILE__

#include <string>
#include "globals.h"
#include "vertex.h"

using namespace std;

class Tile {
    int id, val; //val = dice roll
    int resource;
    bool isRobbed;
    Vertex* vertices[6];
public:
    Tile(int id, int val, int resource, Vertex* a, Vertex* b, Vertex* c, Vertex* d, Vertex* e, Vertex* f);
    string getVal();
    string toStrForm();

    void setResource(int r);
    void setVal(int v);
    string toStateForm();

    void noRobber();
    void yesRobber();
    vector<bool> getResidents();
    vector<vector<int>> diceRolled(int num);
};

#endif
