
// DesktopTextDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopText.h"
#include "DesktopTextDlg.h"
#include "afxdialogex.h"

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


// CDesktopTextDlg 对话框



CDesktopTextDlg::CDesktopTextDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DESKTOPTEXT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(&notifyicondata_id_, 0, sizeof(notifyicondata_id_)); // Initialize NOTIFYICONDATA struct
	notifyicondata_id_.cbSize = sizeof(notifyicondata_id_);
	notifyicondata_id_.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
}

CDesktopTextDlg::~CDesktopTextDlg()
{
	notifyicondata_id_.hIcon = NULL;
	Shell_NotifyIcon(NIM_DELETE, &notifyicondata_id_);
}

void CDesktopTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TEXT, main_text_);
	DDX_Control(pDX, IDC_COMBO_TEXT_SIze, text_size_);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON_TEXT_COLOR, text_color_);
	DDX_Control(pDX, IDC_STATIC_ALPHA, alpha_text_);
	DDX_Control(pDX, IDC_SLIDER_ALPHA, alpha_silder_);
	DDX_Control(pDX, IDC_STATIC_SIZE, text_size_label_);
	DDX_Control(pDX, IDC_SLIDER_SIZE, text_size_slider_);
}

BEGIN_MESSAGE_MAP(CDesktopTextDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MFCBUTTON_UPDATE, &CDesktopTextDlg::OnBnClickedMfcbuttonUpdate)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ALPHA, &CDesktopTextDlg::OnNMCustomdrawSliderAlpha)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SIZE, &CDesktopTextDlg::OnNMCustomdrawSliderSize)
	ON_MESSAGE(UM_TRAYNOTIFY, &CDesktopTextDlg::OnTrayNotify)
END_MESSAGE_MAP()


// CDesktopTextDlg 消息处理程序

BOOL CDesktopTextDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 文字窗口初始化
	CString text;
	BYTE alpha_percent(0), r(0), g(0), b(0);
	float size_ratio;
	int x(0), y(0);
	config_.LoadConfig(text, alpha_percent, size_ratio, r, g, b, x, y);
	text_color_.SetColor(RGB(r, g, b));
	RECT rec_work;
	SystemParametersInfo(SPI_GETWORKAREA, NULL, &rec_work, NULL);
	int max = min(rec_work.right - rec_work.left, rec_work.bottom - rec_work.top) * 0.9;
	text_size_slider_.SetRange(0, max);
	text_size_slider_.SetPos(max * size_ratio);
	alpha_silder_.SetRange(0, 100);
	alpha_silder_.SetPos(alpha_percent);
	main_text_.SetWindowText(text);
	text_window_.Create(x, y);

	// 托盘
	notifyicondata_id_.hWnd = GetSafeHwnd();
	notifyicondata_id_.uCallbackMessage = UM_TRAYNOTIFY;
	// Set tray icon and tooltip
	notifyicondata_id_.hIcon = m_hIcon;
	// Set tray notification tip information
	CString strToolTip = _T("托盘程序");
	_tcsncpy_s(notifyicondata_id_.szTip, strToolTip, strToolTip.GetLength());
	Shell_NotifyIcon(NIM_ADD, &notifyicondata_id_);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDesktopTextDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDesktopTextDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDesktopTextDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDesktopTextDlg::OnBnClickedMfcbuttonUpdate()
{
	// TODO: 在此添加控件通知处理程序代码
	COLORREF color =  text_color_.GetColor();
	uint16_t font_size = text_size_slider_.GetPos();
	CString text;
	main_text_.GetWindowText(text);
	uint8_t alpha_pos = alpha_silder_.GetPos();
	text_window_.SetTextColor(color);
	text_window_.SetTextSize(font_size);
	text_window_.SetText(text);
	text_window_.SetAlphaPercent(alpha_pos);
	text_window_.Draw();
	RECT rc;
	text_window_.GetWindowRect(&rc);
	config_.SaveConfig(text, alpha_pos, (float)text_size_slider_.GetPos() / text_size_slider_.GetRangeMax(), GetRValue(color), GetGValue(color), GetBValue(color), rc.left, rc.top);
}


void CDesktopTextDlg::OnNMCustomdrawSliderAlpha(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	uint8_t pos = alpha_silder_.GetPos();
	CString pos_text;
	pos_text.Format(_T("%d%%"), 100 - pos);
	alpha_text_.SetWindowText(pos_text);
	text_window_.SetAlphaPercent(pos);
	*pResult = 0;
}


void CDesktopTextDlg::OnNMCustomdrawSliderSize(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	uint16_t pos = text_size_slider_.GetPos();
	CString pos_text;
	pos_text.Format(_T("%d"), pos);
	text_size_label_.SetWindowText(pos_text);
	text_window_.SetTextSize(pos);
	*pResult = 0;
}
LRESULT CDesktopTextDlg::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	UINT uMsg = (UINT)lParam;
	switch (uMsg)
	{
	case WM_RBUTTONUP:
	{
		//右键处理
		CMenu menuTray;
		CPoint point;
		int id;
		GetCursorPos(&point);

		menuTray.LoadMenu(IDR_MENU1);
		id = menuTray.GetSubMenu(0)->TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
#if 0
		CString strInfo;
		strInfo.Format(L"menuid %d", id);
		LPCTSTR strtmp;
		strtmp = strInfo.GetBuffer(0);
		MessageBox(strtmp, L"test");
#endif
		switch (id) {
		case IDR_TRAY_QUIT:
			OnOK();
			break;
		case IDR_TRAY_RESUME:
			//窗口前端显示
			SetForegroundWindow();
			ShowWindow(SW_SHOWNORMAL);
			break;
		default:
			break;
		}
		break;
	}
	case WM_LBUTTONDBLCLK:
		SetForegroundWindow();
		ShowWindow(SW_SHOWNORMAL);
		break;
	default:
		break;
	}
	return 0;
}

void CDesktopTextDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (nType == SIZE_MINIMIZED) {
		ShowWindow(SW_HIDE);
	}
}

void CDesktopTextDlg::OnClose()
{
	ShowWindow(SW_HIDE);
}