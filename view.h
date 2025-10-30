#ifndef __VIEW__
#define __VIEW__

#include <string>
#include <iostream>
#include <sstream>
#include "globals.h"
#include "board.h"

class View {
    Board* board;
    ostream& out;

    string getStrVertex(int i) const;
    string getStrEdge(int i) const;
    string getStrTile(int i) const;
    string getVertexHorizontal(int preWhitespace, int tileReps, int *v_count, int *e_count) const;
    string getTileHorizontal(int preWhitespace, int tileReps, int *t_count, int *e_count) const;
    string getAllVertical(int preWhitespace, int tileReps) const;
    
    bool processGenericInput(string msg) const;
    bool isNumInRange(string in, int l, int r) const;
public:
    View(Board* board);
    ~View();

    string toLower(string s) const;
    int getNumRange(string msg, int l, int r) const;

    void printBoard() const;
    int getFirst(int p) const;

    void turnStart(int p, string status) const;
    string getRoll() const;
    int setLoadedDice() const;
    void printLn(string s) const;
    
    void invalidArguments() const;
    void invalidBuild() const;
    void invalidResources() const;
    void invalidInput() const;
    void successBuild() const;

    int getRobberTile(int invalid) const;
    int getVictim(int p, vector<bool> canSteal) const;
    void robberSuccess(int thief, int victim, int resources) const;
    void printRobberLoss(int player, int numLost, vector<int> losses) const;

    void gainHeader(int player) const;
    void gainElem(int resource, int amount) const;
    void noGains() const;

    pair<vector<int>, string> getBuildCmd(int p) const;
    int confirmTrade(int p1, int p2, int r1, int am1, int r2, int am2) const;
    void printHelp() const;

    bool playAgain() const;
};

#endif
