#include <iostream>
#include <vector>
#include "Tile.h"
#include "Yaku.h"

bool Yaku::thirteenOrphans(vector<Tile> hand, vector<Tile> possibleHead) {
	int i, j;
	int honorOne = 0;
	int honorTwo = 0;
	if (possibleHead.size() == 1) {
		if (debug == true) cout << "debug Ư����: ���繫�� üũ" << endl;
		int honorList[13] = { 1,9,11,19,21,29,31,32,33,34,35,36,37 };
		int honorCount[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 };
		//���� üũ 191919�������Ϲ����

		//�䱸�� ���� ����
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
				//����ƴ�
			}
		}
		if (honorOne == 12 && honorTwo == 1) {
			//�䱸�� 12�� 1���� 1�� 2���� ���繫�� ex) 191919�������Ϲ������
			return true;
		}
		else {
			return false;
			//���� �ƴ�
		}
	}
	return false;
}

bool Yaku::sevenPairs(vector<Tile> hand, vector<Tile> possibleHead) {
	int i;
	if (possibleHead.size() == 7) {
		if (debug == true) cout << "debug Ư����: ġ���� üũ" << endl;
		//���� �� 4���� ������ 2���� ����� �� ����
		for (i = 0; i < (14 - 2); i++) {
			if (hand[i] == hand[i + 1] && hand[i + 1] == hand[i + 2]) {
				//3���� ���޾� ������� ġ���̴� ��������
				return false;
			}
		}
		//ġ���� ����
		return true;
	}
	//�Ӹ� ���� ����
	return false;
}