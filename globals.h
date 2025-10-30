#ifndef __GLOBALS__
#define __GLOBALS__

#include <cstdlib>
#include <string>
#include <vector>
#include <memory>

using namespace std;

enum Resource{Brick, Grain, Lumber, Ore, Wool, Desert};

const int numPlayers = 4;

const vector<pair<int, int>> roadMats = {{Brick, 1}, {Lumber, 1}};
const vector<pair<int, int>> settlementMats = {{Brick, 1}, {Lumber, 1}, {Wool, 1}, {Grain, 1}};
const vector<pair<int, int>> cityMats = {{Ore, 3}, {Grain, 2}};
const vector<pair<int, int>> devMats = {{Ore, 1}, {Wool, 1}, {Grain, 1}};

const string playerCol[4] = {"B","R","O","Y"};
const string playerColFull[4] = {"Blue","Red","Orange","Yellow"};
const string validPlayerIn[4][2] = {{"b","blue"},{"r","red"},{"o","orange"},{"y","yellow"}};
const string resourceNames[6] = {"BRICK","GRAIN","LUMBER","ORE","WOOL","DESERT"};
const string resourceLower[5] = {"brick","grain","lumber","ore","wool"};
const string resourcePrintFmt[6] = {" BRICK"," GRAIN","LUMBER","  ORE "," WOOL ","DESERT"};

enum DevCard{KNIGHT, VP, RB, YOP, MONOPOLY, DEV_EMPTY = -1};
const string devCardNames[5] = {"Knight (k)", "Victory Point (vp)", "Road Building (rb)", "Year of Plenty (yp)", "Monopoly (m)"};
const string validDevCardIn[5] = {"k", "vp", "rb", "yp", "m"};


#endif
