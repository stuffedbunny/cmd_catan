#include "view.h"
#include <limits>
#include <vector>

View::View(Board* board): board{board},out{std::cout} {}

View::~View() { board = nullptr; }

string View::getStrVertex(int i) const {
    return "|" + board->getStrVertex(i) + "|";
}
string View::getStrEdge(int i) const {
    return board->getStrEdge(i);
}
string View::getStrTile(int i) const {
    return "  " + board->getStrTile(i) + "  ";
}

string View::getVertexHorizontal(int preWhitespace, int tileReps, int *v_count, int *e_count) const {
    string out = "";
    for (int i = 0; i < preWhitespace; i++) out += " ";
    out += getStrVertex(*v_count);
    (*v_count)++;

    for (int i = 0; i < tileReps; i++) {
        out += "--" + getStrEdge(*e_count);
        out += "--" + getStrVertex(*v_count);
        (*e_count)++; (*v_count)++;
        out += "--" + getStrEdge(*e_count);
        out += "--" + getStrVertex(*v_count);
        (*e_count)++; (*v_count)++;
    }
    return out;
}
string View::getTileHorizontal(int preWhitespace, int tileReps, int *t_count, int *e_count) const {
    string out = "";
    for (int i = 0; i < preWhitespace; i++) out += " ";
    out += getStrEdge(*e_count);
    (*e_count)++;

    for (int i = 0; i < tileReps; i++) {
        out += getStrTile(*t_count); (*t_count)++;
        out += getStrEdge(*e_count); (*e_count)++;
    }
    return out;
}
string View::getAllVertical(int preWhitespace, int tileReps) const {
    string out = "";
    for (int i = 0; i < preWhitespace + 1; i++) out += " ";
    out += "|";
    for (int i = 0; i < tileReps; i++) out +=  "                   |";
    return out;
}

void View::printBoard() const {
    int preWhitespace[5] = {20, 10, 0, 10, 20};
    int tileReps[5] = {3, 4, 5, 4, 3};
    int v = 0, e = 0, t = 0;

    for (int row = 0; row <= 2; row++) {
        cout << getVertexHorizontal(preWhitespace[row], tileReps[row], &v, &e) << endl;
        cout << getAllVertical(preWhitespace[row], tileReps[row]) << endl;
        cout << getTileHorizontal(preWhitespace[row], tileReps[row], &t, &e) << endl;
        cout << getAllVertical(preWhitespace[row], tileReps[row]) << endl;
    }
    cout << getVertexHorizontal(preWhitespace[2], tileReps[2], &v, &e) << endl;
    for (int row = 3; row < 5; row++) {
        cout << getAllVertical(preWhitespace[row], tileReps[row]) << endl;
        cout << getTileHorizontal(preWhitespace[row], tileReps[row], &t, &e) << endl;
        cout << getAllVertical(preWhitespace[row], tileReps[row]) << endl;
        cout << getVertexHorizontal(preWhitespace[row], tileReps[row], &v, &e) << endl;
    }
}

// returns true if yes, else false
bool View::processGenericInput(string msg) const {
    if (msg == "board") { printBoard(); return true; }
    if (msg == "help") { printHelp(); return true; }
    return false;
}

bool View::isNumInRange(string input, int l, int r) const {
    int response;
    if (istringstream iss{input}; !(iss >> response)) return false;
    if (response < l || response > r) return false;
    return true;
}

string View::toLower(string s) const {
    string output = "";
    for (char c : s) output += tolower(c);
    return output;
}

int View::getNumRange(string msg, int l, int r) const {
    while (true) {
        out << msg;
        string input;
        getline(cin, input);
        if (processGenericInput(input)) continue;
        if (cin.eof()) return -1;
        if (isNumInRange(input, l, r)) return stoi(input);
        invalidInput();
    }
    return -1;
}

int View::getFirst(int p) const {
    return getNumRange("Player " + playerColFull[p] + ", where do you want to build a settlement? ", 0, 53);
}

