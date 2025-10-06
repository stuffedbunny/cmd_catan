#include <iostream>
#include <algorithm>
#include <random>

#include "game.h"

int main(int argc, char* argv[]) {
    int prio = 0;
    unsigned int seed = 0;
    string file = "layout.txt";

    // command parsing
    for (int i = 1; i < argc; ++i) {
        string cmd = argv[i];
        if (cmd == "-seed") {
            seed = stoi(argv[i + 1]);
            srand(seed);
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
        unique_ptr<Game> g {new Game()};
        
        if (prio == 3) {
            g->load(file);
        } else if (prio == 1) {
            vector<int> resources = {4, 4, 4, 3, 3, 3, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2};
            vector<int> vals = {2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12, 7};
            auto rng = std::default_random_engine { seed };
            shuffle(begin(resources), end(resources), rng);
            shuffle(begin(vals), end(vals), rng);
            for (int i = 0; i < 19; i++) {
                if (vals[i] == 7) { resources.insert(resources.begin()+i, 5); break; }
            }
            g->setBoard(resources, vals);
        } else {
            ifstream f{file};
            if(f.fail()){
                return 0;
            }
            vector<int> resources; vector<int> vals; int temp;
            for (int i = 0; i < 19; i++) {
                f >> temp;
                resources.emplace_back(temp);
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
