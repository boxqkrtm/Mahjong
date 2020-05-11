#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <omp.h>

#include "Tile.h"
#include "MahjongHandCalculator.h"
#include "MahjongAI.h"
#include "Yaku.h"

class PossibleList {
public:
	string str = "";
	double percent = 0;
	bool operator <(PossibleList p) const{
		return (this->percent < p.percent);
	}
	bool operator >(PossibleList p) const {
		return (this->percent > p.percent);
	}
};

void MahjongHandCalculator::printHand(vector<Tile> hand) {
	int i;
	for (i = 0; i < (int)hand.size(); i++) {
		cout << hand[i].getTileName() << " ";
	}
	cout << endl;
}


void MahjongHandCalculator::sortHand(vector<Tile>& hand) {
	sort(hand.begin(), hand.end());
}

int MahjongHandCalculator::checkRemain(vector<Tile> hand, Tile waitTile) {
	int result = 4;
	int i;
	for (i = 0; i < (int)hand.size(); i++) {
		if (hand[i] == waitTile) {
			result--;
		}
	}
	return result;
}

bool MahjongHandCalculator::tenpaiFinder(vector<Tile> hand, int chiCount, int ponCount, int kanCount, bool tip) {
	int i;
	bool result = false;
	Tile discardTile(0);
	if ((hand.size() + ((chiCount * 3) + (ponCount * 3) + (kanCount * 3))) != 14) {
		//����� �ּ����� ���ϳ��� �а� 14��(Ĳ�� ��� �����Ѱ�(3��))�� ħ
		if(tip == true) cout << "���� �� �����ϴ�." << endl;
		return false;
	}
	vector<Tile> tempHand;
	sortHand(hand);
	for (i = 0; i < 14; i++) {
		//���ʿ��� �ߺ����� ����
		if (i != 0) {
			if (hand[i - 1] == hand[i]) {
				continue;
			}
		}

		//�ӽ��п� ���� ���п��� �ϳ��� �� �� ������ �˻�
		tempHand.clear();
		tempHand.assign(hand.begin(), hand.end());
		discardTile.setTile(hand[i].getTile());
		tempHand.erase(tempHand.begin() + i);

		if (checkTenpai(tempHand, tip, discardTile.getTile(), chiCount, ponCount, kanCount)) {
			result = true;
		}
	}
	if (result == false) {
		if (tip == true) cout << "��� �и� ������ ������ �� �� �����ϴ�." << endl;
	}
	return result;
}

