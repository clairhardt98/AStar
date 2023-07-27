#pragma once
#include "framework.h"
#include <vector>
#include <string>

enum Stat { INIT, STARTPOINT, ENDPOINT, NOWINOPEN, NOWINCLOSED, OBSTACLE };

class Node
{
private:
	
	//int stat;//0 : 초기상태, 1 : 시작점, 2 : 끝점, 3 : 인접 노드 상태
	Stat stat;
	int val_G;
	int val_H;
	int val_F;
	Node* parent;
private:
	const POINT startPos = { 50,50 };
	POINT center;
	POINT GValPos;
	POINT HValPos;
	POINT FValPos;
	POINT Idx;
	RECT rect;
	const LONG radius = 30;
	COLORREF color;

public:
	Node(int i, int j);
	~Node();

public:
	void SetCenter(int i, int j);
	void SetRect();
	void SetValPos();
	void SetStartPoint();
	void SetEndPoint();
	void SetObstacle();
	void SetStat(Stat st) { stat = st; }
	void SetG(int g) { val_G = g; }
	void SetH(int h) { val_H = h; }
	void SetF() { val_F = val_G + val_H; }
public:
	int GetG()const { return val_G; }
	int GetH()const { return val_H; }
	int GetF()const { return val_F; }
	POINT GetCenter() { return center; }
	Stat Getstat() const { return stat; }
	POINT GetIdx() { return Idx; }
public:
	BOOL IsInRect(const POINT& pt);

public:
	void Draw(HDC hdc);

public:
	bool operator<(const Node _node) { return val_F < _node.GetF(); }
	bool operator>(const Node _node) { return val_F > _node.GetF(); }
	void operator=(const Node _node) {}

};