void View::turnStart(int p, string status) const {
    printBoard();
    out << "Player "<< playerColFull[p] << "'s turn." << endl;
    out << status << "\n> ";
}

string View::getRoll() const {
    string response;
    
    while ((response != "load") && (response != "fair") && (response != "roll")) {
        getline(cin, response);
        if (cin.eof()) return "EOF";

        stringstream ss{response};
        ss >> response;
        response = toLower(response);

        if ((response != "load") && (response != "fair") && (response != "roll")) {
            out << "Invalid command.\n> ";
        }
    }

    if (response != "roll") out << "Dice set to " << response << endl;
    return response;
}

int View::setLoadedDice() const {
    return getNumRange("> ", 2, 12);
}

void View::printLn(string s) const {
    out << s << endl;
}

void View::invalidArguments() const {printLn("Invalid or wrong number of command arguments. ");}
void View::invalidBuild() const {printLn("You cannot build here.");}
void View::invalidResources() const {printLn("You do not have enough resources.");}
void View::invalidInput() const {printLn("Invalid Input.");}
void View::successBuild() const {printLn("Build successful.");}

int View::getRobberTile(int invalid) const {
    int response = getNumRange("Choose where to place the Robber.\n", 0, 18);
    while (response == invalid) {
        response = getNumRange("Choose where to place the Robber.\n", 0, 18);
    }
    return response;
}

int View::getVictim(int p, vector<bool> canSteal) const {
    if (!canSteal[0] && !canSteal[1] && !canSteal[2] && !canSteal[3]) {
        out << "Player " << playerColFull[p] << " has no Players to steal from." << endl;
        return 0; //illegal steal returned
    }
    out << "Player " << playerColFull[p] << " can choose to steal from";
    for (int i = 0; i < static_cast<int>(canSteal.size()); i++) {
        if (canSteal[i]) out << " " << playerColFull[i];
    }
    out << ". Choose a Player to steal from.\n >";
    string input;
    while (true) {
        getline(cin, input);
        if (cin.eof()) return -1;
        input = toLower(input);
        for (int i = 0; i < 4; i++) {
            if (canSteal[i] && (input == validPlayerIn[i][0] || input == validPlayerIn[i][1] || input == to_string(i))) return i;
        }
        out << "Choose a Player to steal from.\n >";
    }
    return 0;
}

void View::robberSuccess(int thief, int victim, int resource) const {
    out << "Player " << playerColFull[thief] << " steals " << resourceNames[resource]
     << " from " << playerColFull[victim] << "." << endl;
}

void View::printRobberLoss(int player, int numLost, vector<int> losses) const {
    out << "Player " << playerColFull[player] << "loses " << numLost << " resources to the Robber. They lose:" << endl;
    for (int i = 0; i < 5; i++) {
        if (losses[i] == 0) continue;
        out << losses[i] << " " << resourceNames[i] << endl;
    }
}

void View::gainHeader(int player) const {
    out << "Player " << playerColFull[player] << " gained:" << endl;
}
void View::gainElem(int resource, int amount) const {
    out << amount << " " << resourceNames[resource] << endl;
}
void View::noGains() const {
    printLn("No Players gained resources.");
}

