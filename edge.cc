#include "edge.h"

using namespace std;

Edge::Edge(int id): buildable{id} {}
Edge::~Edge() {}

string Edge::toStrForm() const {
    if (getOwner() != -1) return " " + playerCol[getOwner()];
    return (getId() < 10) ? " "+to_string(getId()) : to_string(getId());
}

void Edge::build(int p) {
    setOwner(p);
}

bool Edge::adjValid(int p) const {
    for (auto i : getAdj()) { // neighbouring vertices
        for (int j : i->getAdjOwners()) { // vertices' edges' owners
            if (j == p) return true; //adjacent road exists
        }
        if (i->getOwner() == p) return true; //adjacent building exists
    }
    return false;
}

vector<int> Edge::getAdjRoads() const {
    vector<int> out;
    for (auto v : getAdj()) {
        for (auto e : v->getAdj()) {
            if (e->getOwner() != getOwner()) continue;
            if (e->getId() == getId()) continue;
            out.emplace_back(e->getId());
        }
    }
    return out;
 }