#pragma once

typedef struct _tagVALUES
{
	int val_G = 0;
	int val_H = 0;
	int val_sum = 0;
	POINT coord = { 0,0 };
	_tagVALUES() {}
	_tagVALUES(int x, int y) { coord.x = x; coord.y = y; }
} VALUES;

class Astar_algo
{
public:
	Astar_algo();
	~Astar_algo();
private:
	POINT s_point;
	POINT d_point = { -1,-1 };
	std::vector<VALUES*> open_list;
	std::vector<VALUES*> close_list;
	std::vector<std::vector<VALUES>> map;
	POINT mousePos_;
public:
	void DrawGrid(HDC hdc);
	void SetMousePos(POINT pos) { mousePos_ = pos; }
	void DrawRect(HDC hdc);
	void Update();
	void Init();
	void KeepInOpenList(VALUES* close_pos);
	void KeepInCloseList();
	POINT PosToCoordinate(POINT pos);
	RECT MakeRect(int coordX, int coordY);
};

