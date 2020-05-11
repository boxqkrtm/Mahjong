#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <windows.h>

#include "MahjongPlayer.h"
#include "Tile.h"
#include "MahjongGame.h"
#include "MahjongAI.h"
#include "MahjongHandCalculator.h"
#include <stack>

using namespace std;

void cleanScreen() {
	int i;
	for (i = 0; i < 100; i++)
		cout << endl;
}

int main() {
	int i;
	vector<int> firstTileData;
	bool isTrue = false;
	string input;
	string korinput;
	stack<int> s;
	bool isLoop = false;
	int loopMax = 3;
	MahjongPlayer player = MahjongPlayer();
	
	while (1) {
		firstTileData.clear();
		player.clearHand();
		cin.clear();
		cout << "----------마작 패 분석기 v0.3----------" << endl << endl;
		cout << "13~14개의 패를 아래와 같이 입력해주세요 (띄어쓰기 상관없음)" << endl;
		cout << "ex) 19만19통19삭 동남서북 백발중중\n\n:";
		//cout << "0X=만수, 1X=통수 2X=삭수 31=동 32=남 33=서 34=북 35=백 36=발 37=중" << endl;

		//패 입력 루프
		do {
			getline(cin, input);

			if (input == "-1") {
				//debug mode code
				isLoop = true;
				loopMax = 3;
				cout << "debug code: "<< -1 <<" on" << endl;
				continue;
			}

			//입력검증
			while (!s.empty()) s.pop();
			firstTileData.clear();
			for (i = 0; i < (int)input.length(); i++) {
				//한글(2바이트 문자) 감지
				if (input[i] & 0x80) {
					korinput = "";
					korinput.append(1, input[i]);
					korinput.append(1, input[i + 1]);
					i++;

					if (korinput == "만") {
						if ((!s.empty())) {
							while (!s.empty()) {
								firstTileData.push_back(s.top());
								s.pop();
							};
						}
						else {
							cout << "잘못된 입력" << endl;
							break;
						}
					}
					else if (korinput == "통") {
						if ((!s.empty())) {
							while (!s.empty()) {
								firstTileData.push_back(s.top() + 10);
								s.pop();
							};
						}
						else {
							cout << "잘못된 입력" << endl;
							break;
						}
					}
					else if (korinput == "삭") {
						if ((!s.empty())) {
							while (!s.empty()) {
								firstTileData.push_back(s.top() + 20);
								s.pop();
							};
						}
						else {
							cout << "잘못된 입력" << endl;
							break;
						}
					}
					else if (korinput == "동") {
						firstTileData.push_back(31);
					}
					else if (korinput == "남") {
						firstTileData.push_back(32);
					}
					else if (korinput == "서") {
						firstTileData.push_back(33);
					}
					else if (korinput == "북") {
						firstTileData.push_back(34);
					}
					else if (korinput == "백") {
						firstTileData.push_back(35);
					}
					else if (korinput == "발") {
						firstTileData.push_back(36);
					}
					else if (korinput == "중") {
						firstTileData.push_back(37);
					}
				}
				else {
					if ((int)input[i] - '0' > 0 && (int)input[i] - '0' < 38) {
						//일반 숫자입력
						s.push((int)input[i] - '0');
						continue;
					}
					else {
						//공백 감지
						continue;
					}
				}
			}

			if ((int)firstTileData.size() == 13 || (int)firstTileData.size() == 14) {
				break;
			}
		} while (1);
	
		//패 저장
		for (i = 0; i < (int)firstTileData.size(); i++) {
			if (firstTileData[i] == 0) {
				continue;
			}
			else {
				Tile tileData(firstTileData[i]);
				player.addHand(tileData);
			}
		}

		//입력체크
		if(!(player.handSizeIncludeCall() >= 13)){
			cout << "잘못된 입력" << endl;
			continue;
		}

		cout << endl;

		cleanScreen();
		player.sortHand();
		player.printHand();
		//안깡 체크
		/*
		if (player.anKanAble().size() > 0) {
			cout << "캉이 가능합니다." << endl;
			for(i=0;i<(int)player.anKanAble().size();i++){
				cout << player.anKanAble()[i].getTile() << "(" << player.anKanAble()[i].getTileName() << ")" << " ";
			}
			cout << endl;
			cout << "0을 입력하면 취소, 패 코드를 입력하면 캉합니다." << endl;
			getline(cin, input);
			if (input != "0") {
				int inputnum = atoi(input.c_str());
				player.anKanNodraw(Tile( inputnum ));
				player.printHand();
			}
			else if (input == "0") {
				cout << "캉 스킵" << endl;
			}
		}
		*/
		if (player.handSizeIncludeCall() == 14) {
			isTrue = player.checkTsumo();
			cout << "쯔모 화료 선언 " << (isTrue == true ? "가능" : "불가능") << endl;
			if (isTrue == false) {
				player.tenpaiFinder(true);
			}
		}
		else if (player.handSizeIncludeCall() == 13) {
			isTrue = player.checkTenpai(true);
			cout << "텐파이 상태 " << (isTrue == true ? "O" : "X") << endl;
			//텐파이 상태라면 대기패 출력
			if(isTrue == true) isTrue = player.tenpaiFinder(true);
		}
		
		//쯔모도 아님, 텐파이도 아님
		if (isTrue == false) {
			//tenpai finder
			isTrue = player.tenpaiFinder(true);
			if (isTrue == false) {
				//하나 버려도 텐파이 안됨 버려봄
				if (isLoop == true) {
					isTrue = player.loopTenpaiFinder(true, loopMax);
				}
				else
				{
					if(player.handSizeIncludeCall() == 14){
						cout << "버림패 추천: " << endl;
						MahjongHandCalculator::printHand(
							MahjongAI::discardAI(player.getHand(), player.chiCount, player.ponCount, player.kanCount)
						);
					}
				}	
			}
		}
		
		cout << "엔터를 입력하면 다음 입력을 받습니다.";
		getline(cin , input);
		cout << "---------------------------------" << endl;
		cleanScreen();
	}
	
	return 0;
}

