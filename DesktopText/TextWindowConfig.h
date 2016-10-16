#pragma once
class TextWindowConfig
{
public:
	TextWindowConfig();
	~TextWindowConfig();
	void SaveConfig(const CString& text, BYTE alpha_percent, float size_ratio, BYTE r, BYTE g, BYTE b, int x, int y);
	void LoadConfig(CString& text, BYTE& alpha_percent, float& size_ratio, BYTE& r, BYTE& g, BYTE& b, int& x, int& y);
private:
	CString path_;
};