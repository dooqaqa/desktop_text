
// DesktopTextDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DesktopText.h"
#include "DesktopTextDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CDesktopTextDlg �Ի���



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
	DDX_Control(pDX, IDC_COMBO_TEXT_INDEX, text_index_);
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
	ON_BN_CLICKED(IDC_MFCBUTTON_APPLY, &CDesktopTextDlg::OnBnClickedMfcbuttonApply)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ALPHA, &CDesktopTextDlg::OnNMCustomdrawSliderAlpha)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SIZE, &CDesktopTextDlg::OnNMCustomdrawSliderSize)
	ON_MESSAGE(UM_TRAYNOTIFY, &CDesktopTextDlg::OnTrayNotify)
	ON_CBN_SELCHANGE(IDC_COMBO_TEXT_INDEX, &CDesktopTextDlg::OnCbnSelchangeComboTextIndex)
END_MESSAGE_MAP()


// CDesktopTextDlg ��Ϣ�������

BOOL CDesktopTextDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	InitConfigWidgets();
	InitTextWindows();

	// ����
	notifyicondata_id_.hWnd = GetSafeHwnd();
	notifyicondata_id_.uCallbackMessage = UM_TRAYNOTIFY;
	// Set tray icon and tooltip
	notifyicondata_id_.hIcon = m_hIcon;
	// Set tray notification tip information
	CString strToolTip = _T("���̳���");
	_tcsncpy_s(notifyicondata_id_.szTip, strToolTip, strToolTip.GetLength());
	Shell_NotifyIcon(NIM_ADD, &notifyicondata_id_);
	RefreshAllTextWindow();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDesktopTextDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDesktopTextDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDesktopTextDlg::OnBnClickedMfcbuttonApply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int index = text_index_.GetCurSel();
	CString text;
	main_text_.GetWindowText(text);
	if (text.IsEmpty()) {
		if (IDYES == MessageBox(_T("Remove this record?"), _T("Warning"), MB_YESNO))
			RemoveRecord(index);
		return;
	}
	if (index >= text_window_list_.size()) {
		index = text_window_list_.size();
		AddNewWindow();
	}
	RECT rc;
	text_window_list_[index]->GetWindowRect(&rc);
	TextConfig cfg;
	COLORREF color = text_color_.GetColor();
	uint16_t font_size = text_size_slider_.GetPos();
	uint8_t alpha_pos = alpha_silder_.GetPos();
	cfg.Assign(text, alpha_pos, (float)text_size_slider_.GetPos() / text_size_slider_.GetRangeMax(), GetRValue(color), GetGValue(color), GetBValue(color), rc.left, rc.top);
	config_.SaveConfig(index, cfg);
	UpdateConfigWidgets(index);
	RefreshTextWindow(index);
}


void CDesktopTextDlg::OnNMCustomdrawSliderAlpha(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	UpdateAlphaLabel();
}


