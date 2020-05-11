#include <string>
#include "Tile.h"

using namespace std;

Tile::Tile(int code) {
	tileCode = code;
}

//with tile name
string Tile::getTileName() {
	string result;
	switch (tileCode / 10) {
	case 0:
		result = to_string(tileCode % 10) + "��";
		break;
	case 1:
		result = to_string(tileCode % 10) + "��";
		break;
	case 2:
		result = to_string(tileCode % 10) + "��";
		break;
	case 3:
		switch (tileCode % 10) {
		case 1:
			result = "��";
			break;
		case 2:
			result = "��";
			break;
		case 3:
			result = "��";
			break;
		case 4:
			result = "��";
			break;
		case 5:
			result = "��";
			break;
		case 6:
			result = "��";
			break;
		case 7:
			result = "��";
			break;
		}
	}
	return result;
}

//with tile code
int Tile::getTile() {
	return tileCode;
}

void Tile::setTile(int input) {
	tileCode = input;
}

bool Tile::operator == (Tile t) {
	return getTile() == t.getTile();
}

bool Tile::operator <(Tile t) {
	return getTile() < t.getTile();
}

bool Tile::operator >(Tile t) {
	return getTile() > t.getTile();
}