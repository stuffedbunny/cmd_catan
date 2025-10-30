#include "player.h"

Player::Player(int n): playerNum{n} {}

string Player::getStatus() const {
    string statusString = playerCol[playerNum] + " has " + to_string(getPts()) + " victory points,";
    for (int i = 0; i < 5; i++) {
        statusString += " " + to_string(resources[i]) + " " + resourceLower[i];
    }
    return statusString;
}

string Player::getSettlements() const {
    string out = "";
    for (Vertex* v : vertices) {
        if (out != "") out += "\n";
        out += to_string(v->getId()) + " " + v->getStageChar();
    }
    return out;
}

string Player::getState() const {
    string out = "";
    for (int i = 0; i < 5; i++) out += to_string(resources[i]) + " ";
    out += "r ";
    for (Edge* e : edges) out += to_string(e->getId()) + " ";
    out += "h ";
    for (Vertex* v : vertices) out += to_string(v->getId()) + " " + v->getStageChar() + " ";
    return out;
}

bool Player::getFairness() const { return isFair; }
void Player::setFair() { isFair = true; }
void Player::setLoaded() { isFair = false; }

int Player::getPts() const { return buildingPts; }
void Player::addPoint() { buildingPts += 1; }

int Player::resourceCount(int resource) const { return resources[resource]; }
int Player::resourceSum() const {
    int sum = 0; for (int i : resources) { sum += i; } return sum;
}

bool Player::hasResources(const vector<pair<int, int>> lst) const {
    for (auto i : lst) {
        if (resources[i.first] < i.second) return false;
    }
    return true;
}
void Player::resourceGain(const vector<pair<int, int>> lst) {
    for (auto i : lst) { resources[i.first] += i.second; }
}
void Player::resourceLoss(const vector<pair<int, int>> lst) {
    for (auto i : lst) { resources[i.first] -= i.second; }
}

void Player::resourceGain(int resource, int n) { resources[resource] += n; }
void Player::resourceLoss(int resource, int n) { resources[resource] -= n; }
int Player::resourceLossRandom() {
    int sum = resourceSum();
    sum = 1 + (rand() % sum);
    for (int i = 0; i < 4; i++) {
        sum -= resources[i];
        if (sum <= 0) {
            resources[i] -= 1; return i;
        }
    }
    resources[4] -= 1; return 4;
}
vector<int> Player::richTax(int loss) {
    vector<int> losses = {0,0,0,0,0};
    while (loss > 0) {
        losses[resourceLossRandom()]++;
        loss--;
    }
    return losses;
}
void Player::addVertex(Vertex* v) {
    vertices.emplace_back(v);
    addPoint();
}
void Player::addEdge(Edge* e) { edges.emplace_back(e); }
