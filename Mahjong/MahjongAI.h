#pragma once
#include <vector>
#include "Tile.h"

#define ACTION_TSUMO 0
#define ACTION_RIICHI 1
#define ACTION_CHI 2
#define ACTION_PON 3
#define ACTION_KAN 4
#define ACTION_RON 5
#define ACTION_DISCARD 6
#define ACTION_SKIP 7
#define ACTION_DRAW 8

class MahjongAI {
public:
	static int coreAI(vector<Tile> hand, vector<Tile> callSpace, int chi, int pon, int kan);
	static int thinkAI(vector<Tile> hand, vector<Tile> callSpace, int chi, int pon, int kan);
	
	//<summary>패의 개선이 이루어질 수 있는 모든 쯔모 출력</summary>
	static vector<Tile> demandingAI(vector<Tile> hand);

	//<summary>손패중 가장 필요없는 패 순서로 출력</summary>
	static vector<Tile> discardAI(vector<Tile> hand, int chi, int pon, int kan);
};