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
		result = to_string(tileCode % 10) + "만";
		break;
	case 1:
		result = to_string(tileCode % 10) + "통";
		break;
	case 2:
		result = to_string(tileCode % 10) + "삭";
		break;
	case 3:
		switch (tileCode % 10) {
		case 1:
			result = "동";
			break;
		case 2:
			result = "남";
			break;
		case 3:
			result = "서";
			break;
		case 4:
			result = "북";
			break;
		case 5:
			result = "백";
			break;
		case 6:
			result = "발";
			break;
		case 7:
			result = "중";
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