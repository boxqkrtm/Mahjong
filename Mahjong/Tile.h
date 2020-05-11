#pragma once

#include <string>
using namespace std;

class Tile {
	// tile code 01~09 만수
	// tile code 11~19 통수
	// tile code 21~29 삭수 
	// tile code 31~34 동남서북
	// tile code 35~37 백발중 
private:
	int tileCode;
public:
	Tile(int code);

	//with tile name
	string getTileName();

	//with tile code
	int getTile();

	void setTile(int input);

	bool operator == (Tile t);

	bool operator <(Tile t);
	bool operator >(Tile t);
};