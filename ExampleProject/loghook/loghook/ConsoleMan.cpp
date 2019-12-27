#include "ConsoleMan.h"
#include "windows.h"
#include "stdio.h"

#include "Utils.h"
#define TAG_PREFIX "yyyyy_"
#define TAG "ConsoleMan"
#define LOGC(FORMAT, ...) (logc(TAG_PREFIX TAG,FORMAT, ##__VA_ARGS__))

ConsoleMan::ConsoleMan()
{
	if (AllocConsole())
	{
		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);
		printf("MINI_DLL console [OK]\n");
		LOGC("MINI_DLL console [OK]\n");
	}
	else
	{
		LOGC("AllocConsole FAILED:%d", GetLastError());
	}
}

ConsoleMan::~ConsoleMan()
{
	LOGC("%s",__FUNCTION__);
	FreeConsole();
}

void ConsoleMan::freeMe()
{
}