pair<vector<int>, string> View::getBuildCmd(int p) const {
    out << "> ";
    string input; string s; vector<string> inputs;
    getline(cin, input);
    if (cin.eof()) return make_pair(vector<int>{-1}, "");
    stringstream ss(input);
    while (getline(ss, s, ' ')) {
        inputs.emplace_back(toLower(s));
    }

    if (inputs.size() == 0) return getBuildCmd(p);
    if (inputs[0] == "board") {
        if (inputs.size() != 1) { invalidArguments(); return getBuildCmd(p); }
        return {vector<int>{0}, ""};
    }
    if (inputs[0] == "status") {
        if (inputs.size() != 1) { invalidArguments(); return getBuildCmd(p); }
        return {vector<int>{1}, ""};
    }
    if (inputs[0] == "settlements") {
        if (inputs.size() != 1) { invalidArguments(); return getBuildCmd(p); }
        return {vector<int>{2}, ""};
    }
    if (inputs[0] == "build-road") {
        if (inputs.size() != 2 || !isNumInRange(inputs[1], 0, 71)) { invalidArguments(); return getBuildCmd(p); }
        return {vector<int>{3, stoi(inputs[1])}, ""};
    }
    if (inputs[0] == "build-res") {
        if (inputs.size() != 2 || !isNumInRange(inputs[1], 0, 53)) { invalidArguments(); return getBuildCmd(p); }
        return {vector<int>{4, stoi(inputs[1])}, ""};
    }
    if (inputs[0] == "improve") {
        if (inputs.size() != 2 || !isNumInRange(inputs[1], 0, 53)) { invalidArguments(); return getBuildCmd(p); }
        return {vector<int>{5, stoi(inputs[1])}, ""};
    }
    if (inputs[0] == "trade") {
        if (inputs.size() != 6) { invalidArguments(); return getBuildCmd(p); }
        int a = -1, b = -1, c = -1;
        for (int i = 0; i < 4; i++) {
            if (inputs[1] == validPlayerIn[i][0] || inputs[1] == validPlayerIn[i][1]) {
                a = i;
            }
        }
        for (int i = 0; i < 5; i++) {
            if (inputs[2] == resourceLower[i]) b = i;
            if (inputs[4] == resourceLower[i]) c = i;
        }
        int b_count = stoi(inputs[3]);
        int c_count = stoi(inputs[5]);
        if (a != -1 && b != -1 && c != -1) return {vector<int>{6, a, b, b_count, c, c_count}, ""};
        invalidArguments(); return getBuildCmd(p);
    }
    if (inputs[0] == "buydev") {
        return {vector<int>{7}, ""};
    }
    if (inputs[0] == "usedev") {
        if (inputs.size() != 2) { invalidArguments(); return getBuildCmd(p); }
        int dc = -1;
        for (int i = 0; i < 4; i++) {
            if (inputs[1] == validDevCardIn[i]) {
                dc = i;
            }
        }
        return {vector<int>{8, dc}, ""};
    }
    if (inputs[0] == "next" && inputs.size() == 1) return {vector<int>{9}, ""};
    if (inputs[0] == "save" && inputs.size() == 2) return {vector<int>{10}, inputs[1]};
    if (inputs[0] == "help" && inputs.size() == 1) return {vector<int>{11}, ""};
    
    invalidInput();
    return getBuildCmd(p);
}

void View::printHelp() const {
    out << "Vertex #: 0-53, Edge #: 0-71, Players: blue/red/orange/yellow or b/r/o/y\n";
    out << "Valid commands (after initial):\nboard\nstatus\nsettlements\nbuild-road <edge#>\nbuild-res <housing#>\nimprove <housing#>\ntrade <colour> <give> <give amount> <take> <take amount>\nbuydev\nusedev <card>\nnext\nsave <file>\nhelp" << endl;
}
bool View::playAgain() const {
    out << "Would you like to play again?" << endl;
    string input = ""; cin >> input; 
    if (cin.eof()) return false;
    input = toLower(input);
    while (input != "yes" && input != "no") {
        out << "Would you like to play again?" << endl;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (input == "yes") return true;
    return false;
}

int View::confirmTrade(int p1, int p2, int r1, int am1, int r2, int am2) const {
    out << playerColFull[p1] << " offers " << playerColFull[p2] << " " << am1 << " " << resourceNames[r1]
    << " for " << am2 << " " << resourceNames[r2] << ".\nDoes " << playerColFull[p2] << " accept this offer?\n> ";

    string input;
    while (true) {
        cin >> input;
        if (cin.eof()) return -1;
        input = toLower(input);
        if (input != "yes" && input != "no") {
            out << "Does " << playerColFull[p2] << " accept this offer?\n> ";
        } else break;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (input == "yes") return 1;
    return 0;
}
