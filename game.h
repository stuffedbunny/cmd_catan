#ifndef __GAME__
#define __GAME__

#include "globals.h"
#include "board.h"
#include "view.h"
#include "player.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Game {
    vector<unique_ptr<Player>> players;

    unique_ptr<Board> board;
    unique_ptr<View> view;

    int turn = 0;
    int winner = 0;
    bool stopProgram = false;

    bool legalRoad(int edge) const;
    void buildRoad(int edge) const;
    bool legalRes(int vertex) const;
    void buildRes(int vertex) const;
    bool firstRes(int vertex, int p) const;
    void improve(int vertex) const; //fail = nothing changes

    bool legalTrade(int other, int give, int take) const;
    void trade(int other, int give, int take) const;

    void autoSave();
public:
    Game();
    ~Game();

    void load(string fileName);
    void setBoard(vector<int> resources, vector<int> vals);

    void setup();
    void runTurn();

    bool hasWinner();
    void printVictor();
    bool playAgain();

    void save(string fileName);
};

#endif