bool MahjongHandCalculator::RecursionTenpaiFinder(vector<Tile> hand, int chiCount, int ponCount, int kanCount, bool tip, int depth, int maxdepth, double &percent) {
	if (depth > maxdepth) {
		return false;
	}
	int i, j;
	vector<Tile> waitTsumo = MahjongAI::demandingAI(hand);
	vector<Tile> waitDiscard = MahjongAI::discardAI(hand, chiCount, ponCount, kanCount);
	vector<double> accumulatePercent;
	int tileList[] = { 1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,21,22,23,24,25,26,27,28,29,31,32,33,34,35,36,37 };
	vector<Tile> tempHand;
	vector<Tile> discardTiles;
	vector<Tile> tsumoTiles;
	if (hand.size() + chiCount * 3 + ponCount * 3 + kanCount * 3 != 14) {
		return false;
	}
	vector<PossibleList> possibleList;
	bool result = false;
	
	for (i = 0; i < (int)waitDiscard.size(); i++) {
		tempHand.clear();
		discardTiles.clear();
		tsumoTiles.clear();
		accumulatePercent.clear();
		tempHand.assign(hand.begin(), hand.end());
		if (i != 0) {
			if (tempHand[i - 1] == tempHand[i]) {
				continue;
			}
		}
		//���� ��
		discardTiles.push_back(waitDiscard[i]);
		for (j = 0; j < (int)tempHand.size(); j++) {
			if (tempHand[j] == discardTiles[0]) {
				tempHand.erase(tempHand.begin() + j);
				break;
			}
		}
		for (j = 0; j < (int)waitTsumo.size(); j++) {
			//��� �� ��
			tempHand.push_back(Tile(waitTsumo[j]));
			//������ üũ
			if (tenpaiFinder(tempHand, chiCount, ponCount, kanCount, false) == true) {
				tsumoTiles.push_back(Tile(waitTsumo[j]));
				result = true;
				if (tip == false && result == true) {
					//�� Ȯ���� ���ϴ°�� ���� ��ŵ
					if (depth == maxdepth && false) {
						//������ ���� ���
						printHand(tempHand);
						tenpaiFinder(tempHand, chiCount, ponCount, kanCount, true);
					}
					//return true;
				}
			}
			else {
				//���� �� ��ͷ� �ִ� ���̱��� �� ��� Ȯ��
				
				if (depth == 1) {
					double p = 1.0;
					if (RecursionTenpaiFinder(tempHand, chiCount, ponCount, kanCount, false, depth + 1, maxdepth, p) == true) {
						tsumoTiles.push_back(Tile(tileList[j]));
						result = true;
						accumulatePercent.push_back(p);
					}
				}
				else {
					if (RecursionTenpaiFinder(tempHand, chiCount, ponCount, kanCount, false, depth + 1, maxdepth, percent) == true) {
						tsumoTiles.push_back(Tile(tileList[j]));
						result = true;
					}
				}
				//����
			}
			tempHand.pop_back();
		}
		//��� ����
		if (tip == true && !tsumoTiles.empty()) {
			PossibleList tmp;
			tmp.str = "";
			tmp.str += "-----------------------����" + to_string(depth) + (string)" " + discardTiles[0].getTileName() + "�� ���� ��" + "-----------------------\n";

			int possiblity = 0;
			for (j = 0; j < (int)tsumoTiles.size(); j++) {
				tmp.str += tsumoTiles[j].getTileName() + "(" + to_string(checkRemain(tempHand, tsumoTiles[j])) + ") ";
				possiblity += (checkRemain(tempHand, tsumoTiles[j]));
			}
			tmp.str += "�� ��� (������ ���ɼ� " + to_string((double)possiblity / 136.0 * 100.0 * accumulatePercent[i]) + ")";
			tmp.percent = (double)possiblity / 136.0 * 100.0 * accumulatePercent[i];
			possibleList.push_back(tmp);
			//tenpaiFinder(tempHand, chiCount, ponCount, kanCount, true);	
		}
		else if(tip == false &&!tsumoTiles.empty() && depth != 1) {
			int sum = 0;
			for (j = 0; j < (int)tsumoTiles.size(); j++) {
				sum += checkRemain(tempHand, tsumoTiles[j]);
			}
				percent *= sum / 136.0;
			return true;
		}
	}
	//��� ���� �� ���
	if (tip == true || result == true) {
		sort(possibleList.begin(), possibleList.end(), greater<PossibleList>());
		for (j = 0; j < (int)possibleList.size(); j++) {
			cout << possibleList[j].str << endl;
		}
	}
	//cout << "����" << depth << "����" << endl;
	return result;
}

bool MahjongHandCalculator::checkTenpai(vector<Tile> hand, bool tip, int discardId, int chiCount, int ponCount, int kanCount) {
	int i;
	int tileList[] = { 1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,21,22,23,24,25,26,27,28,29,31,32,33,34,35,36,37 };
	vector<Tile> tempHand;
	bool result = false;
	vector<Tile> waitTile;

	if (checkTsumo(hand, true, chiCount, ponCount, kanCount)) {
		//��� ����
		if (tip == true) {
			cout << "��� ������ �����Դϴ�." << endl;
		}
		return true;
	}

	for (i = 0; i < 34; i++) {
		tempHand.clear();
		tempHand.assign(hand.begin(), hand.end());
		tempHand.push_back(Tile(tileList[i]));

		if (checkTsumo(tempHand, true, chiCount, ponCount, kanCount) == true) {
			result = true;
			waitTile.push_back(Tile(tileList[i]));
		}
	}

	if (tip == true) {
		if ((int)waitTile.size() > 0) {
			if (discardId >= 1) {
				cout << Tile(discardId).getTileName() << "�� ���� �� ";
			}
			cout << "����� : ";
			for (i = 0; i < (int)waitTile.size(); i++) {
				cout << waitTile[i].getTileName() << "(" << checkRemain(hand, waitTile[i]) << ")" << " ";
			}
			cout << endl;
		}
	}
	return result;
}

