#ifndef __PLAYER__
#define __PLAYER__

#include <string>
#include <vector>
#include "globals.h"
#include "vertex.h"
#include "edge.h"

using namespace std;

class Player {
    int resources[5] = {0, 0, 0, 0, 0};
    bool isFair = true;
    int playerNum;

    int buildingPts = 0;
    int armySize = 0;
    int longestRoad = 0;
    vector<int> devCards = {};

    vector<Vertex*> vertices;
    vector<Edge*> edges;

public:
    Player(int n);
    string getStatus() const;
    string getSettlements() const;
    string getState() const;

    bool getFairness() const;
    void setFair();
    void setLoaded();

    int getPts() const;
    void addPoint();

    int resourceCount(int resource) const;
    int resourceSum() const;

    bool hasResources(const vector<pair<int, int>> lst) const;
    void resourceGain(const vector<pair<int, int>> lst);
    void resourceLoss(const vector<pair<int, int>> lst);

    void resourceGain(int resource, int n);
    void resourceLoss(int resource, int n);
    int resourceLossRandom();
    vector<int> richTax(int loss);

    void addVertex(Vertex* v);
    void addEdge(Edge* e);
};

#endif
