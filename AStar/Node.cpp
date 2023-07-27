#include "Node.h"

Node::Node(int i, int j)
{
	val_G = val_H = val_F = 0;

	SetCenter(i, j);
	Idx = POINT{ i,j };
	SetRect();
	SetValPos();

	stat = INIT;
	parent = nullptr;
}

Node::~Node()
{
}

void Node::SetCenter(int i, int j)
{
	center = POINT{ startPos.x + 2 * j * radius, startPos.y + 2 * i * radius };
}

void Node::SetRect()
{
	rect.left = center.x - radius+1;
	rect.right = center.x + radius-1;
	rect.top = center.y - radius+1;
	rect.bottom = center.y + radius-1;
}

void Node::SetValPos()
{
	GValPos = POINT{ center.x - 17, center.y + 10 };
	HValPos = POINT{ center.x + 10, center.y + 10 };
	FValPos = POINT{ center.x-4, center.y - 25 };
}

void Node::SetStartPoint()
{
	stat = STARTPOINT;
}

void Node::SetEndPoint()
{
	stat = ENDPOINT;
}

void Node::SetObstacle()
{
	stat = OBSTACLE;
}

BOOL Node::IsInRect(const POINT& pt)
{
	return (pt.x<(center.x + radius) && pt.x>(center.x - radius) && pt.y<(center.y + radius) && pt.y>(center.y - radius));
}

void Node::Draw(HDC hdc)
{
	switch (stat)
	{
	case STARTPOINT:
		SelectObject(hdc, CreateSolidBrush(RGB(0, 255, 0)));
		break;
	case ENDPOINT:
		SelectObject(hdc, CreateSolidBrush(RGB(255, 0, 0)));
		break;
	case OBSTACLE:
		SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 0)));
		break;
	case NOWINOPEN:
		SelectObject(hdc, CreateSolidBrush(RGB(128, 128, 0)));
		break;
	case NOWINCLOSED:
		SelectObject(hdc, CreateSolidBrush(RGB(0, 128, 128)));

		break;
	default:
		SelectObject(hdc, CreateSolidBrush(RGB(255, 255, 255)));
	}

	
	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	
	TCHAR str[128];

	if (stat == STARTPOINT || stat == OBSTACLE)return;
	wsprintf(str, TEXT("%d"), val_G);
	TextOut(hdc, GValPos.x, GValPos.y, str, lstrlen(str));
	wsprintf(str, TEXT("%d"), val_H);
	TextOut(hdc, HValPos.x, HValPos.y, str, lstrlen(str));
	wsprintf(str, TEXT("%d"), val_F);
	TextOut(hdc, FValPos.x, FValPos.y, str, lstrlen(str));
}
