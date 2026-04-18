#pragma once
#include"CGameLogic.h"
#include"global.h"
class CGameControl
{
public:
	CGameLogic m_GameLogic;
	Vertex m_svSelFst; // 选中第一点
	Vertex m_svSelSec; // 选中第二点
public:
	//CGameControl();

	int GetElement(int nRow, int nCol);
	bool StartGame(int rows, int cols, int PicNum);
	//是否胜利
	bool IsWin();
	//重置
	void ResetSelect();
	void SetFirstPoint(int nRow, int nCol);					// 设置第一点
	void SetSecPoint(int nRow, int nCol);					// 设置第二点
	bool Link(Vertex avPath[MAX_VERTEX_NUM], int& nVexnum);
};

