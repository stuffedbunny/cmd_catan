#include "buildable.h"

using namespace std;

buildable::buildable(int id): id{id} {};

int buildable::getOwner() const {
    return owner;
}

void buildable::setOwner(int p) {
    owner = p;
}

int buildable::getId() const {
    return id;
}

void buildable::addAdj(buildable* c) {
    adj.emplace_back(c);
}

vector<buildable*> buildable::getAdj() const {
    return adj;
}

vector<int> buildable::getAdjOwners() const {
    vector<int> out;
    for (auto i : adj) {
        out.emplace_back(i->getOwner());
    }
    return out;
}
