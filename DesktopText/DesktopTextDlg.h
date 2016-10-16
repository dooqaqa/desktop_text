
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
class CDesktopTextDlg : public CDialogEx
{
// 构造
public:
	CDesktopTextDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CDesktopTextDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DESKTOPTEXT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

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
public:
	afx_msg void OnBnClickedMfcbuttonUpdate();
private:
	CEdit main_text_;
	CComboBox text_size_;
	CMFCColorButton text_color_;
	const std::vector<std::pair<LPCTSTR, uint8_t>> text_size_list_ = {std::make_pair(_T("字号10"), 10),std::make_pair(_T("字号20"), 20),
		std::make_pair(_T("字号30"), 30), std::make_pair(_T("字号40"), 40), std::make_pair(_T("字号50"), 50) };
	TextWindow text_window_;
	CStatic alpha_text_;
	CSliderCtrl alpha_silder_;
	CStatic text_size_label_;
	CSliderCtrl text_size_slider_;
	static constexpr int max_font_size = 300;
	NOTIFYICONDATA notifyicondata_id_;
	TextWindowConfig config_;
};
