#include <iostream>
#include <vector>
#include <algorithm>
#include "Tile.h"
#include "MahjongPlayer.h"
#include "MahjongHandCalculator.h"

MahjongPlayer::MahjongPlayer() {
	chiCount = 0;
	ponCount = 0;
	kanCount = 0;
}

void MahjongPlayer::clearHand() {
	hand.clear();
}

void MahjongPlayer::addHand(Tile tile) {
	hand.push_back(tile);
}

void MahjongPlayer::printHand(){
	int i;
	for (i = 0; i < (int)hand.size(); i++) {
		cout << hand[i].getTileName() << " ";
	}
	cout << "( ";
	for (i = 0; i < (int)callSpace.size(); i++) {
		cout << callSpace[i].getTileName() << " ";
	}
	cout << ")" << endl;
}

void MahjongPlayer::sortHand() {
	sort(hand.begin(), hand.end());
}

void MahjongPlayer::sortHand(vector<Tile>& hand) {
	sort(hand.begin(), hand.end());
}

bool MahjongPlayer::checkTenpai(bool tip) {
	return MahjongHandCalculator::checkTenpai(hand, tip, 0, chiCount, ponCount, kanCount);
}

bool MahjongPlayer::checkTsumo() {
	return MahjongHandCalculator::checkTsumo(hand, true, chiCount, ponCount, kanCount);
}

bool MahjongPlayer::tenpaiFinder(bool tip) {
	return MahjongHandCalculator::tenpaiFinder(hand, chiCount, ponCount, kanCount, tip);
}

bool MahjongPlayer::loopTenpaiFinder(bool tip, int maxdepth) {
	double d;
	return MahjongHandCalculator::RecursionTenpaiFinder(hand, chiCount, ponCount, kanCount, tip, 1, maxdepth, d);
}

int MahjongPlayer::handSize() {
	return (int)hand.size();
}

int MahjongPlayer::handSizeIncludeCall() {
	return (int)hand.size() + (chiCount + ponCount + kanCount)*3;
}

vector<Tile> MahjongPlayer::anKanAble() {
	return MahjongHandCalculator::anKanAble(hand);
}

void MahjongPlayer::anKan(Tile target) {
	//Todo MahjongGame ¿¬µ¿
	return;
}

void MahjongPlayer::anKanNodraw(Tile target) {
	int i, j;
	int eraseCount = 0;
	for (i = 0; i < (int)MahjongHandCalculator::anKanAble(hand).size(); i++) {
		if (MahjongHandCalculator::anKanAble(hand)[i] == target) {
			//¾È±ø°¡´É
			for (j = 0; j < (int)hand.size(); j++) {
				if (hand[j] == target) {
					hand.erase(hand.begin() + j);
					eraseCount++;
					j = -1;
				}
				if (eraseCount == 4) {
					callSpace.push_back(target);
					callSpace.push_back(target);
					callSpace.push_back(target);
					callSpace.push_back(target);
					kanCount++;
					cout << "-----------Ä²------------" << endl;
					return;
				}
			}
		}
	}
}

vector<Tile> MahjongPlayer::getHand() {
	return hand;
}
