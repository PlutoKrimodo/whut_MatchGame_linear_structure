#include "pch.h"
#include "CGameControl.h"
#include"global.h"
int CGameControl::GetElement(int nRow, int nCol) {
	return m_GameLogic.GetElement(nRow, nCol);
}

bool CGameControl::StartGame(int rows, int cols, int picNum) {
	if ((rows * cols) % (picNum * 2) != 0) {
		return false;
	}
	else {
		m_GameLogic.InitMap(rows, cols, picNum);
	}
	return true;
}

void CGameControl::ResetSelect() {
	m_svSelFst.row = m_svSelFst.col = -1;
	m_svSelSec.row = m_svSelSec.col = -1;
}

//设置第一个点
void CGameControl::SetFirstPoint(int nRow, int nCol){
	m_svSelFst.row = nRow;
	m_svSelFst.col = nCol;
}

// 设置第二个点
void CGameControl::SetSecPoint(int nRow, int nCol)
{
	m_svSelSec.row = nRow;
	m_svSelSec.col = nCol;
}

//连接销子
bool CGameControl::Link(Vertex avPath[MAX_VERTEX_NUM], int& nVexnum) {
	if (m_svSelFst.row == -1 || m_svSelSec.row == -1) {
		nVexnum = 0;
		return false;
	}

	if (m_GameLogic.GameMap[m_svSelFst.row][m_svSelFst.col] !=
		m_GameLogic.GameMap[m_svSelSec.row][m_svSelSec.col]) {
		nVexnum = 0;
		ResetSelect();
		return false;
	}
	//成功
	if (m_GameLogic.IsLink(m_GameLogic.GameMap, m_svSelFst, m_svSelSec)) {
		nVexnum = m_GameLogic.GetVexPath(avPath);
		m_GameLogic.Clear(m_GameLogic.GameMap, m_svSelFst, m_svSelSec);
		ResetSelect();
		return true;
	}

	ResetSelect();
	nVexnum = 0;
	return false;
}

bool CGameControl::IsWin() {
	if (m_GameLogic.IsBlank(m_GameLogic.GameMap)) {
		//m_GameLogic.InitMap(MAX_ROW, MAX_COL, MAX_PIC_NUM);
		return GAME_SUCCESS;
	}
	return GAME_PLAY;
}
