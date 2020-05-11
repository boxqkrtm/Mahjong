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
			//����
			result.push_back(hand[i]);
		}
		if(hand[i].getTile() > 0 && hand[i].getTile() < 30){
			//����
			if ((hand[i].getTile() - 1) % 10 != 0) {
				result.push_back(Tile(hand[i].getTile()+1));
			}
			if ((hand[i].getTile() - 1) % 10 != 0) {
				result.push_back(Tile(hand[i].getTile()-1));
			}
		}
	}

	//�ߺ����� �Ұ�������
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
		targetAI�� ����� �ֺ��� ���� �ִٸ� �׿����� ������ AI�߰�
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

	//�ѹ� ������ �����̰��ɽ� ������ ��õ
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

	//����� �ߺ�����
	noDhand.assign(hand.begin(), hand.end());
	MahjongHandCalculator::sortHand(noDhand);
	for (i = 0; i < (int)noDhand.size()-1; i++) {
		if (noDhand[i] == noDhand[i + 1]) {
			noDhand.erase(noDhand.begin() + i);
			i = i-1;
		}
	}

	//�� �̿ܿ��� ������ �� ����
	for (i = 0; i < (int)noDhand.size(); i++) {
		score = 0;

		//Ŀ�갡�ɼ� �տ� ������ ��������� �ö�
		score += 3-MahjongHandCalculator::checkRemain(hand, noDhand[i]);

		//���갡�ɼ� �ϴ� ������
		if (noDhand[i].getTile() / 10 == 3) {
			scoreL.push_back(score);
			continue;
		}

		//Todo �̹� checkRemain�� �л� �������� ��ȭ�ؾߵ�

		//������ ��
		if(i+1 < (int)noDhand.size()){
			//���갡�ɼ�
			if (noDhand[i].getTile() + 1 == noDhand[i + 1].getTile()) {
				// ���갡�ɼ� �յڷ� �а� �����ϸ� ���� �߰�
				if ((noDhand[i].getTile() - 1) % 10 != 0) {
					score += MahjongHandCalculator::checkRemain(hand, Tile(noDhand[i].getTile() - 1));
				}
				if (((noDhand[i].getTile() + 2) % 10 != 0) && (((noDhand[i].getTile() + 2) / 10) != 3)) {
					score += MahjongHandCalculator::checkRemain(hand, Tile(noDhand[i].getTile() + 2));
				}
			}
		}

		//���� ���갡�ɼ�
		if ((noDhand[i].getTile() + 2) == (noDhand[i + 1].getTile())) {
			if (((noDhand[i].getTile() + 1) % 10) != 0) {
				score += MahjongHandCalculator::checkRemain(hand, Tile(noDhand[i].getTile() + 1));
			}
		}
		
		//������ ��
		if (i - 1 >= 0) {
			//�ڷ� ���갡�ɼ�
			if (noDhand[i].getTile() - 1 == noDhand[i - 1].getTile()) {
				if ((noDhand[i].getTile() - 2) % 10 != 0) {
					score += MahjongHandCalculator::checkRemain(hand, Tile(noDhand[i].getTile() - 2));
				}
				if (((noDhand[i].getTile() + 1) % 10 != 0) && (((noDhand[i].getTile() + 1) / 10) != 3)) {
					score += MahjongHandCalculator::checkRemain(hand, Tile(noDhand[i].getTile() + 1));
				}
			}
			//���� ���갡�ɼ�
			if ((noDhand[i].getTile() - 2) == (noDhand[i - 1].getTile())) {
				if (((noDhand[i].getTile() - 1) % 10) != 0) {
					score += MahjongHandCalculator::checkRemain(hand, Tile(noDhand[i].getTile() - 1));
				}
			}
		}

		//���� ������ ��
		if (i + 2 < (int)noDhand.size()) {
			//���� ���갡�ɼ�
			if ((noDhand[i].getTile() + 2) == (noDhand[i + 2].getTile())) {
				if (((noDhand[i].getTile() + 1) % 10) != 0) {
					score += MahjongHandCalculator::checkRemain(hand, Tile(noDhand[i].getTile() + 1));
				}
			}
		}
		//���� ������ ��
		if (i - 2 >= 0) {
			//���� ���갡�ɼ�
			if ((noDhand[i].getTile() - 2) == (noDhand[i - 2].getTile())) {
				if (((noDhand[i].getTile() - 1) % 10) != 0) {
					score += MahjongHandCalculator::checkRemain(hand, Tile(noDhand[i].getTile() - 1));
				}
			}
		}
		scoreL.push_back(score);
	}
	//������ Ÿ�� ��ü
	for (i = 0; i < (int)noDhand.size(); i++) {
		scoreTile.push_back(ScoreWithTile(noDhand[i], scoreL[i]));
	}
	//���������� ����
	std::sort(scoreTile.begin(), scoreTile.end());

	//���� n�� ��� �ϴ� ���� �����
	for (i = 0; i < (int)scoreTile.size(); i++) {
		result.push_back(*(scoreTile[i].tile));
	}
	return result;
}
	
