// CGameDlg.cpp: 实现文件
//

#include "pch.h"
#include "MatchGame.h"
#include "afxdialogex.h"
#include "CGameDlg.h"
#include"global.h"
//资源路径初始化
CString CGameDlg::BGPath = _T("theme/picture/cxk_bg.bmp");
CString CGameDlg::ElementPath = _T("theme/picture/cxk_element.bmp");
CString CGameDlg::MaskPath = _T("theme/picture/cxk_mask.bmp");
// CGameDlg 对话框
IMPLEMENT_DYNAMIC(CGameDlg, CDialogEx)

CGameDlg::CGameDlg(CWnd* pParent /*=nullptr*/): CDialogEx(IDD_GAME_DIALOG, pParent)
{
	// 初始化起始点坐标
	m_ptGameTop.x = MAP_LEFT;
	m_ptGameTop.y = MAP_TOP;

	// 初始化图片元素大小
	m_sizeElem.cx = PIC_WIDTH;
	m_sizeElem.cy = PIC_HEIGHT;

	// 初始化图标选中状态
	m_bFirstPoint = true;
	m_bPlaying = false;

	// 初始化游戏更新区域
	m_rtGameRect.top = m_ptGameTop.y;
	m_rtGameRect.left = m_ptGameTop.x;
	m_rtGameRect.right = m_rtGameRect.left + m_sizeElem.cx * MAX_COL;
	m_rtGameRect.bottom = m_rtGameRect.top + m_sizeElem.cy * MAX_ROW;
}

CGameDlg::~CGameDlg()
{
	m_dcMem.DeleteDC();
	m_dcBG.DeleteDC();
	m_dcElement.DeleteDC();
	m_dcMask.DeleteDC();
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGameDlg, CDialogEx)
//	ON_WM_PAINT()
ON_WM_PAINT()
ON_BN_CLICKED(IDC_BTN_START, &CGameDlg::OnBnClickedBtnStart)
ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CGameDlg 消息处理程序

void CGameDlg::InitBackGround() {
	HANDLE Backbmp = ::LoadImageW(NULL, BGPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	CClientDC dc(this);
	m_dcBG.CreateCompatibleDC(&dc);
	m_dcBG.SelectObject(Backbmp);

	m_dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, 800, 600);
	m_dcMem.SelectObject(&bmpMem);

	m_dcMem.BitBlt(0, 0, 800, 600, &m_dcBG, 0, 0, SRCCOPY);

	CRect rtWin;
	CRect rtClient;
	this->GetWindowRect(rtWin);
	this->GetClientRect(rtClient);
	int nSpanWidth = rtWin.Width() - rtClient.Width();
	int nSpanHeight = rtWin.Height() - rtClient.Height();

	MoveWindow(0, 0, 800 + nSpanWidth, 600 + nSpanHeight);
	CenterWindow();
}

BOOL CGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	this->ModifyStyleEx(0, WS_EX_APPWINDOW);
	//设置背景
	InitBackGround();
	//设置模式
	InitMode(this->GameMode);
	//设置游戏窗口按钮初始状态
	
	//初始化加载对象元素和掩码
	InitElement(ElementPath, MaskPath);
	return TRUE;
	// 异常: OCX 属性页应返回 FALSE
}



void CGameDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	dc.BitBlt(0, 0, 800, 600, &m_dcMem, 0, 0, SRCCOPY);
}

void CGameDlg::SetGameMode(int mode) {
	this->GameMode = mode;
}

void CGameDlg::InitMode(int mode) {
	if (mode == 1) {
		this->SetWindowTextW(_T("MatchGame--Basic"));
	}
	else if(mode==0){
		this->SetWindowTextW(_T("MatchGame--Relex"));
	}
	else {
		this->SetWindowTextW(_T("MatchGame--Level"));
	}
}

