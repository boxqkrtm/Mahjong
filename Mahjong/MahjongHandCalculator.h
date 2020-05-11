#pragma once

// <summary> Mahjong Hand Calculate Class
// <para>none</para>
// </summary>
class MahjongHandCalculator {
private:
	//<summary>�Ӹ��� ������ ����� Ŀ��� �� ������ 4������ �м�</summary>
	static bool FourBodyFinder(vector<Tile> hand, vector<Tile> possibleHead, int chiCount, int PonCount, int KanCount);

	static bool const debug = false;
	
public:
	//<summary>���� ���</summary>
	static void printHand(vector<Tile> hand);

	//<summary>������ �ִ� �� �������� �������� ����</summary>
	static int checkRemain(vector<Tile> hand, Tile waitTile);

	//<summary>hand���� �Ӹ��� �� �� �ִ� �ĺ� ����Ʈ �м�</summary>
	static void findHead(vector<Tile> hand, vector<Tile>& possibleHead);

	//<summary>�� ���� ��������</summary>
	static void sortHand(vector<Tile>& hand);

	//<summary>�ϳ��� ������ ������ �� �� �ִ� ��츦 �˻�
	//<para>�������� �� ��¿���(tip)</para>
	//</summary>
	static bool tenpaiFinder(vector<Tile> hand, int chiCount, int ponCount, int kanCount, bool tip = false);


	//<summary>�ϳ��� ����ϸ� �Ǵ� ������ ���� �˻�
	//<para>�������� �� ��¿���(tip)</para>
	//</summary>
	static bool checkTenpai(vector<Tile> hand, bool tip, int discardId, int chiCount, int ponCount, int kanCount);

	//<summary>���ȭ�� �� �� �ִ� ���� �˻�
	//<para>��������</para>
	//</summary>
	static bool checkTsumo(vector<Tile> hand, bool needSort, int chiCount, int ponCount, int kanCount);


	//<summary>��Ĳ �������� �˻� ��ȯ
	//<para>��������</para>
	//</summary>
	static vector<Tile> anKanAble(vector<Tile> hand, bool needSort = true);

	static bool RecursionTenpaiFinder(vector<Tile> hand, int chiCount, int ponCount, int kanCount, bool tip, int depth, int maxdepth, double &percent);
};