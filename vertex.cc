#include "vertex.h"

using namespace std;

Vertex::Vertex(int id): buildable{id} {}
Vertex::~Vertex() {}

string Vertex::toStrForm() const {
    if (getOwner() != -1) return playerCol[getOwner()] + getStageChar();
    return getId() < 10 ? " "+to_string(getId()) : to_string(getId());
}

bool Vertex::adjValid(int p) const { return adjValid(p, false); }

bool Vertex::adjValid(int p, bool isFirst) const {
    for (auto i : getAdj()) { // neighbouring edges
        for (int j : i->getAdjOwners()) { // vertices' vertices' owners
            if (j != -1) return false; // existing housing too close
        }
    }

    // -- no cities are too close --
    for (auto i : getAdjOwners()) {
        if (i == p) return true; //has adjacent road
    }

    // -- no adjacent road --
    return isFirst; //allow only if first
}

int Vertex::getStage() const { return stage; }

string Vertex::getStageChar() const { string s[4] = {" ", "S", "C", "/"}; return s[stage]; }

void Vertex::build(int p) {
    setOwner(p); ++stage;
}

void Vertex::improve() {
    ++stage;
}
