#include "showcmdline.h"
#include "windows.h"
#include "Utils.h"
#pragma  warning(disable:4996)

#include "Utils.h"
#define TAG_PREFIX "yyyyy_"
#define TAG "dllmain"
#define LOGC(FORMAT, ...) (logc(TAG_PREFIX TAG,FORMAT, ##__VA_ARGS__))

void showcmdline()
{
	char* cmd = 0;
	cmd = GetCommandLine();
	LOGC("cmdline:[%s]",cmd);
}