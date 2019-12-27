#include "example.h"

#include <cstdarg>
#include "headers/CapstoneDisassembler.hpp"
#include "headers/Detour/x86Detour.hpp"
#include "Utils.h"


#define TAG_PREFIX "yyyyy_"
#define TAG "example"
#define LOGC(FORMAT, ...) (logc(TAG_PREFIX TAG,FORMAT, ##__VA_ARGS__))


uint64_t hookPrintfTramp = NULL;
NOINLINE int __cdecl h_hookPrintf(const char* format, ...) {
	char buffer[512];
	va_list args;
	va_start(args, format);
	vsprintf_s(buffer, format, args);
	va_end(args);
	LOGC("%s", buffer);
	return PLH::FnCast(hookPrintfTramp, &printf)("INTERCEPTED YO:%s", buffer);
}

/** THIS EXAMPLE IS SETUP FOR x86. IT WILL CRASH IF YOU COMPILE IN x64**/
int polyhook_example()
{
	// Switch modes for x64
	PLH::CapstoneDisassembler dis(PLH::Mode::x86);
	PLH::x86Detour detour((char*)& printf, (char*)& h_hookPrintf, &hookPrintfTramp, dis);
	detour.hook();

	printf("%s %f\n", "hi", .5f);
	detour.unHook();
	return 0;
}
//////////////////////////////////////////////////////////////////////

uint64_t trampMessageBoxA = NULL;
NOINLINE  int WINAPI myMessageBoxA(
	HWND hWnd,
	LPCSTR lpText,
	LPCSTR lpCaption,
	UINT uType)
{
	string cap = string_format("l0veyya~ [%s]", lpCaption);
	LOGC("cap replace to %s", cap.c_str());
	return PLH::FnCast(trampMessageBoxA, &MessageBoxA)(hWnd, lpText, cap.c_str(), uType);

}

ExampleHook::ExampleHook()
{
	LOGC("on %s", __FUNCTION__);
	PLH::x86Detour* detour = new PLH::x86Detour((char*)& MessageBoxA, (char*)& myMessageBoxA, &trampMessageBoxA, *((PLH::CapstoneDisassembler*)m_dis));
	m_detour = detour;
}

ExampleHook::~ExampleHook()
{
	LOGC("on %s", __FUNCTION__);
	if (m_detour)delete ((PLH::x86Detour*)m_detour);

}

void ExampleHook::hook()
{
	LOGC("on %s", __FUNCTION__);
	if (m_detour)
	{
		m_isHook = ((PLH::x86Detour*)m_detour)->hook();
		LOGC("ExampleHook m_isHook:%d", IsHook());
	}

}

void ExampleHook::unhook()
{
	LOGC("on %s", __FUNCTION__);
	if (m_detour)((PLH::x86Detour*)m_detour)->unHook();
}
