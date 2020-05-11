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
	//����
	vector<Tile> hand;

	//�ķ� �� �� ���� 
	vector<Tile> callSpace;

	//�ķ� ī��Ʈ
public:
	int chiCount;
	int ponCount;
	int kanCount;

	MahjongPlayer();

	//<summary>���� ��� ����</summary>
	void clearHand();

	//<summary>���п� Ÿ�� �߰�</summary>
	void addHand(Tile tile);

	//<summary>���� ���</summary>
	void printHand();

	void sortHand();

	//<summary>���� ����</summary>
	void sortHand(vector<Tile>& hand);

	//<summary>�ϳ��� ������ ������ �� �� �ִ� ��츦 ��ȯ</summary>
	bool tenpaiFinder(bool tip = false);

	//<summary>���� ������ ������ �� �� �ִ� ��츦 �˻�</summary>
	bool doubleTenpaiFinder(bool tip = false);

	//<summary>������ ���� ��ȯ
	//<para>����� ��¿���</para>
	//</summary>
	bool checkTenpai(bool tip = false);

	//<summary>���ȭ�� ���ɿ��� ��ȯ</summary>
	bool checkTsumo();

	//<summary>������ ���� ��ȯ</summary>
	int handSize();	

	//<summary>������ ���� ��ȯ(ġ��Ĳ ���� 14���� ����)</summary>
	int handSizeIncludeCall();

	//<summary>���� ������ ��ȯ</summary>
	vector<Tile> getHand();

	//<summary>�ȱ� ������ ����Ʈ ��ȯ</summary>
	vector<Tile> anKanAble();

	//<summary>�ȱ� ����</summary>
	void anKan(Tile target);

	//<summary>�ȱ� ����(������ �Ȼ���) �׽�Ʈ��</summary>
	void anKanNodraw(Tile target);

	bool loopTenpaiFinder(bool tip = false ,int maxdepth = 3);
};