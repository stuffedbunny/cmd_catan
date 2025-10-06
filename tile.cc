#include "tile.h"

using namespace std;

Tile::Tile(int id, int val, int resource, Vertex* a, Vertex* b, Vertex* c, Vertex* d, Vertex* e, Vertex* f): 
    id{id}, val{val}, resource{resource}, isRobbed{false}, vertices{{a}, {b}, {c}, {d}, {e}, {f}} {}

string Tile::getVal() {
    if (id == 7) return "  ";
    return (val < 10 ? " "+to_string(val) : to_string(val));
}

void Tile::setResource(int r) {
    resource = r;
}

void Tile::setVal(int v) {
    val = v;
}

string Tile::toStrForm() {
    string baseStr = resourcePrintFmt[resource] + "  " + getVal();
    return (isRobbed ? "(R) " + baseStr : "  " + baseStr + "  ");
}

string Tile::toStateForm() {
    return to_string(resource) + " " + to_string(val);
}

void Tile::noRobber() {
    isRobbed = false;
}
void Tile::yesRobber() {
    isRobbed = true;
}

vector<bool> Tile::getResidents() {
    vector<bool> out = {false, false, false, false};
    for (int i = 0; i < 6; i++) {
        if (vertices[i]->getOwner() == -1) continue;
        out[vertices[i]->getOwner()] = true;
    }
    return out;
}

vector<vector<int>> Tile::diceRolled(int num) {
    if (num != this->val || this->isRobbed) return {};
    vector<vector<int>> out;
    for (int i = 0; i < 6; i++) {
        vector<int> v = {vertices[i]->getOwner(), resource, vertices[i]->getStage()};
        out.emplace_back(v);
    }
    return out;
}
