#ifndef __GRAPH__
#define __GRAPH__

#include <string>
#include <vector>
#include "globals.h"

using namespace std;

class buildable {
    vector<buildable*> adj;
    int id;
    int owner = -1;
public:
    buildable(int id);
    int getOwner() const;
    void setOwner(int p);
    int getId() const;

    virtual bool adjValid(int p) const = 0;

    virtual std::string toStrForm() const = 0;

    void addAdj(buildable* c);
    vector<buildable*> getAdj() const;
    vector<int> getAdjOwners() const;
};

#endif
