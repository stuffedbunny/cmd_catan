#include "devcards.h"
#include "rng.h"
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <sstream>

using namespace std;

DevCards::DevCards(RNG* rng): rng{rng} {
	armySizes = vector<int>(numPlayers, 0);
	longestRoads = vector<int>(numPlayers, 0);

	// 14 knights, 5 vps, 2 rb, 2 year of plenty, 2 monopoly
	devCardDeck = {
		MONOPOLY, MONOPOLY,
		YOP, YOP,
		RB, RB,
		VP, VP, VP, VP, VP,
		KNIGHT, KNIGHT, KNIGHT, KNIGHT, KNIGHT, KNIGHT, KNIGHT, KNIGHT, KNIGHT, KNIGHT, KNIGHT, KNIGHT, KNIGHT, KNIGHT
	};
	rng->shuffle(devCardDeck.begin(), devCardDeck.end());
}

int DevCards::getAwardPoints(int playerNum) {
	int base = 0;
	if (largestArmyOwner == playerNum) base += 2;
	if (longestRoadOwner == playerNum) base += 2;
	for (DevCard c : playerDevCards[playerNum]) {
		if (c == VP) base += 1;
	}
	for (DevCard c : pendingDevCards[playerNum]) {
		if (c == VP) base += 1;
	}
	return base;
}

void DevCards::playKnight(int playerNum) {
	armySizes[playerNum]++;
	if (armySizes[playerNum] >= 3) {
		if (largestArmyOwner == -1) largestArmyOwner = playerNum;
		else if (armySizes[playerNum] > armySizes[largestArmyOwner]) largestArmyOwner = playerNum;
	}
}
void DevCards::playRoad(int playerNum, int roadLength) {
	if (roadLength <= longestRoads[playerNum]) return;
	longestRoads[playerNum] = roadLength;

	if (longestRoads[playerNum] >= 3) {
		if (longestRoadOwner == -1) longestRoadOwner = playerNum;
		else if (longestRoads[playerNum] > longestRoads[longestRoadOwner]) longestRoadOwner = playerNum;
	}
}

DevCard DevCards::drawDevCard(int playerNum) {
	if (devCardDeck.size() == 0) return DEV_EMPTY;
	DevCard out = devCardDeck.back();
	devCardDeck.pop_back();
	pendingDevCards[playerNum].push_back(out);
	return out;
}
void DevCards::turnEnd(int playerNum) {
	while (!pendingDevCards[playerNum].empty()) {
		playerDevCards[playerNum].push_back(pendingDevCards[playerNum].back());
		pendingDevCards[playerNum].pop_back();
	}
	sort(playerDevCards[playerNum].begin(), playerDevCards[playerNum].end());
}
vector<DevCard> DevCards::getDevCards(int playerNum) {
	return playerDevCards[playerNum];
}
bool DevCards::useDevCard(int playerNum, DevCard devCard) {
	for (int i = 0; i < int(playerDevCards[playerNum].size()); i++) {
		if (playerDevCards[playerNum][i] == devCard) {
			playerDevCards[playerNum].erase(playerDevCards[playerNum].begin() + i);
			return true;
		}
	}
	return false; // not found
}

string DevCards::getStatus() {
	string la = ""; string lr = "";
	if (largestArmyOwner == -1) la = "Nobody has the largest army.";
	else la = "Player " + playerColFull[largestArmyOwner] + " has the largest army at " + to_string(armySizes[largestArmyOwner]);
	if (longestRoadOwner == -1) lr = "Nobody has the longest road.";
	else lr = "Player " + playerColFull[longestRoadOwner] + " has the largest army at " + to_string(longestRoads[longestRoadOwner]);
	return la + " " + lr;
}
string DevCards::getStatus(int playerNum) {
	string out1 = "Player " + playerColFull[playerNum] + " has dev cards: ";
	string none1 = "none.";
	for (int i = 0; i < playerDevCards[playerNum].size(); i++) {
		none1 = "";
		if (i > 1) out1 += ", ";
		out1 += to_string(i) + "=" + devCardNames[playerDevCards[playerNum][i]];
	}

	string out2 = " and also bought this turn: ";
	string none2 = "none.";
	for (int i = 0; i < pendingDevCards[playerNum].size(); i++) {
		none2 = "";
		if (i > 1) out2 += ", ";
		out2 += devCardNames[pendingDevCards[playerNum][i]];
	}
	return out1 + none1 + out2 + none2;
}

const string delim = " ";

string DevCards::getSaveState() {
	stringstream ss;
	ss << largestArmyOwner << delim << longestRoadOwner;
	for (int armySize : armySizes) ss << delim << armySize;
	for (int roadLen : longestRoads) ss << delim << roadLen;
	for (DevCard dc : devCardDeck) ss << delim << dc;
	ss << delim << DEV_EMPTY;
	for (auto dcs : playerDevCards) {
		for (DevCard dc : dcs) ss << delim << dc;
		ss << delim << DEV_EMPTY;
	}
	return ss.str();
}
void DevCards::loadSaveState(string saveState) {
	stringstream ss(saveState);

	ss >> largestArmyOwner >> longestRoadOwner;
	for (int armySize : armySizes) ss >> armySize;
	for (int roadLen : longestRoads) ss >> roadLen;

	int item;
	devCardDeck = {};
	ss >> item;
	while (item != DEV_EMPTY) { devCardDeck.emplace_back(DevCard(item)); ss >> item; }

	for (auto playerDeck : playerDevCards) {
		playerDeck = {};
		while (item != DEV_EMPTY) { playerDeck.emplace_back(DevCard(item)); ss >> item; }
	}
}