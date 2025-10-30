#ifndef __DEVCARDS__
#define __DEVCARDS__

#include <string>
#include <vector>
#include "globals.h"
#include "board.h"
#include "rng.h"

using namespace std;

class DevCards {
	vector<int> armySizes;
	vector<int> longestRoads;
	vector<vector<DevCard>> pendingDevCards = {{}, {}, {}, {}};
	vector<vector<DevCard>> playerDevCards = {{}, {}, {}, {}};
	vector<DevCard> devCardDeck = {};

	RNG* rng;
	int largestArmyOwner = -1;
	int longestRoadOwner = -1;
public:
	DevCards(RNG* rng);

	int getAwardPoints(int playerNum);
	void playKnight(int playerNum);
	void playRoad(int playerNum, int roadLen);

	DevCard drawDevCard(int playerNum); // takes from deck
	void turnEnd(int playerNum); // pushes all pending dev cards
	vector<DevCard> getDevCards(int playerNum);
	bool useDevCard(int playerNum, DevCard devCard);

	string getStatus();
	string getStatus(int playerNum);
	string getSaveState();
	void loadSaveState(string saveState);
};

#endif
