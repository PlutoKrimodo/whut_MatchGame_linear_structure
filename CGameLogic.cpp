#include "pch.h"
#include "CGameLogic.h"
#include"global.h"
CGameLogic::CGameLogic(void)
{
	m_nVexNum = 0; // 数组中实际元素的个数
	m_nCorner = 0; // 路径中的拐点数
}

CGameLogic::~CGameLogic() {
	for (int i = 0; i < rows; i++) {
		delete[]GameMap[i];
	}
	delete[]GameMap;
}

int CGameLogic::GetElement(int nRow, int nCol) {
	return GameMap[nRow][nCol];
}

void CGameLogic::InitMap(int nRows, int nCols, int nPicNum) {
	this->rows = nRows;
	this->cols = nCols;
	this->picNum = nPicNum;

	this->GameMap = new int* [nRows];
	for (int i = 0; i < nRows; i++) {
		GameMap[i] = new int[nCols];
		memset(GameMap[i], BLANK, sizeof(int) * nCols);
	}

	int nRepeatNum = (nRows * nCols) / nPicNum;
	int nCount = 0;
	for (int i = 0; i < nPicNum; i++) {
		for (int j = 0; j < nRepeatNum; j++) {
			GameMap[nCount / nCols][nCount % nCols] = i;
			nCount++;
		}
	}

	srand((int)time(NULL));
	int nVertexNum = nRows * nCols;
	for (int i = 0; i < nVertexNum; i++) {
		int nIndex1 = rand() % nVertexNum;
		int nIndex2 = rand() % nVertexNum;

		int nTmp = GameMap[nIndex1 / nCols][nIndex1 % nCols];
		GameMap[nIndex1 / nCols][nIndex1 % nCols] = GameMap[nIndex2 / nCols][nIndex2 % nCols];
		GameMap[nIndex2 / nCols][nIndex2 % nCols] = nTmp;
	}
}

//是否连通
bool CGameLogic::IsLink(int** pGameMap, Vertex v1, Vertex v2) {
	ClearStack();
	//同一个点
	if (v1 == v2) {
		return false;
	}
	//为空
	if (pGameMap[v1.row][v1.col] == BLANK || pGameMap[v2.row][v2.col] == BLANK) {
		return false;
	}

	//不拐弯
	if (LinkInRow(pGameMap, v1, v2)) {
		PushVertex(v1);
		PushVertex(v2);
		return true;
	}
	if (LinkInCol(pGameMap, v1, v2)) {
		PushVertex(v1);
		PushVertex(v2);
		return true;
	}

	//拐一次
	if (OneCornerLink(pGameMap, v1, v2)) {
		return true;
	}

	//拐两次
	if (TwoCornerLink(pGameMap, v1, v2)) {
		return true;
	}

	ClearStack();
	return false;
}

// 检查同一行两点之间是否为空
bool CGameLogic::LineX(int** pGameMap, int nRow, int nCol1, int nCol2)
{
	int minCol = min(nCol1, nCol2);
	int maxCol = max(nCol1, nCol2);
	// 遍历所有列，检查是否全为空
	for (int col = minCol + 1; col < maxCol; ++col)
	{
		if (pGameMap[nRow][col] != BLANK)
			return false;
	}
	return true;
}
// 检查同一列两点之间是否为空
bool CGameLogic::LineY(int** pGameMap, int nRow1, int nRow2, int nCol)
{
	int minRow = min(nRow1, nRow2);
	int maxRow = max(nRow1, nRow2);
	// 遍历所有行，检查是否全为空
	for (int row = minRow + 1; row < maxRow; ++row)
	{
		if (pGameMap[row][nCol] != BLANK)
			return false;
	}
	return true;
}

//横，纵是否连通
bool CGameLogic::LinkInRow(int** pGameMap, Vertex v1, Vertex v2) {
	if (v1.row != v2.row) {
		return false;
	}
	return LineX(pGameMap, v1.row, v1.col, v2.col);
}
bool CGameLogic::LinkInCol(int** pGameMap, Vertex v1, Vertex v2) {
	if (v1.col != v2.col) return false;
	return LineY(pGameMap, v1.row, v2.row, v1.col);
}

//只折一次
bool CGameLogic::OneCornerLink(int** pGameMap, Vertex v1, Vertex v2) {
	//拐点1，先横后竖
	Vertex corner1(v1.row, v2.col);
	if(pGameMap[corner1.row][corner1.col]==BLANK&&
		LineX(pGameMap, v1.row, v1.col, corner1.col) &&
			LineY(pGameMap, corner1.row, v2.row, corner1.col)) {
		ClearStack();
		PushVertex(v1);
		PushVertex(corner1);
		PushVertex(v2);
		return true;
	}

	// 拐点2，先竖后横
	Vertex corner2(v2.row, v1.col);
	if (pGameMap[corner2.row][corner2.col] == BLANK &&
		LineY(pGameMap, v1.row, corner2.row, v1.col) &&
		LineX(pGameMap, corner2.row, corner2.col, v2.col))
	{
		ClearStack();
		PushVertex(v1);
		PushVertex(corner2);
		PushVertex(v2);
		return true;
	}

	return false;
}