bool MahjongHandCalculator::FourBodyFinder(vector<Tile> hand, vector<Tile> possibleHead, int chiCount, int ponCount, int kanCount) {
	int i, j, k;
	int jmax;
	int bodyCount;
	vector<Tile> needGroup;//�ʿ��� �� 
	vector<Tile> tempHand;//�Ӹ��� ������ ������ ���� ���� ���� 
	int removeCount;
	Tile head(0);
	string output = "";

	//�Ϲ����� ��� ���� �м� �Ӹ����� ����4�� Ž�� 
	for (i = 0; i < (int)possibleHead.size(); i++) {
		string output = "";
		head.setTile(possibleHead[i].getTile());//�Ӹ����� 
		tempHand.clear();
		tempHand.assign(hand.begin(), hand.end());//���� ���� 
		if (debug == true) {
			output.append(head.getTileName());
			output.append(head.getTileName());
			output.append(" ");
		}

		//�Ӹ��κ� ���� 
		removeCount = 0;
		jmax = 14;
		for (j = 0; j < jmax; j++) {
			if (removeCount == 2) {
				//�Ӹ��κ� ���� �Ϸ� 
				break;
			}
			if (tempHand[j] == head) {
				//�Ӹ� ã�� �� ���� 
				tempHand.erase(tempHand.begin() + j);
				removeCount++;
				j--;
				jmax--;//���� �ε����� ��������Ƿ� jmax ������ -1 
			}
		}
		if (debug == true) {
			cout << "debug:�Ӹ� ���� ";
			printHand(tempHand);
		}

		bodyCount = chiCount + ponCount + kanCount;//���밳�� ��� 
		//���� 4�� ã��

		int straight = 0;
		int same = 0;
		vector<int> delList;
		int searchSwitch = 0;//0Ŀ�� 1����
		//Ŀ�� �˻�(111) ���� �˻�(123)
		delList.clear();
		for (j = 0; j < (int)tempHand.size()-1; j++) {
			//Ŀ�� �õ�
			if (searchSwitch == 0) {
				//�ѹ��� Ŀ�긦 ã�� ����
				if (!((j + 2) < (int)tempHand.size())) {
					//Ŀ�� �м��� �ؽ�Ʈ�� ���� �ٷγѱ�
					searchSwitch = 1;
					j = -1;
					continue;
					break;
				}

				//Ŀ��ã��
				if (j + 2 < (int)tempHand.size()) {
					if ((tempHand[j] == tempHand[j + 1]) && (tempHand[j + 1] == tempHand[j + 2])) {
						
						if (debug == true) {
							output.append(tempHand[j].getTileName());
							output.append(tempHand[j+1].getTileName());
							output.append(tempHand[j+2].getTileName());
							output.append(" ");
						}
						tempHand.erase(tempHand.begin() + j + 2);
						tempHand.erase(tempHand.begin() + j + 1);
						tempHand.erase(tempHand.begin() + j + 0);
						bodyCount++;
						j = -1;//�ٽ÷���
						continue;
					}
				}

				//cout << "Ŀ�� ����" << tempHand[j].getTileName() << " " << tempHand[j + 1].getTileName() << " " << tempHand[j + 2].getTileName() << endl;
				//����� ��ȯ ó������ ����
				searchSwitch = 1;
				j = -1;
				continue;
			}
			else {
				if ((tempHand[j].getTile() / 10) == 3) {
					//���д� ���갡 �� �� ���� ��ŵ�� 
					continue;
				}
				if ((tempHand[j].getTile() + 1) == (tempHand[j + 1].getTile())) {
					//�����׸�� �����׸��� 1���̳� ���갡�ɼ� +1
					if (straight == 0) {
						delList.push_back(j);
						delList.push_back(j + 1);
					}
					else {
						delList.push_back(j + 1);
					}
					straight++;
				}
				else if (tempHand[j] == tempHand[j + 1]) {
					//���ٸ� ���� �׸� Ȯ��
					continue;
				}
				else {
					//+1�̰ų� ���� ��ξƴ� ���� ������ =>Ŀ�� ���� �Ѵ� �õ������� ����
					straight = 0;
					delList.clear();
					break;
				}
				if (straight == 2) {
					//2�� �̾��� ���� ����߰� 
					if (debug == true) {
						for (k = 0; k < (int)delList.size(); k++) {
							output.append(tempHand[delList[k]].getTileName());
						}
						output.append(" ");
					}
					//
					for (k = (int)delList.size() - 1; k >= 0; k--) {
						tempHand.erase(tempHand.begin() + delList[k]);
					}
					bodyCount++;
					delList.clear();
					straight = 0;
					// ó������ �ٽ� üũ ���� Ž������ ��ȯ
					j = -1;
					searchSwitch = 0;
				}
			}
		}
		if (debug == true) {
			cout << "debug: " << output << endl;
		}
		//����4�� �߰� ������
		if (bodyCount == 4) {
			return true;
		}
	}
	return false;
}

