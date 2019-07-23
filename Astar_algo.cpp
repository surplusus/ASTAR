#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "Astar_algo.h"


Astar_algo::Astar_algo()
{
}


Astar_algo::~Astar_algo()
{
}

void Astar_algo::DrawGrid(HDC hdc)
{
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 204, 204));
	HBRUSH hEmptyBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hOldBrush;
	HPEN hPen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
	HPEN hOldPen;
	hOldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)hBrush);
	Rectangle(hdc, START_X, START_Y, START_X + GRID_WIDTH, START_Y + GRID_HEIGHT);

	SelectObject(hdc, (HBRUSH)hEmptyBrush);
	hOldPen = (HPEN)SelectObject(hdc, hPen);
	
	for (size_t i = 0; i < 10; i++)
	{
		for (size_t j = 0; j < 10; j++)
		{
			int x = GRID_START_X + j * RECT_SIZE;
			int y = GRID_START_Y + i * RECT_SIZE;
			Rectangle(hdc, x, y, x + RECT_SIZE, y + RECT_SIZE);
		}
	}
	DeleteObject(hBrush);
	DeleteObject(hEmptyBrush);
	DeleteObject(hPen);
}

void Astar_algo::DrawRect(HDC hdc)
{
	HBRUSH hS_posBrush = CreateSolidBrush(RGB(0, 179, 0));
	HBRUSH hD_posBrush = CreateSolidBrush(RGB(204, 51, 0));
	HBRUSH hGBrush = CreateSolidBrush(RGB(204, 255, 51));
	HBRUSH hBBrush = CreateSolidBrush(RGB(0, 153, 255));
	HBRUSH hOldBrush;
	
	hOldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)hS_posBrush);
	int xx = s_point.x * RECT_SIZE + GRID_START_X;
	int yy = s_point.y * RECT_SIZE + GRID_START_Y;
	RECT re = MakeRect(xx, yy);
	FillRect(hdc, &re, hS_posBrush);
	TCHAR buf[8];
	RECT r = re;
	r.left += 5;	r.top += 3;
	_stprintf_s(buf, _T("S"));
	DrawText(hdc, buf, 8, &r, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	
	hOldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)hD_posBrush);
	xx = s_point.x * RECT_SIZE + GRID_START_X;
	yy = s_point.y * RECT_SIZE + GRID_START_Y;
	re = MakeRect(xx, yy);
	FillRect(hdc, &re, hD_posBrush);
	r = re;
	r.left += 5;	r.top += 3;
	_stprintf_s(buf, _T("D"));
	DrawText(hdc, buf, 8, &r, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

	hOldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)hGBrush);
	for (auto found : open_list)
	{
		int x = found->coord.x * RECT_SIZE + GRID_START_X;
		int y = found->coord.y * RECT_SIZE + GRID_START_Y;
		Rectangle(hdc, x, y, x+RECT_SIZE, y+RECT_SIZE);
		
		_stprintf_s(buf, _T("%d"),found->val_G);
		TextOut(hdc, x, y, buf, 4);
		_stprintf_s(buf, _T("%d"), found->val_H);
		TextOut(hdc, x+30, y, buf, 4);
		_stprintf_s(buf, _T("%d"), found->val_sum);
		TextOut(hdc, x+15, y+15, buf, 4);
	}

	SelectObject(hdc, (HBRUSH)hBBrush);
	for (auto path : close_list)
	{
		int x = path->coord.x * RECT_SIZE + GRID_START_X;
		int y = path->coord.y * RECT_SIZE + GRID_START_Y;
		Rectangle(hdc, x, y, x + RECT_SIZE, y + RECT_SIZE);

		_stprintf_s(buf, _T("%d"), path->val_G);
		TextOut(hdc, x, y, buf, 4);
		_stprintf_s(buf, _T("%d"), path->val_H);
		TextOut(hdc, x + 30, y, buf, 4);
		_stprintf_s(buf, _T("%d"), path->val_sum);
		TextOut(hdc, x + 15, y + 15, buf, 4);
	}
	
	SelectObject(hdc, (HBRUSH)hOldBrush);
	DeleteObject(hGBrush);
	DeleteObject(hBBrush);
}

void Astar_algo::Update()
{
	if (mousePos_.x != d_point.x || mousePos_.y != d_point.y)
	{
		d_point = PosToCoordinate(mousePos_);
	}

	while (true)
	{
		if (close_list.size() == 0)
		{
			VALUES* v = &map[s_point.y][s_point.x];
			close_list.emplace_back(v);
		}
		KeepInOpenList(close_list.back());
		KeepInCloseList();

		if (d_point.x == close_list.back()->coord.x &&
			d_point.y == close_list.back()->coord.y)
			break;
	}
}

void Astar_algo::Init()
{
	map.resize(10);
	for (size_t i = 0; i < 10; i++)
	{
		for (size_t j = 0; j < 10; j++)
		{
			map[i].emplace_back(VALUES(j,i));
		}
	}
	s_point = { 1,1 };
}

void Astar_algo::KeepInOpenList(VALUES* close_pos)
{
	int dx[8] = { -1,0,1,1,1,0,-1,-1 };
	int dy[8] = { 1,1,1,0,-1,-1,-1,0 };
	for (size_t i = 0; i < 8; i++)
	{
		if (close_pos->coord.x + dx[i] < 0 || close_pos->coord.x + dx[i] < 0 ||
			close_pos->coord.y + dy[i] >= 10 || close_pos->coord.y + dy[i] >= 10)
			continue;

		VALUES* v = &map[close_pos->coord.y+ dy[i]][close_pos->coord.x + dx[i]];
		// G value
		if (dx[i] == 0 || dy[i] == 0)
			v->val_G = close_pos->val_G + 10;
		else
			v->val_G = close_pos->val_G + 14;
		// H value
		VALUES* basis = &map[close_pos->coord.y][close_pos->coord.x];
		int hx = static_cast<int>(abs(basis->coord.x - d_point.x));
		int hy = static_cast<int>(abs(basis->coord.y - d_point.y));
		if (hx <= hy) {
			hx *= 10;
			hy *= 4;
		}
		else {
			hy *= 10;
			hy *= 4;
		}
		v->val_H = hy + hx;
		// sum value
		v->val_sum = v->val_G + v->val_H;

		close_list.emplace_back(v);
	}
}

void Astar_algo::KeepInCloseList()
{
	priority_queue<pair<int, VALUES*>> check_list;
	for (auto list : open_list)
	{
		pair<int, VALUES*> tmp;
		tmp.first = list->val_sum;
		tmp.second = list;
		check_list.push(tmp);
	}
	if (check_list.size() != 0)
		close_list.emplace_back(check_list.top().second);

}

POINT Astar_algo::PosToCoordinate(POINT pos)
{
	pos.x = (pos.x - GRID_START_X) / RECT_SIZE;
	pos.y = (pos.y - GRID_START_Y) / RECT_SIZE;
	
	return pos;
}

RECT Astar_algo::MakeRect(int coordX, int coordY)
{
	RECT re;
	re.left = coordX;
	re.top = coordY;
	re.right = coordX + RECT_SIZE;
	re.bottom = coordY + RECT_SIZE;
	return re;
}

