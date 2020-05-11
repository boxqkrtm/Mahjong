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
		//쯔모의 최소조건 중하나인 패가 14개(캉의 경우 몸통한개(3개))로 침
		if(tip == true) cout << "버릴 수 없습니다." << endl;
		return false;
	}
	vector<Tile> tempHand;
	sortHand(hand);
	for (i = 0; i < 14; i++) {
		//불필요한 중복수행 방지
		if (i != 0) {
			if (hand[i - 1] == hand[i]) {
				continue;
			}
		}

		//임시패에 실제 손패에서 하나를 뺀 뒤 텐파이 검사
		tempHand.clear();
		tempHand.assign(hand.begin(), hand.end());
		discardTile.setTile(hand[i].getTile());
		tempHand.erase(tempHand.begin() + i);

		if (checkTenpai(tempHand, tip, discardTile.getTile(), chiCount, ponCount, kanCount)) {
			result = true;
		}
	}
	if (result == false) {
		if (tip == true) cout << "어떠한 패를 버려도 텐파이 할 수 없습니다." << endl;
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
		//버릴 패
		discardTiles.push_back(waitDiscard[i]);
		for (j = 0; j < (int)tempHand.size(); j++) {
			if (tempHand[j] == discardTiles[0]) {
				tempHand.erase(tempHand.begin() + j);
				break;
			}
		}
		for (j = 0; j < (int)waitTsumo.size(); j++) {
			//쯔모 할 패
			tempHand.push_back(Tile(waitTsumo[j]));
			//텐파이 체크
			if (tenpaiFinder(tempHand, chiCount, ponCount, kanCount, false) == true) {
				tsumoTiles.push_back(Tile(waitTsumo[j]));
				result = true;
				if (tip == false && result == true) {
					//패 확인을 안하는경우 연산 스킵
					if (depth == maxdepth && false) {
						//마지막 예시 출력
						printHand(tempHand);
						tenpaiFinder(tempHand, chiCount, ponCount, kanCount, true);
					}
					//return true;
				}
			}
			else {
				//없을 시 재귀로 최대 깊이까지 더 깊게 확인
				
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
				//없음
			}
			tempHand.pop_back();
		}
		//결과 수집
		if (tip == true && !tsumoTiles.empty()) {
			PossibleList tmp;
			tmp.str = "";
			tmp.str += "-----------------------깊이" + to_string(depth) + (string)" " + discardTiles[0].getTileName() + "을 버린 뒤" + "-----------------------\n";

			int possiblity = 0;
			for (j = 0; j < (int)tsumoTiles.size(); j++) {
				tmp.str += tsumoTiles[j].getTileName() + "(" + to_string(checkRemain(tempHand, tsumoTiles[j])) + ") ";
				possiblity += (checkRemain(tempHand, tsumoTiles[j]));
			}
			tmp.str += "을 쯔모 (텐파이 가능성 " + to_string((double)possiblity / 136.0 * 100.0 * accumulatePercent[i]) + ")";
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
	//결과 정렬 후 출력
	if (tip == true || result == true) {
		sort(possibleList.begin(), possibleList.end(), greater<PossibleList>());
		for (j = 0; j < (int)possibleList.size(); j++) {
			cout << possibleList[j].str << endl;
		}
	}
	//cout << "종료" << depth << "깊이" << endl;
	return result;
}

bool MahjongHandCalculator::checkTenpai(vector<Tile> hand, bool tip, int discardId, int chiCount, int ponCount, int kanCount) {
	int i;
	int tileList[] = { 1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,21,22,23,24,25,26,27,28,29,31,32,33,34,35,36,37 };
	vector<Tile> tempHand;
	bool result = false;
	vector<Tile> waitTile;

	if (checkTsumo(hand, true, chiCount, ponCount, kanCount)) {
		//쯔모 상태
		if (tip == true) {
			cout << "쯔모 가능한 상태입니다." << endl;
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
				cout << Tile(discardId).getTileName() << "를 버릴 시 ";
			}
			cout << "대기패 : ";
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
	vector<Tile> needGroup;//필요한 패 
	vector<Tile> tempHand;//머리를 제외한 나머지 손패 보관 변수 
	int removeCount;
	Tile head(0);
	string output = "";

	//일반적인 쯔모 형태 분석 머리기준 몸통4개 탐색 
	for (i = 0; i < (int)possibleHead.size(); i++) {
		string output = "";
		head.setTile(possibleHead[i].getTile());//머리선택 
		tempHand.clear();
		tempHand.assign(hand.begin(), hand.end());//손패 복사 
		if (debug == true) {
			output.append(head.getTileName());
			output.append(head.getTileName());
			output.append(" ");
		}

		//머리부분 제거 
		removeCount = 0;
		jmax = 14;
		for (j = 0; j < jmax; j++) {
			if (removeCount == 2) {
				//머리부분 제거 완료 
				break;
			}
			if (tempHand[j] == head) {
				//머리 찾은 후 삭제 
				tempHand.erase(tempHand.begin() + j);
				removeCount++;
				j--;
				jmax--;//현재 인덱스가 사라졌으므로 jmax 사이즈 -1 
			}
		}
		if (debug == true) {
			cout << "debug:머리 제외 ";
			printHand(tempHand);
		}

		bodyCount = chiCount + ponCount + kanCount;//몸통개수 계산 
		//몸통 4개 찾기

		int straight = 0;
		int same = 0;
		vector<int> delList;
		int searchSwitch = 0;//0커쯔 1슌쯔
		//커쯔 검사(111) 슌쯔 검사(123)
		delList.clear();
		for (j = 0; j < (int)tempHand.size()-1; j++) {
			//커쯔 시도
			if (searchSwitch == 0) {
				//한번에 커쯔를 찾는 구문
				if (!((j + 2) < (int)tempHand.size())) {
					//커쯔 분석할 넥스트가 없음 바로넘김
					searchSwitch = 1;
					j = -1;
					continue;
					break;
				}

				//커쯔찾기
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
						j = -1;//다시루프
						continue;
					}
				}

				//cout << "커쯔 실패" << tempHand[j].getTileName() << " " << tempHand[j + 1].getTileName() << " " << tempHand[j + 2].getTileName() << endl;
				//슌쯔로 전환 처음부터 시작
				searchSwitch = 1;
				j = -1;
				continue;
			}
			else {
				if ((tempHand[j].getTile() / 10) == 3) {
					//자패는 슌쯔가 될 수 없음 스킵함 
					continue;
				}
				if ((tempHand[j].getTile() + 1) == (tempHand[j + 1].getTile())) {
					//현재항목과 다음항목이 1차이남 슌쯔가능성 +1
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
					//같다면 다음 항목 확인
					continue;
				}
				else {
					//+1이거나 같음 모두아닌 슌쯔 끊어짐 =>커쯔 슌쯔 둘다 시도햇으나 없음
					straight = 0;
					delList.clear();
					break;
				}
				if (straight == 2) {
					//2번 이어진 슌쯔 몸통발견 
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
					// 처음부터 다시 체크 슌쯔 탐색으로 전환
					j = -1;
					searchSwitch = 0;
				}
			}
		}
		if (debug == true) {
			cout << "debug: " << output << endl;
		}
		//몸통4개 발견 쯔모상태
		if (bodyCount == 4) {
			return true;
		}
	}
	return false;
}