void MahjongHandCalculator::findHead(vector<Tile> hand, vector<Tile>& possibleHead) {
	int i;
	//hand���� �Ӹ��� �� �� �ִ� �ĺ� ����Ʈ �м�
	sortHand(hand);
	for (i = 0; i < (14 - 1); i++) {
		//i��°�� i+1��°�� �� ������ �Ӹ��� ������
		if (hand[i] == hand[i + 1]) {
			possibleHead.push_back(hand[i]);
			//�Ӹ� �߽߰� index�� 2�߰��ϰ� ������ �Ӹ���� �߰� 
			//cout<<"�Ӹ� �߰� "<<hand[i].getTileName()<<endl;
			i++;
			continue;//for i++
		}
	}

	//�ߺ�����
	for (i = 0; i < (int)possibleHead.size()-1; i++) {
		if (possibleHead[i] == possibleHead[i + 1]) {
			possibleHead.erase(possibleHead.begin() + i+1);
			i--;
			continue;
		}
	}
}

bool MahjongHandCalculator::checkTsumo(vector<Tile> hand, bool needSort, int chiCount, int ponCount, int kanCount) {
	vector<Tile> possibleHead;
	vector<Tile> needGroup;//�ʿ��� �� 
	vector<Tile> tempHand;//�Ӹ��� ������ ������ ���� ���� ���� 
	Tile head(0);

	//if (debug == true) cout << "debug:���ȭ�� ���ɿ��� üũ" << endl;

	if (needSort == true) {
		sortHand(hand); //�� ���� 
	}

	if (((int)hand.size() + ((chiCount * 3) + (ponCount * 3) + (kanCount * 3))) != 14) {
		//��� �ʿ��� ���� ���� üũ
		//if(debug == true) cout << "debug:��� �ʿ��� �к���" << endl;
		return false;
	}

	//�Ӹ� ������ �ĺ� ����Ʈ ���
	findHead(hand, possibleHead);

	//--Ư���� üũ
	//pass ���÷�

	//�Ӹ����� ȭ��Ұ�
	if (possibleHead.size() == 0) {
		if (debug == true) cout << "debug �Ӹ� 0�� Ž���� ���ȭ�� �Ұ�" << endl;
		return false;
	}

	//--Ư���� üũ--
	//ġ���� or ���繫��
	if (Yaku::sevenPairs(hand, possibleHead) || 
		Yaku::thirteenOrphans(hand, possibleHead)) {
		return true;
	}
	
	//if (debug == true) cout << "debug �Ϲ� Ž�� ����" << endl;
	return FourBodyFinder(hand, possibleHead, chiCount, ponCount, kanCount);
}

vector<Tile> MahjongHandCalculator::anKanAble(vector<Tile> hand, bool needSort) {
	int i, same;
	vector<Tile> result;
	if (needSort) {
		sortHand(hand);
	}
	same = 0;
	for (i = 0; i < (int)hand.size()-1; i++) {
		if (hand[i] == hand[i + 1]) {
			same += 1;
		}
		else {
			same = 0;
		}
		if (same == 3) {
			result.push_back(Tile(hand[i].getTile()));
			i++;
			continue;
		}
	}
	return result;
}