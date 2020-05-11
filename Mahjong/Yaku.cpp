#include <iostream>
#include <vector>
#include "Tile.h"
#include "Yaku.h"

bool Yaku::thirteenOrphans(vector<Tile> hand, vector<Tile> possibleHead) {
	int i, j;
	int honorOne = 0;
	int honorTwo = 0;
	if (possibleHead.size() == 1) {
		if (debug == true) cout << "debug 특수역: 국사무쌍 체크" << endl;
		int honorList[13] = { 1,9,11,19,21,29,31,32,33,34,35,36,37 };
		int honorCount[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 };
		//국사 체크 191919동남서북백발중

		//요구패 개수 저장
		for (i = 0; i < 14; i++) {
			for (j = 0; j < 13; j++) {
				if (hand[i].getTile() == honorList[j]) {
					honorCount[j]++;
				}
			}
		}
		for (i = 0; i < 13; i++) {
			switch (honorCount[i]) {
			case 1:
				honorOne += 1;
				break;
			case 2:
				honorTwo += 1;
				break;
			default:
				return false;
				//국사아님
			}
		}
		if (honorOne == 12 && honorTwo == 1) {
			//요구패 12종 1개에 1종 2개면 국사무쌍 ex) 191919동남서북백발중중
			return true;
		}
		else {
			return false;
			//국사 아님
		}
	}
	return false;
}

bool Yaku::sevenPairs(vector<Tile> hand, vector<Tile> possibleHead) {
	int i;
	if (possibleHead.size() == 7) {
		if (debug == true) cout << "debug 특수역: 치또이 체크" << endl;
		//같은 패 4개는 또이츠 2개로 계산할 수 없다
		for (i = 0; i < (14 - 2); i++) {
			if (hand[i] == hand[i + 1] && hand[i + 1] == hand[i + 2]) {
				//3개가 연달아 붙은경우 치또이는 성립실패
				return false;
			}
		}
		//치또이 성립
		return true;
	}
	//머리 개수 부족
	return false;
}