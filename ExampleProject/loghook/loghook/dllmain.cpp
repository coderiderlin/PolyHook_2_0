#include "dllmain.h"
#include "windows.h"
#include "showcmdline.h"
#include "example.h"
#include "unloadme.h"
#include "qqlog.h"
#include "ConsoleMan.h"
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

//#pragma comment(linker, "/INCLUDE:_mainCRTStartup")

//#pragma comment(linker,"/ENTRY:DllMain")

//#pragma comment(linker,"/ALIGN:32")

#include "Utils.h"
#define TAG_PREFIX "yyyyy_"
#define TAG "dllmain"
#define LOGC(FORMAT, ...) (logc(TAG_PREFIX TAG,FORMAT, ##__VA_ARGS__))

#define DLLEXPORT __declspec(dllexport)
#define EXTERN_C extern "C"
#define TITLE "YYA"


int run();
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		LOGC("on DLL_PROCESS_ATTACH");
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)run, NULL, NULL, NULL);
		break;
	case DLL_THREAD_ATTACH:
		LOGC("on DLL_THREAD_ATTACH");
		break;
	case DLL_THREAD_DETACH:
		LOGC("on DLL_THREAD_DETACH");
		break;
	case DLL_PROCESS_DETACH:
		LOGC("on DLL_PROCESS_DETACH");
		break;
	default:;
		LOGC("on unknow reson");
		break;
	}
	return TRUE;
}

int run()
{
	LOGC("================= loghook run ==================");
	showcmdline();

	//polyhook_example();
	//hookMsgBox();
	LifeManager *lifeMan=new LifeManager;

	/*ExampleHook *exampleHook=new ExampleHook;
	exampleHook->hook();
	lifeMan->FreeMeWhileUnload(exampleHook);*/

	ConsoleMan *consoleMan=new ConsoleMan;
	lifeMan->FreeMeWhileUnload(consoleMan);
	

	QQLogHook *qqlogHook=new QQLogHook;
	qqlogHook->hook();
	lifeMan->FreeMeWhileUnload(qqlogHook);

	lifeMan->LoopCheckUnloadMe();
	
	LOGC("================= loghook run out ==================");

	return 0;
}
