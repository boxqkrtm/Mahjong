#include <vector>
#include <algorithm>
#include "Tile.h"
#include "MahjongHandCalculator.h"
#include "MahjongAI.h"
#include <iostream>

using namespace std;

class ScoreWithTile {
public:
	Tile *tile;
	int score;

	ScoreWithTile(Tile t, int s) {
		tile = new Tile(t.getTile());
		score = s;
	}

	bool operator >(ScoreWithTile s) {
		return score > s.score;
	}

	bool operator <(ScoreWithTile s) {
		return score < s.score;
	}

	bool operator ==(ScoreWithTile s) {
		return score == s.score;
	}
};

int MahjongAI::coreAI(vector<Tile> hand, vector<Tile> callSpace, int chi, int pon, int kan) {
	return 0;
}

int MahjongAI::thinkAI(vector<Tile> hand, vector<Tile> callSpace, int chi, int pon, int kan)
{
	MahjongHandCalculator::sortHand(hand);
	if ((int)hand.size() + ((chi + pon + kan) * 3) == 14) {
		if (MahjongHandCalculator::checkTsumo(hand, false, chi, pon, kan) == true) {
			return ACTION_TSUMO;
		}
	}
}

vector<Tile> MahjongAI::demandingAI(vector<Tile> hand)
{
	vector<Tile> result;
	vector<Tile> possibleHead;
	int i;
	MahjongHandCalculator::sortHand(hand);
	MahjongHandCalculator::findHead(hand, possibleHead);
	result.assign(possibleHead.begin(), possibleHead.end());
	for (i = 0; i < (int)hand.size(); i++) {
		if (hand[i].getTile() > 30) {
			//자패
			result.push_back(hand[i]);
		}
		if(hand[i].getTile() > 0 && hand[i].getTile() < 30){
			//수패
			if ((hand[i].getTile() - 1) % 10 != 0) {
				result.push_back(Tile(hand[i].getTile()+1));
			}
			if ((hand[i].getTile() - 1) % 10 != 0) {
				result.push_back(Tile(hand[i].getTile()-1));
			}
		}
	}

	//중복제거 불가능제거
	MahjongHandCalculator::sortHand(result);
	for (i = 0; i < (int)result.size()-1; i++) {
		if (i == i - 1);
		if (i == i + 1);
		if (result[i] == result[(long(i - (int)1)]) {
			result.erase(result.begin() + i+1);
			i--;
			continue;
		}
		else {
			if ( MahjongHandCalculator::checkRemain(hand, result[i]) < 0 ) {
				result.erase(result.begin() + i);
				i--;
				continue;
			}
		}
	}
	return result;
}

vector<Tile> MahjongAI::discardAI(vector<Tile> hand, int chi, int pon, int kan)
{

	/*
		-Todo-
		targetAI를 만들어 주변에 역이 있다면 그역으로 버리는 AI추가
	*/
	int i;
	vector<Tile> result;
	vector<Tile> possibleHead;
	vector<Tile> tmp;
	vector<int> scoreL;
	vector<Tile> noDhand;
	vector<ScoreWithTile> scoreTile;
	int score;
	MahjongHandCalculator::sortHand(hand);

	//한번 버리고 텐파이가능시 버림패 추천
	if ((int)hand.size() + (chi + pon + kan) * 3 == 14) {
		for (i = 0; i < (int)hand.size(); i++) {
			tmp.assign(hand.begin(), hand.end());
			tmp.erase(tmp.begin() + i);
			if (MahjongHandCalculator::checkTenpai(tmp, false, 0, chi, pon, kan) == true) {
				result.push_back(hand[i]);
			}
		}
		if (result.size() > 0) {
			return result;
		}
	}

	//연산용 중복제거
	noDhand.assign(hand.begin(), hand.end());
	MahjongHandCalculator::sortHand(noDhand);
	for (i = 0; i < (int)noDhand.size()-1; i++) {
		if (noDhand[i] == noDhand[i + 1]) {
			noDhand.erase(noDhand.begin() + i);
			i = i-1;
		}
	}

	//그 이외에는 떨어진 패 버림
	for (i = 0; i < (int)noDhand.size(); i++) {
		score = 0;

		//커쯔가능성 손에 여러개 들고있으면 올라감
		score += 3-MahjongHandCalculator::checkRemain(hand, noDhand[i]);

		//슌쯔가능성 일단 자패컷
		if (noDhand[i].getTile() / 10 == 3) {
			scoreL.push_back(score);
			continue;
		}

		//Todo 이밑 checkRemain은 패산 기준으로 변화해야됨

		//다음과 비교
		if(i+1 < (int)noDhand.size()){
			//슌쯔가능성
			if (noDhand[i].getTile() + 1 == noDhand[i + 1].getTile()) {
				// 슌쯔가능성 앞뒤로 패가 존재하면 점수 추가
				if ((noDhand[i].getTile() - 1) % 10 != 0) {
					score += MahjongHandCalculator::checkRemain(hand, Tile(noDhand[i].getTile() - 1));
				}
				if (((noDhand[i].getTile() + 2) % 10 != 0) && (((noDhand[i].getTile() + 2) / 10) != 3)) {
					score += MahjongHandCalculator::checkRemain(hand, Tile(noDhand[i].getTile() + 2));
				}
			}
		}

		//사이 슌쯔가능성
		if ((noDhand[i].getTile() + 2) == (noDhand[i + 1].getTile())) {
			if (((noDhand[i].getTile() + 1) % 10) != 0) {
				score += MahjongHandCalculator::checkRemain(hand, Tile(noDhand[i].getTile() + 1));
			}
		}
		
		//이전과 비교
		if (i - 1 >= 0) {
			//뒤로 슌쯔가능성
			if (noDhand[i].getTile() - 1 == noDhand[i - 1].getTile()) {
				if ((noDhand[i].getTile() - 2) % 10 != 0) {
					score += MahjongHandCalculator::checkRemain(hand, Tile(noDhand[i].getTile() - 2));
				}
				if (((noDhand[i].getTile() + 1) % 10 != 0) && (((noDhand[i].getTile() + 1) / 10) != 3)) {
					score += MahjongHandCalculator::checkRemain(hand, Tile(noDhand[i].getTile() + 1));
				}
			}
			//사이 슌쯔가능성
			if ((noDhand[i].getTile() - 2) == (noDhand[i - 1].getTile())) {
				if (((noDhand[i].getTile() - 1) % 10) != 0) {
					score += MahjongHandCalculator::checkRemain(hand, Tile(noDhand[i].getTile() - 1));
				}
			}
		}

		//다음 다음과 비교
		if (i + 2 < (int)noDhand.size()) {
			//사이 슌쯔가능성
			if ((noDhand[i].getTile() + 2) == (noDhand[i + 2].getTile())) {
				if (((noDhand[i].getTile() + 1) % 10) != 0) {
					score += MahjongHandCalculator::checkRemain(hand, Tile(noDhand[i].getTile() + 1));
				}
			}
		}
		//이전 이전과 비교
		if (i - 2 >= 0) {
			//사이 슌쯔가능성
			if ((noDhand[i].getTile() - 2) == (noDhand[i - 2].getTile())) {
				if (((noDhand[i].getTile() - 1) % 10) != 0) {
					score += MahjongHandCalculator::checkRemain(hand, Tile(noDhand[i].getTile() - 1));
				}
			}
		}
		scoreL.push_back(score);
	}
	//점수와 타일 합체
	for (i = 0; i < (int)noDhand.size(); i++) {
		scoreTile.push_back(ScoreWithTile(noDhand[i], scoreL[i]));
	}
	//낮은순서로 정렬
	std::sort(scoreTile.begin(), scoreTile.end());

	//하위 n종 출력 일단 전부 출력함
	for (i = 0; i < (int)scoreTile.size(); i++) {
		result.push_back(*(scoreTile[i].tile));
	}
	return result;
}
	