//折两次
bool CGameLogic::TwoCornerLink(int** pGameMap, Vertex v1, Vertex v2) {
	//沿行扫描，找中间列作为公共拐点
	for (int col = 0; col < cols; ++col)
	{
		// 拐点1：(V1.row, col)，拐点2：(V2.row, col)
		Vertex mid1(v1.row, col);
		Vertex mid2(v2.row, col);
		// 两个拐点必须为空，且不是起点或者终点
		if (pGameMap[mid1.row][mid1.col] != BLANK || pGameMap[mid2.row][mid2.col] != BLANK)
			continue;
		if (mid1 == v1 || mid1 == v2 || mid2 == v1 || mid2 == v2)
			continue;

		// 检查三段路径：V1→mid1（横）、mid1→mid2（竖）、mid2→V2（横）
		if (LineX(pGameMap, v1.row, v1.col, mid1.col) &&
			LineY(pGameMap, mid1.row, mid2.row, mid1.col) &&
			LineX(pGameMap, v2.row, mid2.col, v2.col))
		{
			ClearStack();
			PushVertex(v1);
			PushVertex(mid1);
			PushVertex(mid2);
			PushVertex(v2);
			return true;
		}
	}

	//沿列扫描，找中间行作为公共拐点
	for (int row = 0; row < rows; ++row)
	{
		// 拐点1：(row, V1.col)，拐点2：(row, V2.col)
		Vertex mid1(row, v1.col);
		Vertex mid2(row, v2.col);
		if (pGameMap[mid1.row][mid1.col] != BLANK|| pGameMap[mid2.row][mid2.col] != BLANK)
			continue;
		if (mid1 == v1 || mid1 == v2 || mid2 == v1 || mid2 == v2)
			continue;

		// 检查三段路径：V1→mid1（竖）、mid1→mid2（横）、mid2→V2（竖）
		if (LineY(pGameMap, v1.row, mid1.row, v1.col) &&
			LineX(pGameMap, mid1.row, mid1.col, mid2.col) &&
			LineY(pGameMap, mid2.row, v2.row, v2.col))
		{
			ClearStack();
			PushVertex(v1);
			PushVertex(mid1);
			PushVertex(mid2);
			PushVertex(v2);
			return true;
		}
	}

	return false;
}

// 添加一个路径顶点
void CGameLogic::PushVertex(Vertex nV) {
	//顶点入栈
	if (m_nVexNum < 4) {
		m_avPath[m_nVexNum] = nV;
		m_nVexNum++;
	}
	//新的拐点
	if (IsCorner()) {
		m_nCorner++;
	}
}

// 取出一个路径顶点
void CGameLogic::PopVertex()
{
	// 判断是否形成拐点，如果存在拐点，在取出之前减少拐点数
	if (IsCorner())
	{
		m_nCorner--;
	}
	if (m_nVexNum > 0) {
		m_nVexNum--;
	}
}

//判断拐点是否有效
bool CGameLogic::IsCorner() {
	if (m_nVexNum >= 3) {
		Vertex& v1 = m_avPath[m_nVexNum - 3];
		Vertex& v2 = m_avPath[m_nVexNum - 2];
		Vertex& v3 = m_avPath[m_nVexNum - 1];
		if ((v1.row == v2.row && v2.row == v3.row) ||
			(v1.col == v2.col && v2.col == v3.col)) {
			return false;
		}
		else {
			return true;
		}
	}
	return false;
}

//清除栈内元素
void CGameLogic::ClearStack() {
	m_nVexNum = 0;
}

//销子
void CGameLogic::Clear(int** pGameMap, Vertex v1, Vertex v2) {
	pGameMap[v1.row][v1.col] = BLANK;
	pGameMap[v2.row][v2.col] = BLANK;
	ClearStack();
}

//获取路径
int CGameLogic::GetVexPath(Vertex avPath[MAX_VERTEX_NUM]) {
	for (int i = 0; i < m_nVexNum; ++i) {
		avPath[i] = m_avPath[i];
	}
	return m_nVexNum;
}

bool CGameLogic::IsBlank(int** pGameMap) {
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			if (pGameMap[i][j] != BLANK) {
				return false;
			}
		}
	}
	return true;
}