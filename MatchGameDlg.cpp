#include "pch.h"
#include "framework.h"
#include "MatchGame.h"
#include "MatchGameDlg.h"
#include "CGameDlg.h"
#include "afxdialogex.h"
#include"global.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMatchGameDlg 对话框



CMatchGameDlg::CMatchGameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MATCHGAME_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMatchGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMatchGameDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_BASIC, &CMatchGameDlg::OnBnClickedBtnBasic)
	ON_BN_CLICKED(IDC_BTN_RELEX, &CMatchGameDlg::OnBnClickedBtnRelex)
	ON_BN_CLICKED(IDC_BTN_LEVEL, &CMatchGameDlg::OnBnClickedBtnLevel)
	ON_BN_CLICKED(IDC_BTN_SETTING, &CMatchGameDlg::OnBnClickedBtnSetting)
	ON_BN_CLICKED(IDC_BTN_RANK, &CMatchGameDlg::OnBnClickedBtnRank)
	ON_BN_CLICKED(IDC_BTN_HELP, &CMatchGameDlg::OnBnClickedBtnHelp)
END_MESSAGE_MAP()


// CMatchGameDlg 消息处理程序

BOOL CMatchGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitBackground();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMatchGameDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMatchGameDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CRect rect;
		GetClientRect(&rect);

		// 永久绘制背景图
		dc.StretchBlt(
			rect.left, rect.top, rect.Width(), rect.Height(),
			&m_dcMem,
			0, 0, m_bmpWidth, m_bmpHeight,
			SRCCOPY
		);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMatchGameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMatchGameDlg::InitBackground() {
	// 加载位图
	CBitmap bmpMain;
	bmpMain.LoadBitmapW(IDB_MAIN);

	BITMAP bmpInfo;
	bmpMain.GetBitmap(&bmpInfo);

	// 把宽高存到成员变量，给 OnPaint 用
	m_bmpWidth = bmpInfo.bmWidth;
	m_bmpHeight = bmpInfo.bmHeight;

	// 创建内存DC（只创建一次，长期保存）
	CClientDC dc(this);
	m_dcMem.DeleteDC();
	m_dcMem.CreateCompatibleDC(&dc);

	// 把位图贴到内存DC
	static CBitmap bmpMem; // 静态 = 程序不退出就不销毁
	bmpMem.DeleteObject();
	bmpMem.Attach(bmpMain.Detach());
	m_dcMem.SelectObject(&bmpMem);

	// 调整窗口大小
	CRect rtClient, rtWin;
	GetClientRect(&rtClient);
	GetWindowRect(&rtWin);
	int frameWidth = rtWin.Width() - rtClient.Width();
	int frameHeight = rtWin.Height() - rtClient.Height();
	MoveWindow(0, 0, m_bmpWidth + frameWidth, m_bmpHeight + frameHeight, TRUE);
	CenterWindow();
}

void CMatchGameDlg::OnBnClickedBtnBasic()
{
	this->ShowWindow(SW_HIDE);

	CGameDlg dlg;
	dlg.SetGameMode(1);
	dlg.DoModal();

	this->ShowWindow(SW_SHOW);
}

void CMatchGameDlg::OnBnClickedBtnRelex()
{
	this->ShowWindow(SW_HIDE);

	CGameDlg dlg;
	dlg.SetGameMode(0);
	dlg.DoModal();

	this->ShowWindow(SW_SHOW);
}

void CMatchGameDlg::OnBnClickedBtnLevel()
{
	this->ShowWindow(SW_HIDE);

	CGameDlg dlg;
	//0为休闲，1为基本，其余数为闯关，3是随机设的
	dlg.SetGameMode(3);
	dlg.DoModal();

	this->ShowWindow(SW_SHOW);
}

void CMatchGameDlg::OnBnClickedBtnRank()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CMatchGameDlg::OnBnClickedBtnSetting()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMatchGameDlg::OnBnClickedBtnHelp()
{
	// TODO: 在此添加控件通知处理程序代码
}
