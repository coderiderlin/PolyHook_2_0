#include "qqlog.h"

#include <cstdarg>
#include "headers/CapstoneDisassembler.hpp"
#include "headers/Detour/x86Detour.hpp"
#include "Utils.h"
#include <thread>


#define TAG_PREFIX "yyyyy_"
#define TAG "qqlog"
#define LOGC(FORMAT, ...) (logc(TAG_PREFIX TAG,FORMAT, ##__VA_ARGS__))

#define TAG_PREFIXW L"yyyyy_"
#define TAGW L"qqlog"
#define LOGCW(FORMAT, ...) (logcw(TAG_PREFIXW TAGW,FORMAT, ##__VA_ARGS__))

typedef void(__cdecl* FUNC_e_TXLog_DoTXLogVW)(int handle, wchar_t* tag, wchar_t* format, va_list vlist);
#define FUNC_NAME_e_TXLog_DoTXLogVW "e_TXLog_DoTXLogVW"
#define DLL_NAME_common "common.dll"






uint64_t tramp_e_txlog_dotxlogvw = NULL;

void __cdecl my_e_txlog_dotxlogvw(int handle, wchar_t* tag, wchar_t* format, va_list vlist)
{
	wchar_t buffer[512];
	_vsnwprintf_s(buffer, 512, _TRUNCATE,format, vlist);
	LOGCW(L"[e_txlog_dotxlogvw][%08X][%s][%s]", handle, tag, buffer);
	return;

}

FUNC_e_TXLog_DoTXLogVW getFUNC_e_TXLog_DoTXLogVW()
{
	HMODULE hcommon = GetModuleHandle(DLL_NAME_common);
	if (!hcommon)
	{
		LOGC("hook failed null hcommon:%d", GetLastError());
		return NULL;
	}
	LOGC("hcommon:%08X", hcommon);
	FUNC_e_TXLog_DoTXLogVW e_TXLog_DoTXLogVW = (FUNC_e_TXLog_DoTXLogVW)GetProcAddress(hcommon, FUNC_NAME_e_TXLog_DoTXLogVW);
	LOGC("e_TXLog_DoTXLogVW:%p", e_TXLog_DoTXLogVW);
	if (!e_TXLog_DoTXLogVW)
	{
		LOGC("hook failed null e_TXLog_DoTXLogVW:%d", GetLastError());
		return NULL;
	}
	return e_TXLog_DoTXLogVW;
	/*PLH::CapstoneDisassembler dis(PLH::Mode::x86);
	PLH::x86Detour detour((char*)& e_TXLog_DoTXLogVW, (char*)& my_e_txlog_dotxlogvw, &tramp_e_txlog_dotxlogvw, dis);
	detour.hook();
	LOGC("hooked!");*/
}

QQLogHook::QQLogHook()
{
	FUNC_e_TXLog_DoTXLogVW e_TXLog_DoTXLogVW = getFUNC_e_TXLog_DoTXLogVW();
	if (e_TXLog_DoTXLogVW)
	{
		PLH::x86Detour* detour = new PLH::x86Detour((char*)e_TXLog_DoTXLogVW, (char*)& my_e_txlog_dotxlogvw, &tramp_e_txlog_dotxlogvw, *((PLH::CapstoneDisassembler*)m_dis));
		m_detour = detour;
	}


}

QQLogHook::~QQLogHook()
{
	LOGC("on %s", __FUNCTION__);
	if (m_detour)delete ((PLH::x86Detour*)m_detour);
}

void QQLogHook::hook()
{
	LOGC("on %s", __FUNCTION__);
	if (m_detour)
	{
		for (int i = 0; i < 5; i++)
		{
			m_isHook = ((PLH::x86Detour*)m_detour)->hook();
			LOGC("%s m_isHook:%d", __FUNCTION__, IsHook());
			if (IsHook())
			{
				LOGC("hook ok!");
				break;
			}
			this_thread::sleep_for(chrono::milliseconds(2000));
		}

	}
}

void QQLogHook::unhook()
{
	LOGC("on %s", __FUNCTION__);
	if (m_detour)
	{
		((PLH::x86Detour*)m_detour)->unHook();
	}
}