void MahjongHandCalculator::findHead(vector<Tile> hand, vector<Tile>& possibleHead) {
	int i;
	//hand에서 머리가 되 수 있는 후보 리스트 분석
	sortHand(hand);
	for (i = 0; i < (14 - 1); i++) {
		//i번째와 i+1번째를 비교 같으면 머리가 가능함
		if (hand[i] == hand[i + 1]) {
			possibleHead.push_back(hand[i]);
			//머리 발견시 index를 2추가하고 가능한 머리목록 추가 
			//cout<<"머리 발견 "<<hand[i].getTileName()<<endl;
			i++;
			continue;//for i++
		}
	}

	//중복제거
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
	vector<Tile> needGroup;//필요한 패 
	vector<Tile> tempHand;//머리를 제외한 나머지 손패 보관 변수 
	Tile head(0);

	//if (debug == true) cout << "debug:쯔모화료 가능여부 체크" << endl;

	if (needSort == true) {
		sortHand(hand); //패 정렬 
	}

	if (((int)hand.size() + ((chiCount * 3) + (ponCount * 3) + (kanCount * 3))) != 14) {
		//쯔모에 필요한 패의 개수 체크
		//if(debug == true) cout << "debug:쯔모에 필요한 패부족" << endl;
		return false;
	}

	//머리 가능한 후보 리스트 계산
	findHead(hand, possibleHead);

	//--특수역 체크
	//pass 로컬룰

	//머리없음 화료불가
	if (possibleHead.size() == 0) {
		if (debug == true) cout << "debug 머리 0개 탐지됨 쯔모화료 불가" << endl;
		return false;
	}

	//--특수역 체크--
	//치또이 or 국사무쌍
	if (Yaku::sevenPairs(hand, possibleHead) || 
		Yaku::thirteenOrphans(hand, possibleHead)) {
		return true;
	}
	
	//if (debug == true) cout << "debug 일반 탐색 시작" << endl;
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