#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include "Tile.h"
#include "MahjongHandCalculator.h"

// <summary> Mahjong Game Player Class
// <para>none</para>
// </summary>
class MahjongPlayer {
private:
	//손패
	vector<Tile> hand;

	//후로 한 패 보관 
	vector<Tile> callSpace;

	//후로 카운트
public:
	int chiCount;
	int ponCount;
	int kanCount;

	MahjongPlayer();

	//<summary>손패 모두 삭제</summary>
	void clearHand();

	//<summary>손패에 타일 추가</summary>
	void addHand(Tile tile);

	//<summary>손패 출력</summary>
	void printHand();

	void sortHand();

	//<summary>손패 정렬</summary>
	void sortHand(vector<Tile>& hand);

	//<summary>하나를 버려서 텐파이 할 수 있는 경우를 반환</summary>
	bool tenpaiFinder(bool tip = false);

	//<summary>둘을 버려서 텐파이 할 수 있는 경우를 검사</summary>
	bool doubleTenpaiFinder(bool tip = false);

	//<summary>텐파이 상태 반환
	//<para>대기패 출력여부</para>
	//</summary>
	bool checkTenpai(bool tip = false);

	//<summary>쯔모화료 가능여부 반환</summary>
	bool checkTsumo();

	//<summary>손패의 개수 반환</summary>
	int handSize();	

	//<summary>손패의 개수 반환(치퐁캉 포함 14개로 만듬)</summary>
	int handSizeIncludeCall();

	//<summary>손패 데이터 반환</summary>
	vector<Tile> getHand();

	//<summary>안깡 가능한 리스트 반환</summary>
	vector<Tile> anKanAble();

	//<summary>안깡 실행</summary>
	void anKan(Tile target);

	//<summary>안깡 실행(영상패 안뽑음) 테스트용</summary>
	void anKanNodraw(Tile target);

	bool loopTenpaiFinder(bool tip = false ,int maxdepth = 3);
};