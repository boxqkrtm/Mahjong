#pragma once

// <summary> Mahjong Hand Calculate Class
// <para>none</para>
// </summary>
class MahjongHandCalculator {
private:
	//<summary>머리를 제외한 슌쯔와 커쯔로 된 몸통이 4개인지 분석</summary>
	static bool FourBodyFinder(vector<Tile> hand, vector<Tile> possibleHead, int chiCount, int PonCount, int KanCount);

	static bool const debug = false;
	
public:
	//<summary>손패 출력</summary>
	static void printHand(vector<Tile> hand);

	//<summary>가지고 있는 패 기준으로 남은개수 예측</summary>
	static int checkRemain(vector<Tile> hand, Tile waitTile);

	//<summary>hand에서 머리가 될 수 있는 후보 리스트 분석</summary>
	static void findHead(vector<Tile> hand, vector<Tile>& possibleHead);

	//<summary>패 정렬 오름차순</summary>
	static void sortHand(vector<Tile>& hand);

	//<summary>하나를 버려서 텐파이 할 수 있는 경우를 검사
	//<para>손패정보 및 출력여부(tip)</para>
	//</summary>
	static bool tenpaiFinder(vector<Tile> hand, int chiCount, int ponCount, int kanCount, bool tip = false);


	//<summary>하나를 쯔모하면 되는 텐파이 상태 검사
	//<para>손패정보 및 출력여부(tip)</para>
	//</summary>
	static bool checkTenpai(vector<Tile> hand, bool tip, int discardId, int chiCount, int ponCount, int kanCount);

	//<summary>쯔모화료 할 수 있는 상태 검사
	//<para>손패정보</para>
	//</summary>
	static bool checkTsumo(vector<Tile> hand, bool needSort, int chiCount, int ponCount, int kanCount);


	//<summary>안캉 가능한지 검사 반환
	//<para>손패정보</para>
	//</summary>
	static vector<Tile> anKanAble(vector<Tile> hand, bool needSort = true);

	static bool RecursionTenpaiFinder(vector<Tile> hand, int chiCount, int ponCount, int kanCount, bool tip, int depth, int maxdepth, double &percent);
};