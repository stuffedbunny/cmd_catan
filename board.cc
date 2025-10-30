#include "board.h"
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

Board::Board() {
    //creating vertices, edges, and tiles
    for (int i = 0; i < 54; i++) { vertices.emplace_back(unique_ptr<Vertex>(new Vertex(i))); }
    for (int i = 0; i < 72; i++) { edges.emplace_back(unique_ptr<Edge>(new Edge(i))); }
    int val[19] = {3, 10, 5, 4, 7, 10, 11, 3, 8, 2, 6, 8, 12, 5, 11, 4, 6, 9, 9};
    int res[19] = {0, 1, 3, 1, 5, 3, 2, 0 ,3, 0, 0, 1, 4, 1, 4, 2, 4, 2, 2};
    int tileAdj[19][6] = {
                        {0, 1, 2, 8, 9, 10},
                        {2, 3, 4, 10, 11, 12},
                        {4, 5, 6, 12, 13, 14},
                        {7, 8, 9, 17, 18, 19},
                        {9, 10, 11, 19, 20, 21},
                        {11, 12, 13, 21, 22, 23},
                        {13, 14, 15, 23, 24, 25},
                        {16, 17, 18, 27, 28, 29},
                        {18, 19, 20, 29, 30, 31},
                        {20, 21, 22, 31, 32, 33},
                        {22, 23, 24, 33, 34, 35},
                        {24, 25, 26, 35, 36, 37},
                        {28, 29, 30, 38, 39, 40},
                        {30, 31, 32, 40, 41, 42},
                        {32, 33, 34, 42, 43, 44},
                        {34, 35, 36, 44, 45, 46},
                        {39, 40, 41, 47, 48, 49},
                        {41, 42, 43, 49, 50, 51},
                        {43, 44, 45, 51, 52, 53}
                    };
    // int tileAdj[19][6] = {{0, 1, 3, 4, 8, 9},{2, 3, 7, 8, 13, 14},{4, 5, 9, 10, 15, 16},{6, 7, 12, 13, 18, 19},{8, 9, 14, 15, 20, 21},
    //                     {10, 11, 16, 17, 22, 23},{13, 14, 19, 20, 25, 26},{15, 16, 21, 22, 27, 28},{18, 19, 24, 25, 30, 31},{20, 21, 26, 27, 32, 33},
    //                     {12, 23, 28, 29, 34, 35},{25, 26, 31, 32, 37, 38},{27, 28, 33, 34, 39, 40},{30, 31, 36, 37, 42, 43},{32, 33, 38, 39, 44, 45},
    //                     {34, 35, 40, 41, 46, 47},{37, 38, 43, 44, 48, 49},{39, 40, 45, 46, 50, 51},{44, 45, 49, 50, 52, 53}};
    for (int i = 0; i < 19; i++) {
        tiles.emplace_back(unique_ptr<Tile>(
            new Tile(i, val[i], res[i], vertices[tileAdj[i][0]].get(), vertices[tileAdj[i][1]].get(), 
            vertices[tileAdj[i][2]].get(), vertices[tileAdj[i][3]].get(), vertices[tileAdj[i][4]].get(), 
            vertices[tileAdj[i][5]].get())));
    }
    
    //setting up connectivity
    ifstream file("vertexEdgeAdj.txt");
    string lineEdges; int tempEdge;
    for (int vertex = 0; vertex < 54; vertex++) {
        getline(file, lineEdges);
        stringstream s(lineEdges);
        while (s >> tempEdge) {
            vertices[vertex]->addAdj(edges[tempEdge].get());
            edges[tempEdge]->addAdj(vertices[vertex].get());
        }
    }

    moveRobber(4);
}

Board::~Board() { }

string Board::getStrVertex(int i) const {
    return vertices[i]->toStrForm();
}
string Board::getStrEdge(int i) const {
    return edges[i]->toStrForm();
}
string Board::getStrTile(int i) const {
    return tiles[i]->toStrForm();
}

Tile* Board::getTile(int i) const {
    return tiles[i].get();
}

Vertex* Board::getVertex(int i) const {
    return vertices[i].get();
}

Edge* Board::getEdge(int i) const {
    return edges[i].get();
}

vector<vector<int>> Board::diceRolled(int num) const { // [player][resource] = gain
    vector<vector<int>> out;
    vector<int> v = {0, 0, 0, 0 ,0};
    for (int i = 0; i < 4; i++) { out.emplace_back(v); }
    for (int i = 0; i < 19; i++) {
        vector<vector<int>> temp = tiles[i]->diceRolled(num);
        for (auto j : temp) {
            if (j[0] == -1) continue;
            out[j[0]][j[1]] += j[2];
        }
    }
    return out;
}

int Board::getRobberPos() const {
    return robberPos;
}

vector<bool> Board::moveRobber(int dest) { //Moves robber, returns list of potential victims
    tiles[robberPos]->noRobber();
    robberPos = dest;
    tiles[dest]->yesRobber();
    return tiles[dest]->getResidents();
}