void CGameDlg::InitElement(CString ElementPath, CString MaskPath) {
	CClientDC dc(this);
	HANDLE hBmp = ::LoadImageW(NULL, ElementPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_dcElement.CreateCompatibleDC(&dc);
	m_dcElement.SelectObject(hBmp);

	HANDLE hMask = ::LoadImageW(NULL, MaskPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_dcMask.CreateCompatibleDC(&dc);
	m_dcMask.SelectObject(hMask);
}

void CGameDlg::UpdateMap() {
	// 计算图片的顶点坐标与图片大小
	int nTop = m_ptGameTop.y;
	int nLeft = m_ptGameTop.x;
	int nElemW = m_sizeElem.cx;
	int nElemH = m_sizeElem.cy;

	m_dcMem.BitBlt(m_rtGameRect.left, m_rtGameRect.top, m_rtGameRect.Width(), m_rtGameRect.Height(), 
		&m_dcBG, m_rtGameRect.left, m_rtGameRect.top, SRCCOPY);

	for (int i = 0; i < MAX_ROW; i++)
	{
		for (int j = 0; j < MAX_COL; j++)
		{
			int nElemVal = m_GameC.GetElement(i, j);
			if (nElemVal == BLANK)
				continue;

			// 将背景与掩码相与，边保留，图像区域为1
			m_dcMem.BitBlt(nLeft + j * nElemW, nTop + i * nElemH, nElemH, nElemW, &m_dcMask, 0, nElemVal * nElemH, SRCPAINT);

			// 与元素图片相或，边保留，图片区域为元素图片
			m_dcMem.BitBlt(nLeft + j * nElemW, nTop + i * nElemH, nElemH, nElemW, &m_dcElement, 0, nElemVal * nElemH, SRCAND);
		}
	}
	Invalidate(false);
}
void CGameDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	bool status = m_GameC.StartGame(MAX_ROW, MAX_COL, MAX_PIC_NUM);
	if (status) {
		m_bPlaying = true;
		UpdateMap();
		//只允许点击一次
		this->GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	}
	else {
		AfxMessageBox(L"行列和图片个数不匹配，无法绘制地图！", MB_OKCANCEL);
	}
	InvalidateRect(FALSE);
}

void CGameDlg::OnLButtonUp(UINT nFlags, CPoint point)
{	
	if (!m_bPlaying) { 
		// 游戏未开始时忽略点击
		return CDialogEx::OnLButtonUp(nFlags, point);
	}

	if (point.x < m_ptGameTop.x || point.y < m_ptGameTop.y) {
		return CDialogEx::OnLButtonUp(nFlags, point);
	}

	// 换算点击的坐标点
	int nRow = (point.y - m_ptGameTop.y) / m_sizeElem.cy;
	int nCol = (point.x - m_ptGameTop.x) / m_sizeElem.cx;

	// 判断坐标的有效性
	if (nRow > MAX_ROW - 1 || nCol > MAX_COL - 1)
	{
		return CDialogEx::OnLButtonUp(nFlags, point);
	}

	// 跳过已清空的位置
	if (m_GameC.GetElement(nRow, nCol) == BLANK) {
		return CDialogEx::OnLButtonUp(nFlags, point);
	}

	//选中第一个点
	if (m_bFirstPoint) {
		DrawTipFrame(nRow, nCol);
		m_GameC.SetFirstPoint(nRow, nCol);
		//切换为第二个点
		m_bFirstPoint = false;
	}
	else {
		// 不能选中同一个点
		if (m_GameC.m_svSelFst.row == nRow && m_GameC.m_svSelFst.col == nCol) {
			return CDialogEx::OnLButtonUp(nFlags, point);
		}

		DrawTipFrame(nRow, nCol);
		m_GameC.SetSecPoint(nRow, nCol);

		Vertex avPath[MAX_VERTEX_NUM];
		int nVexnum=0;

		if (m_GameC.Link(avPath, nVexnum)) {
			//提示线
			DrawTipLine(avPath, nVexnum);
			UpdateMap();
			JudgeWin();
		}
		else {
			UpdateMap();
		}
		m_bFirstPoint = true;
		m_GameC.ResetSelect();
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}

//绘制提示框
void CGameDlg::DrawTipFrame(int nRow, int nCol) {
	CClientDC dc(this);
	CBrush brush(RGB(233, 43, 43));
	CRect rtTipFrame;

	rtTipFrame.left = m_ptGameTop.x + nCol * m_sizeElem.cx;
	rtTipFrame.top = m_ptGameTop.y + nRow * m_sizeElem.cy;
	rtTipFrame.right = rtTipFrame.left + m_sizeElem.cx;
	rtTipFrame.bottom = rtTipFrame.top + m_sizeElem.cy;

	dc.FrameRect(rtTipFrame, &brush);
}

//提示线
void CGameDlg::DrawTipLine(Vertex avPath[MAX_VERTEX_NUM], int nVexnum) {
	// 使用内存DC绘制
	CPen penLine(PS_SOLID, 2, RGB(255, 0, 0));
	CPen* pOldPen = m_dcMem.SelectObject(&penLine);

	// 计算第一个点的中心坐标
	int startX = m_ptGameTop.x + avPath[0].col * m_sizeElem.cx + m_sizeElem.cx / 2;
	int startY = m_ptGameTop.y + avPath[0].row * m_sizeElem.cy + m_sizeElem.cy / 2;
	m_dcMem.MoveTo(startX, startY);

	// 绘制路径线
	for (int i = 0; i < nVexnum - 1; i++) {
		int endX = m_ptGameTop.x + avPath[i + 1].col * m_sizeElem.cx + m_sizeElem.cx / 2;
		int endY = m_ptGameTop.y + avPath[i + 1].row * m_sizeElem.cy + m_sizeElem.cy / 2;
		m_dcMem.LineTo(endX, endY);
	}

	m_dcMem.SelectObject(pOldPen);
	// 强制刷新窗口
	InvalidateRect(m_rtGameRect, FALSE);
	UpdateWindow();
	// 延长显示时间
	Sleep(100);
}

void CGameDlg::JudgeWin() {
	// 游戏胜负判断
	bool bGameStatus = m_GameC.IsWin();

	// 判断是否继续游戏
	if (bGameStatus == GAME_PLAY)
	{
		return;
	}
	else
	{
		// 将游戏表示改为false
		m_bPlaying = false;

		// 提示获胜
		CString strTitle;

		this->GetWindowTextW(strTitle);

		if (bGameStatus == GAME_SUCCESS)
		{
			MessageBox(_T("You Win!"), strTitle);
		}
		else if (bGameStatus == GAME_LOSE)
		{
			MessageBox(_T("You Lose!\nGame Over!"), strTitle);
		}

		// 还原游戏按钮可点击
		this->GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	}
}