void CDesktopTextDlg::OnNMCustomdrawSliderSize(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	UpdateSizeLabel();
}
LRESULT CDesktopTextDlg::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	UINT uMsg = (UINT)lParam;
	switch (uMsg)
	{
	case WM_RBUTTONUP:
	{
		//�Ҽ�����
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
			//����ǰ����ʾ
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

void CDesktopTextDlg::OnCbnSelchangeComboTextIndex()
{
	// TODO: Add your control notification handler code here
	size_t index = text_index_.GetCurSel();
	if (index >= text_index_.GetCount() - 1) {
		text_index_.InsertString(index, _T("new item"));
		text_index_.SetCurSel(index);
	}
	UpdateConfigWidgets(index);
}

void CDesktopTextDlg::RefreshTextWindow(int index)
{
	TextConfig cfg = std::move(config_.GetConfig(index));
	uint8_t alpha_pos = alpha_silder_.GetPos();
	if (index >= text_window_list_.size()) {
		index = text_window_list_.size();
		AddNewWindow();
	}
	text_window_list_[index]->SetTextColor(RGB(cfg.red_, cfg.green_, cfg.blue_));
	text_window_list_[index]->SetTextSize(text_size_slider_.GetRangeMax() * cfg.size_ratio_);
	text_window_list_[index]->SetText(cfg.content_);
	text_window_list_[index]->SetAlphaPercent(alpha_pos);
	text_window_list_[index]->Draw();

}

void CDesktopTextDlg::RefreshAllTextWindow()
{
	for (auto i = 0; i < config_.GetConfigSize(); ++i) {
		RefreshTextWindow(i);
	}
}

void CDesktopTextDlg::InitConfigWidgets()
{
	// ���ִ��ڳ�ʼ��
	RECT rec_work;
	SystemParametersInfo(SPI_GETWORKAREA, NULL, &rec_work, NULL);
	int max = min(rec_work.right - rec_work.left, rec_work.bottom - rec_work.top) * 0.9;
	text_size_slider_.SetRange(0, max);
	std::vector<TextConfig> cfg_list = std::move(config_.GetAllConfig());
	for (auto& c : cfg_list) {
		text_index_.AddString(c.content_);
	}
	text_index_.AddString(_T("Add new"));
	text_index_.SetCurSel(0);
	UpdateConfigWidgets(0);
}

void CDesktopTextDlg::UpdateConfigWidgets(size_t index)
{
	TextConfig c;
	if (index < config_.GetConfigSize())
		c = std::move(config_.GetConfig(index));
	text_color_.SetColor(RGB(c.red_, c.green_, c.blue_));
	text_size_slider_.SetPos(text_size_slider_.GetRangeMax() * c.size_ratio_);
	alpha_silder_.SetRange(0, 100);
	alpha_silder_.SetPos(c.alpha_percent_);
	main_text_.SetWindowText(c.content_);
	UpdateSizeLabel();
	UpdateAlphaLabel();
}

void CDesktopTextDlg::InitTextWindows()
{
	std::vector<TextConfig> cfg = std::move(config_.GetAllConfig());
	RECT rec_work;
	SystemParametersInfo(SPI_GETWORKAREA, NULL, &rec_work, NULL);
	for (auto& c : cfg) {
		int x(c.left_x_), y(c.top_y_);
		text_window_list_.push_back(std::make_shared<TextWindow>(text_window_list_.size(), this));
		auto& w = text_window_list_.back();
		if (x < rec_work.left || x > rec_work.right) x = rec_work.left;
		if (y < rec_work.top || y > rec_work.bottom) y = rec_work.top;
		w->Create(x, y);
	}
}

void CDesktopTextDlg::AddNewWindow()
{
	text_window_list_.push_back(std::make_shared<TextWindow>(text_window_list_.size(), this));
	auto& w = text_window_list_.back();
	w->Create(0, 0);
}

void CDesktopTextDlg::UpdateSizeLabel()
{
	uint16_t pos = text_size_slider_.GetPos();
	CString pos_text;
	pos_text.Format(_T("%d"), pos);
	text_size_label_.SetWindowText(pos_text);
	//text_window_list_[index]->SetTextSize(pos);
}

void CDesktopTextDlg::UpdateAlphaLabel()
{
	uint8_t pos = alpha_silder_.GetPos();
	CString pos_text;
	pos_text.Format(_T("%d%%"), 100 - pos);
	alpha_text_.SetWindowText(pos_text);
	//text_window_list_[index]->SetAlphaPercent(pos);
}

void CDesktopTextDlg::RemoveRecord(size_t index)
{
	if (2 < text_index_.GetCount()) {
		text_index_.DeleteString(index);
		text_index_.SetCurSel(index == 0 ? 0 : index - 1);
		OnCbnSelchangeComboTextIndex();
	}
	config_.RemoveConfig(index);
}

void CDesktopTextDlg::OnLeftButtonUp(int x, int y, size_t index)
{
	if (index >= config_.GetConfigSize() || index >= text_window_list_.size())
		return;
	TextConfig cfg = std::move(config_.GetConfig(index));
	RECT rc;
	text_window_list_[index]->GetWindowRect(&rc);
	cfg.left_x_ = rc.left;
	cfg.top_y_ = rc.top;
	config_.SaveConfig(index, cfg);
}