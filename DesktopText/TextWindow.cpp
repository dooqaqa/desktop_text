// TextWindow.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopText.h"
#include "TextWindow.h"
using namespace Gdiplus;

// TextWindow

IMPLEMENT_DYNAMIC(TextWindow, CWnd)

TextWindow::TextWindow()
{
	HDC hdc = ::GetDC(NULL);
	dc_ = ::CreateCompatibleDC(hdc);
	::ReleaseDC(NULL, hdc);

}

TextWindow::~TextWindow()
{
}


BEGIN_MESSAGE_MAP(TextWindow, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_CREATE()
END_MESSAGE_MAP()

int TextWindow::OnCreate(LPCREATESTRUCT create)
{
	font_family_ = std::make_unique<Gdiplus::FontFamily>();
	text_format_ = std::make_unique<Gdiplus::StringFormat>();
	text_format_->SetFormatFlags(StringFormatFlagsNoWrap);//不换行
	text_format_->SetAlignment(StringAlignmentCenter); //置水平对齐方式
	text_format_->SetLineAlignment(StringAlignmentNear); //置垂直对齐方式
	SetTextFont(_T("微软雅黑"), font_size_, FontStyleBold);

	return 0;
}
void TextWindow::OnDestroy()
{
	font_family_.release();
	text_format_.release();
	font_.release();
}


// TextWindow 消息处理程序

BOOL TextWindow::Create(LPCTSTR class_name, int x, int y, int width, int height)
{
	width_ = width;
	height_ = height;
	if (!RegisterWndClass(class_name))
	{
		TRACE("Class　Registration　Failedn");
	}
	DWORD dwStyle = WS_POPUP | WS_VISIBLE;
	DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_LAYERED;
	if (!CWnd::CreateEx(dwExStyle, class_name, NULL, dwStyle, x, y, width, height, NULL, NULL))
		return FALSE;
	return 0;
}

BOOL TextWindow::Create(int x, int y)
{
	Create(_T("TextWindow"), x, y, 0, 0);
	return 0;
}

BOOL TextWindow::Create()
{
	////取出桌面工作区域
	//RECT rec_work;
	//SystemParametersInfo(SPI_GETWORKAREA, NULL, &rec_work, NULL);
	//int work_width = rec_work.right - rec_work.left;
	//int work_height = rec_work.bottom - rec_work.top;
	////设置左边、顶边位置,让窗口在屏幕下方
	//int x = rec_work.left + ((work_width - width_) / 2);
	//int y = rec_work.top + ((work_height - height_) / 2);
	////int y = rec_work.bottom - height;
	Create(_T("TextWindow"), 0, 0, 0, 0);
	return 0;
}

void TextWindow::Destroy()
{
	DestroyWindow();
}


void TextWindow::SetTextColor(COLORREF color)
{
	text_color_ = color;
}

void TextWindow::SetTextSize(unsigned short sz)
{
	font_size_ = sz;
	SetTextFont(_T("微软雅黑"), font_size_, FontStyleBold);
}

void TextWindow::SetText(const CString& text)
{
	text_ = text;
}

void TextWindow::Draw()
{
	ResizeWindow();
	HBITMAP hBitmap = CreateBitmap();
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(dc_, hBitmap);

	Gdiplus::Graphics graphics(dc_);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);

	std::unique_ptr<Gdiplus::Brush> brush = std::make_unique<Gdiplus::SolidBrush>(Gdiplus::Color(text_alpha_, GetRValue(text_color_), GetGValue(text_color_), GetBValue(text_color_)));
	graphics.DrawString(text_, -1, font_.get(), PointF(0, 0), brush.get());

	//----------------------------------
	SetAlphaWindow();
	//释放资源
	HDC hDC = ::GetDC(m_hWnd);
	::SelectObject(dc_, hOldBitmap);
	::DeleteObject(hBitmap);
	::ReleaseDC(m_hWnd, hDC);

}

