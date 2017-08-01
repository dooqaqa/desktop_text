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
	LoadConfig(path_);
}


TextWindowConfig::~TextWindowConfig()
{
}


void TextWindowConfig::SaveConfig(size_t index, const TextConfig& cfg)
{
	if (config_list_.size() > index)
		config_list_[index] = cfg;
	else
		config_list_.push_back(cfg);
	FlushToFile();
}

std::vector<TextConfig> TextWindowConfig::GetAllConfig()
{
	return config_list_;
}

TextConfig TextWindowConfig::GetConfig(size_t index)
{
	if (index >= config_list_.size())
		return TextConfig();
	return config_list_[index];
}
void TextWindowConfig::LoadConfig(const CString& path)
{
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");//设定

	CStdioFile file;
	CFileException file_exception;
	if (file.Open(path, CFile::typeText | CFile::modeRead)) {
		CString index;
		while (file.ReadString(index)) {
			auto cfg = config_list_.insert(config_list_.end(), TextConfig());
			CString line;
			while (file.ReadString(line)) {
				if (0 == line.Compare(_T("[text end]"))) {
					break;
				}
				if (!cfg->content_.IsEmpty()) cfg->content_ += _T("\n");
				cfg->content_ += line;
			}
			CString num;
			file.ReadString(num);
			cfg->alpha_percent_ = _ttoi(num);
			file.ReadString(num);
			cfg->size_ratio_ = _ttof(num);
			file.ReadString(num);
			cfg->red_ = _ttoi(num);
			file.ReadString(num);
			cfg->green_ = _ttoi(num);
			file.ReadString(num);
			cfg->blue_ = _ttoi(num);
			file.ReadString(num);
			cfg->left_x_ = _ttoi(num);
			file.ReadString(num);
			cfg->top_y_ = _ttoi(num);
		}
		file.Close();
	}

	setlocale(LC_CTYPE, old_locale);
	free(old_locale);//还原区域设定
}

void TextWindowConfig::RemoveConfig(size_t index)
{
	if (config_list_.size() > index)
		config_list_.erase(config_list_.begin() + index);
	FlushToFile();
}

void TextWindowConfig::FlushToFile()
{
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");//设定
	CStdioFile file;
	CFileException file_exception;
	if (file.Open(path_, CFile::typeText | CFile::modeWrite | CFile::modeCreate)) {
		file.SetLength(0);
		size_t i = 0;
		CString total;
		for (auto& c : config_list_) {
			CString index_str;
			index_str.Format(_T("%d\n"), i++);
			CString num;
			num.Format(_T("%d\n%.3f\n%d\n%d\n%d\n%d\n%d\n"), c.alpha_percent_, c.size_ratio_, c.red_, c.green_, c.blue_, c.left_x_, c.top_y_);
			total += index_str + c.content_ + _T("\n[text end]\n") + num;
		}
		file.WriteString(total);
		file.Close();
	}

	setlocale(LC_CTYPE, old_locale);
	free(old_locale);//还原区域设定
}