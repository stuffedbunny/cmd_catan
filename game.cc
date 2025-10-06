#include "game.h"

Game::Game(): board{new Board()}, view{new View(this->board.get())} {
    for (int i = 0; i < 4; ++i) {
        players.emplace_back(new Player(i));
    }
}

Game::~Game() {}

void Game::load(string fileName) {
    ifstream f{fileName};
    f >> turn;
    int num; string s; string line;
    getline(f, line);
    for (int i = 0; i < numPlayers; i++) {
        getline(f, line);
        stringstream ss{line};
        for (int j = 0; j < 5; j++) {
            ss >> num;
            players[i]->resourceGain(j, num);
        }
        ss >> s >> s; // r
        while (s != "h") {
            board->getEdge(stoi(s))->build(i);
            players[i]->addEdge(board->getEdge(stoi(s)));
            ss >> s;
        }
        while (ss >> num >> s) {
            board->getVertex(num)->build(i);
            players[i]->addVertex(board->getVertex(num));
            if (s == "B") continue;
            board->getVertex(num)->improve();
            players[i]->addPoint();
            if (s == "H") continue;
            board->getVertex(num)->improve();
            players[i]->addPoint();
        }
    }
    getline(f, line);
    stringstream ss{line};
    vector<int> res; vector<int> val;
    for (int i = 0; i < 19; i++) {
        ss >> num; res.emplace_back(num);
        ss >> num; val.emplace_back(num);
    }
    setBoard(res, val);
    f >> num;
    board->moveRobber(num);
}

void Game::setBoard(vector<int> resources, vector<int> vals) {
    for (int i = 0; i < 19; i++) {
        Tile* t = board->getTile(i);
        t->setResource(resources[i]);
        t->setVal(vals[i]);
        if (vals[i] == 7) {
            board->moveRobber(i);
        }
    }
}

// --- board/player interactions ---
bool Game::legalRoad(int edge) const {
    Edge* e = board->getEdge(edge);
    if (e->getOwner() != -1 || !e->adjValid(turn)) { view->invalidBuild(); return false; }

    if (!players[turn]->hasResources(roadMats)) {
        view->invalidResources(); return false;
    }
    return true;
}

void Game::buildRoad(int edge) const {
    board->getEdge(edge)->build(turn);
    players[turn]->resourceLoss(roadMats);
    players[turn]->addEdge(board->getEdge(edge));
    view->successBuild();
}

bool Game::legalRes(int vertex) const {
    Vertex* v = board->getVertex(vertex);
    if (v->getOwner() != -1 || !v->adjValid(turn)) { view->invalidBuild(); return false; }

    if (!players[turn]->hasResources(settlementMats)) {
        view->invalidResources(); return false;
    }
    return true;
}

void Game::buildRes(int vertex) const {
    board->getVertex(vertex)->build(turn);
    players[turn]->resourceLoss(settlementMats);
    players[turn]->addVertex(board->getVertex(vertex)); //also adds point
    view->successBuild();
}

bool Game::firstRes(int vertex, int p) const {
    Vertex* v = board->getVertex(vertex);
    if (v->getOwner() != -1) return false;
    if (!v->adjValid(p, true)) return false;
    v->build(p);
    return true;
}

void Game::improve(int vertex) const {
    Vertex* v = board->getVertex(vertex);
    if (v->getOwner() != turn) { view->invalidBuild(); return; } //illegal owner
    switch (v->getStage()) {
        case 1:
            if (players[turn]->hasResources(cityMats)) {
                players[turn]->resourceLoss(cityMats);
                break;
            }
            view->invalidResources(); return;
        // case 2:
        //     if (players[turn]->hasResources(towerMats)) {
        //         players[turn]->resourceLoss(towerMats);
        //         break;
        //     }
        //     view->invalidResources(); return;
        default: //illegal stage
            view->invalidBuild(); return;
    }
    v->improve();
    players[turn]->addPoint();
    view->successBuild();
}

bool Game::legalTrade(int other, int give, int take) const {
    if (other == turn) { view->print("You can't trade with yourself"); return false; }
    if (!players[turn]->hasResources(vector<pair<int, int>>{{give, 1}})
        || !players[other]->hasResources(vector<pair<int, int>>{{take, 1}})) {
        view->invalidResources();
        return false;
    }
    return true;
}

void Game::trade(int other, int give, int take) const {
    players[turn]->resourceLoss(give, 1);
    players[other]->resourceGain(give, 1);
    players[other]->resourceLoss(take, 1);
    players[turn]->resourceGain(take, 1);
}

// --- misc game elements ---
bool Game::hasWinner() {
    for (int i = 0; i < numPlayers; i++) {
        if (players[i]->getPts() >= 10) {
            winner = i; return true;
        }
    }
    return stopProgram;
}

void Game::printVictor() {
    if (!stopProgram) view->print("Player " + playerColFull[winner] + " has won!");
}

bool Game::playAgain() {
    if (!stopProgram) return view->playAgain();
    return false;
}

void Game::autoSave() {
    save("backup.sv");
}

