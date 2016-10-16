#pragma once
#include <gdiplus.h>
#pragma comment(lib,"GdiPlus.lib")
#include <memory>

// TextWindow

class TextWindow : public CWnd
{
	DECLARE_DYNAMIC(TextWindow)

public:
	TextWindow();
	virtual ~TextWindow();
	BOOL Create(LPCTSTR class_name, int x, int y, int width, int height);
	BOOL Create(int x, int y);
	BOOL Create();
	void SetTextColor(COLORREF color);
	void SetTextSize(unsigned short sz);
	void SetText(const CString& text);
	void SetTextFont(const CString& font, Gdiplus::REAL em_size, INT style = Gdiplus::FontStyleRegular, Gdiplus::Unit unit = Gdiplus::UnitPixel);
	void SetAlphaPercent(unsigned char percent);
	void Draw();
	BOOL TextWindow::RegisterWndClass(LPCTSTR class_name);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT create);
	afx_msg void OnDestroy();
	void Destroy();

protected:
	DECLARE_MESSAGE_MAP()
	HDC dc_;
	int width_;
	int height_;
	CString text_;
	int font_style_;
	Gdiplus::REAL font_size_;
	COLORREF text_color_;
	unsigned char text_alpha_ = 255;
	std::unique_ptr<Gdiplus::FontFamily> font_family_ = nullptr;
	std::unique_ptr<Gdiplus::StringFormat> text_format_ = nullptr;
	std::unique_ptr<Gdiplus::Font> font_ = nullptr;
	void ResizeWindow();
	void SetAlphaWindow();
	HBITMAP CreateBitmap();
};


