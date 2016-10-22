#include "stdafx.h"
#include "TextWindowConfig.h"
#include <Shlobj.h>  
#include <locale.h>

TextWindowConfig::TextWindowConfig()
{
	TCHAR my_documents[MAX_PATH] = { 0 };
	SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, my_documents);
	path_ = my_documents;
	path_ += "\\text_window_config.txt";
}


TextWindowConfig::~TextWindowConfig()
{
}


void TextWindowConfig::SaveConfig(const CString& text, BYTE alpha_percent, float size_ratio, BYTE r, BYTE g, BYTE b, int x, int y)
{
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");//设定

	CStdioFile file;
	CFileException file_exception;
	if (file.Open(path_, CFile::typeText | CFile::modeWrite), &file_exception) {
		CString num;
		num.Format(_T("%d\n%.3f\n%d\n%d\n%d\n%d\n%d\n"), alpha_percent, size_ratio, r, g, b, x, y);
		CString total = text + _T("\n[text end]\n") + num;
		file.WriteString(total);
		file.Close();
	}
	
	setlocale(LC_CTYPE, old_locale);
	free(old_locale);//还原区域设定
}

void TextWindowConfig::LoadConfig(CString& text, BYTE& alpha_percent, float& size_ratio, BYTE& r, BYTE& g, BYTE& b, int& x, int& y)
{
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");//设定

	CStdioFile file;
	CFileException file_exception;
	if (file.Open(path_, CFile::typeText | CFile::modeRead), &file_exception) {
		CString line;
		while (file.ReadString(line)) {
			if (0 == line.Compare(_T("[text end]"))) {
				break;
			}
			text += line;
			text += _T("\n");
		}
		CString num;
		file.ReadString(num);
		alpha_percent = _ttoi(num);
		file.ReadString(num);
		size_ratio = _ttof(num);
		file.ReadString(num);
		r = _ttoi(num);
		file.ReadString(num);
		g = _ttoi(num);
		file.ReadString(num);
		b = _ttoi(num);
		file.ReadString(num);
		x = _ttoi(num);
		file.ReadString(num);
		y = _ttoi(num);
		file.Close();
	}

	setlocale(LC_CTYPE, old_locale);
	free(old_locale);//还原区域设定
}