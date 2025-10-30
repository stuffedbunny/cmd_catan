#include <iostream>
#include <algorithm>
#include <chrono>

#include "game.h"
#include "rng.h"

int main(int argc, char* argv[]) {
    int prio = 0;
    unsigned int seed = chrono::system_clock::now().time_since_epoch().count();
    string file = "layout.txt";

    // command parsing
    for (int i = 1; i < argc; ++i) {
        string cmd = argv[i];
        if (cmd == "-seed") {
            seed = stoi(argv[i + 1]);
        } else if (cmd == "-load") {
            prio = 3; file = argv[i+1];
        } else if (cmd == "-board") {
            if (prio < 2) {
                prio = 2;
                file = argv[i+1];
            }
        } else if (cmd == "-random-board") {
            if (prio < 1) {
                prio = 1;
            }
        }
    }

    // gameplay loop
    while (true) {
        unique_ptr<Game> g {new Game(seed)};
        
        if (prio == 3) {
            g->load(file);
        } else if (prio == 1) {
            g->setRandomBoard();
        } else {
            ifstream f{file};
            if(f.fail()){
                return 0;
            }
            vector<Resource> resources; vector<int> vals; int temp;
            for (int i = 0; i < 19; i++) {
                f >> temp;
                resources.emplace_back(Resource(temp));
                f >> temp;
                vals.emplace_back(temp);
            }
            g->setBoard(resources, vals);
        }

        if (prio != 3) {
            g->setup();
        }
        prio = 0;

        while (!g->hasWinner()) {
            g->runTurn();
        }
        g->printVictor();
        if (g->playAgain()) {

        } else break;
    }
    return 0;
}
