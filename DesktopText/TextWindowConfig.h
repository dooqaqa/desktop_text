#pragma once
#include <vector>
class TextConfig
{
public:
	CString content_;
	BYTE alpha_percent_ = 0;
	float size_ratio_ = 0;
	BYTE red_ = 0;
	BYTE green_ = 0;
	BYTE blue_ = 0;
	int left_x_ = 0;
	int top_y_ = 0;
	void Assign(const CString& content, BYTE alpha_percent, float size_ratio, BYTE red, BYTE green, BYTE blue, int left_x, int top_y) {
		content_ = content;
		alpha_percent_ = alpha_percent;
		size_ratio_ = size_ratio;
		red_ = red;
		green_ = green;
		blue_ = blue;
		left_x_ = left_x;
		top_y_ = top_y;
	}
};

class TextWindowConfig
{
public:
	TextWindowConfig();
	~TextWindowConfig();
	std::vector<TextConfig> GetAllConfig();
	TextConfig GetConfig(size_t index);
	void SaveConfig(size_t index, const TextConfig& cfg);
	void LoadConfig(const CString& path);
	size_t GetConfigSize() { return config_list_.size(); }
	void RemoveConfig(size_t index);

private:
	void FlushToFile();
	CString path_;
	std::vector<TextConfig> config_list_;
};