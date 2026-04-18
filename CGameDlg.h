#pragma once
#include "afxdialogex.h"
#include"CGameControl.h"
#include"CGameLogic.h"
#include"global.h"
// CGameDlg 对话框
class CGameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameDlg)
private:
	int GameMode;
public:
	CGameDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGameDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	HICON m_hIcon;
	//内存DC
	CDC m_dcMem;
	//背景
	CDC m_dcBG;
	//元素
	CDC m_dcElement;
	//掩码
	CDC m_dcMask;

	CPoint m_ptGameTop; // 游戏起始坐标
	CSize m_sizeElem;	// 图片元素大小
	CRect m_rtGameRect; // 游戏区域大小

	bool m_bFirstPoint; // 选中的图片是不是第一次选中，默认为是
	bool m_bPlaying;	// 表示游戏是否正在进行
	bool m_bPause;		// 游戏暂停与否
	int timeCount;		// 计时

	//背景路径
	static CString BGPath;
	//元素图片路径
	static CString ElementPath;
	//掩码图片路径
	static CString MaskPath;
	//游戏控制类
	CGameControl m_GameC;
	DECLARE_MESSAGE_MAP()
public:
	void SetGameMode(int mode);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	void InitBackGround();
	void InitMode(int mode);
	//void SetButton(bool start, bool stop, bool reset, bool prompt);
	void InitElement(CString ElementPath, CString MaskPath);
	void UpdateMap();
	void DrawTipFrame(int nRow, int nCol);		//绘制提示框
	void DrawTipLine(Vertex asvPath[MAX_VERTEX_NUM], int nVexnum); // 画提示线
	void JudgeWin(void);										   // 判断胜负

	CProgressCtrl m_GameProgress;
public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

