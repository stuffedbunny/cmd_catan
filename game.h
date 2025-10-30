#ifndef __GAME__
#define __GAME__

#include "globals.h"
#include "board.h"
#include "view.h"
#include "player.h"
#include "rng.h"
#include "DevCards.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

class Game {
    vector<unique_ptr<Player>> players;

    unique_ptr<RNG> rng;
    unique_ptr<Board> board;
    unique_ptr<View> view;
    unique_ptr<DevCards> devCards;

    int turn = 0;
    int winner = 0;
    bool stopProgram = false;

    bool legalRoad(int edge) const;
    void buildRoad(int edge) const;
    int getRoadLength(vector<int> path) const;
    bool legalRes(int vertex) const;
    void buildRes(int vertex) const;
    bool firstRes(int vertex, int p) const;
    void improve(int vertex) const; //fail = nothing changes

    void richTax() const;
    bool robberAction() const;
    vector<int> getResourceInput(int numResources) const;
    void useDevCard(DevCard dc) const;

    bool legalTrade(int other, int give, int giveAmount, int take, int takeAmount) const;
    void trade(int other, int give, int giveAmount, int take, int takeAmount) const;

    void autoSave();
public:
    Game(int seed);
    ~Game();

    void load(string fileName);
    void setRandomBoard();
    void setBoard(vector<Resource> resources, vector<int> vals);

    void setup();
    void runTurn();

    bool hasWinner();
    void printVictor();
    bool playAgain();

    void save(string fileName);
};

#endif
