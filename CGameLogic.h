#pragma once
#include<stack>
#include"global.h"
using namespace std;
class CGameLogic
{
public:
	int** GameMap;
	int rows;
	int cols;
	int picNum;

	Vertex m_avPath[MAX_VERTEX_NUM];	// 保存在进行连接判断时所经过的顶点
	int m_anPath[MAX_VERTEX_NUM];		// 保存连通路径的顶点索引
	int m_nVexNum=0;						//顶点数
	int m_nCorner=0;						// 连接路径数组m_anPath中的拐点数
private:
	stack<Vertex> verList;
public:
	CGameLogic();
	~CGameLogic();
	int GetElement(int nRow, int nCol);
	void InitMap(int nRows, int Cols, int PicNum);
	bool IsLink(int **pGameMap, Vertex v1, Vertex v2);  // 判断顶点v1与v2是否连通
	void Clear(int** pGameMap, Vertex v1, Vertex v2);
	int GetVexPath(Vertex avPath[MAX_VERTEX_NUM]);
	
	//横，纵向向连通
	bool LinkInRow(int** pGameMap, Vertex v1, Vertex v2);
	bool LinkInCol(int** pGameMap, Vertex v1, Vertex v2);
	//一个拐点连通判断
	bool OneCornerLink(int** pGameMap, Vertex v1, Vertex v2);
	//直线连通X Y
	bool LineX(int** pGameMap, int nRow1, int nRow2, int nCol);
	bool LineY(int** pGameMap, int nRow1, int nRow2, int nCol);
	
	//添加路径节点
	void PushVertex(Vertex nV);
	//取出路径节点
	void PopVertex();
	void ClearStack();
	//三条直线消子判断
	bool TwoCornerLink(int** pGameMap, Vertex v1, Vertex v2);
	bool IsCorner();			//判断拐点是否有效
	//判断地图中是否清空
	bool IsBlank(int** pGameMap);
};

