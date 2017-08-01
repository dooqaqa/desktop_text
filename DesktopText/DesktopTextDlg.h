
// DesktopTextDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcolorbutton.h"
#include <string>
#include <vector>
#include "TextWindow.h"
#include "afxcmn.h"
#include "TextWindowConfig.h"
#define UM_TRAYNOTIFY WM_USER + 11

// CDesktopTextDlg 对话框
class CDesktopTextDlg : public CDialogEx, public ITextWindowListener
{
// 构造
public:
	CDesktopTextDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CDesktopTextDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DESKTOPTEXT_DIALOG };
#endif

// 实现
public:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnNMCustomdrawSliderAlpha(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderSize(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedMfcbuttonApply();
	virtual void OnLeftButtonUp(int x, int y, size_t index);
private:
	void RefreshTextWindow(int index);
	void RefreshAllTextWindow();
	void InitConfigWidgets();
	void UpdateConfigWidgets(size_t index);
	void InitTextWindows();
	void AddNewWindow();
	void UpdateSizeLabel();
	void UpdateAlphaLabel();
	void RemoveRecord(size_t index);
	CEdit main_text_;
	CComboBox text_index_;
	CMFCColorButton text_color_;
	const std::vector<std::pair<LPCTSTR, uint8_t>> text_size_list_ = {std::make_pair(_T("字号10"), 10),std::make_pair(_T("字号20"), 20),
		std::make_pair(_T("字号30"), 30), std::make_pair(_T("字号40"), 40), std::make_pair(_T("字号50"), 50) };
	std::vector<std::shared_ptr<TextWindow>> text_window_list_;
	CStatic alpha_text_;
	CSliderCtrl alpha_silder_;
	CStatic text_size_label_;
	CSliderCtrl text_size_slider_;
	static constexpr int max_font_size = 300;
	NOTIFYICONDATA notifyicondata_id_;
	TextWindowConfig config_;
public:
	afx_msg void OnCbnSelchangeComboTextIndex();
};
