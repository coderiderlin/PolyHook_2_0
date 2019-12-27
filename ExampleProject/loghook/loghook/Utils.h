#pragma once
#include <string>
#include "stdio.h"
#include <memory>
#include "windows.h"
using namespace std;
#pragma  warning(disable:4996)

template<typename ... Args>
std::string string_format(const std::string &format, Args ... args){
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

template<typename ... Args>
std::wstring wstring_format(const std::wstring &format, Args ... args){
	size_t size = _snwprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	std::unique_ptr<wchar_t[]> buf(new wchar_t[size]);
	_snwprintf(buf.get(), size, format.c_str(), args ...);
	return std::wstring(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

template<typename ... Args>
void logc(const std::string &tag,const std::string &format, Args ... args){
	string msg=string_format(format.c_str(), args ...);
	msg=string_format("[%05d %05d][%s]%s\n",GetCurrentProcessId(),GetCurrentThreadId(),tag.c_str(),msg.c_str());
	OutputDebugStringA(msg.c_str());
}

template<typename ... Args>
void logcw(const std::wstring &tag,const std::wstring &format, Args ... args){
	wstring msg=wstring_format(format.c_str(), args ...);
	msg=wstring_format(L"[%05d %05d][%s]%s\n",GetCurrentProcessId(),GetCurrentThreadId(),tag.c_str(),msg.c_str());
	OutputDebugStringW(msg.c_str());
}

// the following functions are used to convert encodings between MBCS and Unicode
bool MBCSToUnicode(const char *input, std::wstring& output, int code_page = CP_ACP);
bool MBCSToUnicode(const std::string &input, std::wstring& output, int code_page = CP_ACP);
bool UnicodeToMBCS(const wchar_t *input, std::string &output, int code_page = CP_ACP);
bool UnicodeToMBCS(const std::wstring& input, std::string &output, int code_page = CP_ACP);

string ws2s(const wstring & str);