void TextWindow::SetTextFont(const CString& font, REAL emSize, INT style, Unit unit)
{

	if (font_) {
		font_.release();
	}
	Gdiplus::FontFamily family(font, NULL);
	Status lastResult = family.GetLastStatus();
	if (lastResult != Ok)
	{
		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		LOGFONTW lf;
		ZeroMemory(&lf, sizeof(LOGFONTW));
		GetObjectW(hFont, sizeof(LOGFONTW), &lf);
		Gdiplus::FontFamily family2(lf.lfFaceName, NULL);
		font_ = std::make_unique<Gdiplus::Font>(&family2, emSize, style, unit);
	}
	else {
		font_ = std::make_unique<Gdiplus::Font>(&family, emSize, style, unit);
	}
}

BOOL TextWindow::RegisterWndClass(LPCTSTR class_name)
{
	HINSTANCE hInstance = AfxGetInstanceHandle();
	WNDCLASSEX wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASSEX));
	wndcls.cbSize = sizeof(WNDCLASSEX);
	if (GetClassInfoEx(hInstance, class_name, &wndcls))
	{
		return TRUE;
	}
	if (GetClassInfoEx(NULL, class_name, &wndcls))
	{
		return TRUE;
	}

	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = hInstance;
	wndcls.hIcon = NULL;
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndcls.lpszMenuName = NULL;
	wndcls.lpszClassName = class_name;
	if (!RegisterClassEx(&wndcls))
	{
		return FALSE;
	}
	return TRUE;
}

void TextWindow::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);
	ReleaseCapture();
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, NULL);
}

void TextWindow::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CWnd::OnLButtonUp(nFlags, point);
}

void TextWindow::SetAlphaPercent(unsigned char percent)
{
	text_alpha_ = 255 * percent / 100;
}

void TextWindow::ResizeWindow()
{
	RECT rect = { 0 };
	GetWindowRect(&rect);
	Graphics calc(dc_);
	Gdiplus::RectF layout_rect(0, 0, 0, 0);
	Gdiplus::RectF boundingBox;
	calc.MeasureString(text_, -1, font_.get(), layout_rect, text_format_.get(), &boundingBox, 0, 0);
	rect.right = rect.left + (LONG)boundingBox.Width;
	rect.bottom = rect.top + (LONG)boundingBox.Height;
	MoveWindow(&rect);
}

void TextWindow::SetAlphaWindow()
{
	//设置透明窗口
	CPoint DestPt(0, 0);
	CSize psize(width_, height_);
	BLENDFUNCTION blendFunc32bpp;
	blendFunc32bpp.AlphaFormat = AC_SRC_ALPHA;
	blendFunc32bpp.BlendFlags = 0;
	blendFunc32bpp.BlendOp = AC_SRC_OVER;
	blendFunc32bpp.SourceConstantAlpha = 255;
	HDC hDC = ::GetDC(m_hWnd);
	::UpdateLayeredWindow(m_hWnd, hDC, NULL, &psize, dc_, &DestPt, 0, &blendFunc32bpp, ULW_ALPHA);
}

HBITMAP TextWindow::CreateBitmap()
{
	CRect rcClient;
	GetClientRect(rcClient);
	width_ = rcClient.Width();
	height_ = rcClient.Height();

	BITMAPINFO bitmapinfo;
	bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biBitCount = 32;
	bitmapinfo.bmiHeader.biHeight = height_;
	bitmapinfo.bmiHeader.biWidth = width_;
	bitmapinfo.bmiHeader.biPlanes = 1;
	bitmapinfo.bmiHeader.biCompression = BI_RGB;
	bitmapinfo.bmiHeader.biXPelsPerMeter = 0;
	bitmapinfo.bmiHeader.biYPelsPerMeter = 0;
	bitmapinfo.bmiHeader.biClrUsed = 0;
	bitmapinfo.bmiHeader.biClrImportant = 0;
	bitmapinfo.bmiHeader.biSizeImage = bitmapinfo.bmiHeader.biWidth * bitmapinfo.bmiHeader.biHeight * bitmapinfo.bmiHeader.biBitCount / 8;
	return CreateDIBSection(dc_, &bitmapinfo, 0, NULL, 0, 0);
}