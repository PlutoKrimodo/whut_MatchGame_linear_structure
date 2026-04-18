
// MatchGameDlg.h: 头文件
//

#pragma once


// CMatchGameDlg 对话框
class CMatchGameDlg : public CDialogEx
{
// 构造
public:
	CMatchGameDlg(CWnd* pParent = nullptr);	// 标准构造函数
	int m_bmpWidth, m_bmpHeight;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MATCHGAME_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CDC m_dcMem;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void InitBackground();
public:
	afx_msg void OnBnClickedBtnBasic();
	afx_msg void OnBnClickedBtnRelex();
	afx_msg void OnBnClickedBtnLevel();
	afx_msg void OnBnClickedBtnSetting();
	afx_msg void OnBnClickedBtnRank();
	afx_msg void OnBnClickedBtnHelp();
};