void Game::save(string fileName) {
    ofstream file(fileName, ofstream::trunc);
    file << turn << "\n";
    for (int i = 0; i < numPlayers; i++) {
        file << players[i]->getState() << "\n";
    }
    for (int i = 0; i < 19; ++i) {
        Tile* cur = board->getTile(i);
        file << cur->toStateForm() << " ";
    }
    file << "\n";
    file << board->getRobberPos();
}

// --- game stages ---
void Game::setup() {
    view->printBoard();
    while (turn < numPlayers) {
        int startSqr = view->getFirst(turn);
        if (startSqr == -1) { stopProgram = true; return; }
        if (firstRes(startSqr, turn)) {
            players[turn]->addVertex(board->getVertex(startSqr));
            ++turn;
        } else {
            view->invalidBuild();
        }
    }
    turn = 3;
    while (turn > -1) {
        int startSqr = view->getFirst(turn);
        if (startSqr < 0) { stopProgram = true; return; }
        if (firstRes(startSqr, turn)) {
            players[turn]->addVertex(board->getVertex(startSqr));
            --turn;
        } else {
            view->invalidBuild();
        }
    }
    turn = 0;
}

void Game::runTurn() {
    string cmd;
    view->turnStart(turn, players[turn]->getStatus());

    // --- Roll Phase ---
    while (cmd != "roll") {
        cmd = view->getRoll();
        if (cmd == "load") { players[turn]->setLoaded(); }
        else if (cmd == "fair")  { players[turn]->setFair(); }
        else if (cmd == "EOF") { stopProgram = true; return; }
    }

    int roll;
    if (players[turn]->getFairness()) {
        roll = 1 + (rand() % 6) + 1 + (rand() % 6); // rolls 2 dice
    } else {
        roll = view->setLoadedDice();
        if (roll == -1) { stopProgram = true; return; }
    }
    view->print("Rolled a " + to_string(roll));

    if (roll == 7) {
        //Tax the rich
        for (int i = 0; i < numPlayers; ++i) {
            if (players[i]->resourceSum() > 9) {
                int numLost = players[i]->resourceSum()/2;
                vector<int> losses = players[i]->richTax(numLost);
                view->printRobberLoss(i, numLost, losses);
            }
        }

        //Player gets to steal
        int newTile = view->getRobberTile(board->getRobberPos());
        vector<bool> canSteal = board->moveRobber(newTile);
        canSteal[turn] = false;
        for (int i = 0; i < numPlayers; i++) {
            if (players[turn]->resourceSum() == 0) canSteal[i] = false;
        }
        int selection = view->getVictim(turn, canSteal);
        if (selection == -1) { stopProgram = true; return; }
        if (canSteal[selection]) {
            int resource =  players[selection]->resourceLossRandom();
            players[turn]->resourceGain(resource, 1);
            view->robberSuccess(turn, selection, resource);
        }
    } else { // Non-robber roll
        vector<vector<int>> gains = board->diceRolled(roll);
        vector<bool> hasGot = {false, false, false, false};
        for (int i = 0; i < numPlayers; ++i) {
            for (int j = 0; j < 5; ++j) {
                if (gains[i][j] != 0) hasGot[i] = true;
            }
            if (hasGot[i]) view->gainHeader(i);
            for (int j = 0; j < 5; ++j) {
                if (gains[i][j] == 0) continue;
                players[i]->resourceGain(j, gains[i][j]);
                view->gainElem(j, gains[i][j]);
            }
        }
        if (!hasGot[0] && !hasGot[1] && !hasGot[2] && !hasGot[3]) {
            view->noGains();
        }
    }

    // --- Build phase ---
    while (cmd != "next") {
        // vector is cmd + inputs, string is only used for fileName
        pair<vector<int>, string> cmds = view->getBuildCmd(turn);
        if (cmds.first[0] == -1) {
            stopProgram = true; return;
        }

        switch (cmds.first[0]) {
            case 0: //"board":
                view->printBoard(); break;
            case 1: //"status":
                for (int i = 0; i < numPlayers; i++) { view->print(players[i]->getStatus()); }
                break;
            case 2: //"residences":
                view->print(players[turn]->getResidences());
                break;
            case 3: //"build-road #"
                if (legalRoad(cmds.first[1])) buildRoad(cmds.first[1]);
                break;
            case 4: //"build-res #"
                if (legalRes(cmds.first[1])) buildRes(cmds.first[1]);
                break;
            case 5: //"improve #"
                improve(cmds.first[1]); break;
            case 6: //"trade"
                if (legalTrade(cmds.first[1], cmds.first[2], cmds.first[3])) {
                    int response = view->confirmTrade(turn, cmds.first[1], cmds.first[2], cmds.first[3]);
                    if (response == -1) { stopProgram = true; return; }
                    if (response == 1) trade(cmds.first[1], cmds.first[2], cmds.first[3]);
                }
                break;
            case 7: //"next":
                cmd = "next"; break;
            case 8: //"save":
                save(cmds.second); break;
            case 9: //"help":
                view->printHelp(); break;
        }
        if (hasWinner()) return;
        autoSave();
    }

    ++turn;
    if (turn == 4) turn = 0;
}
