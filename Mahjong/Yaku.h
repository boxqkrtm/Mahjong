#pragma once
#include <vector>

class Yaku {
	private:
		static const bool debug = false;

	public:
		static bool thirteenOrphans(vector<Tile> hand, vector<Tile> possibleHead);
		static bool sevenPairs(vector<Tile> hand, vector<Tile> possibleHead);
};

