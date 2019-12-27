#include "Utils.h"


bool MBCSToUnicode(const char *input, std::wstring& output, int code_page)
{
	output.clear();
	int length = ::MultiByteToWideChar(code_page, 0, input, -1, NULL, 0);
	if (length <= 0)
		return false;
	output.resize(length-1);
	if (output.empty())
		return true;
	::MultiByteToWideChar(code_page,
						  0,
						  input,
						  -1,
						  &output[0],
						  static_cast<int>(output.size()));
	return true;
}

bool MBCSToUnicode(const std::string &input, std::wstring& output, int code_page)
{
	output.clear();
	int length = ::MultiByteToWideChar(code_page, 0, input.c_str(), static_cast<int>(input.size()), NULL, 0);
	output.resize(length);
	if (output.empty())
		return true;
	::MultiByteToWideChar(code_page,
						  0,
						  input.c_str(),
						  static_cast<int>(input.size()),
						  &output[0],
						  static_cast<int>(output.size()));
	return true;
}

bool UnicodeToMBCS(const wchar_t *input, std::string &output, int code_page)
{
	output.clear();
	int length = ::WideCharToMultiByte(code_page, 0, input, -1, NULL, 0, NULL, NULL);
	if (length <= 0)
		return false;
	output.resize(length-1);
	if (output.empty())
		return true;
	::WideCharToMultiByte(code_page,
						  0,
						  input,
						  length-1,
						  &output[0],
						  static_cast<int>(output.size()),
						  NULL,
						  NULL);
	return true;
}

bool UnicodeToMBCS(const std::wstring& input, std::string &output, int code_page)
{
	output.clear();
	int length = ::WideCharToMultiByte(code_page, 0, input.c_str(), static_cast<int>(input.size()), NULL, 0, NULL, NULL);
	output.resize(length);
	if (output.empty())
		return true;
	::WideCharToMultiByte(code_page,
						  0,
						  input.c_str(),
						  static_cast<int>(input.size()),
						  &output[0],
						  static_cast<int>(output.size()),
						  NULL,
						  NULL);
	return true;
}

string ws2s(const wstring & str)
{
	string tstr;
	UnicodeToMBCS(str, tstr);
	return tstr;
}

wstring s2ws(const string & str)
{
	wstring tstr;
	MBCSToUnicode(str, tstr);
	return tstr;
}