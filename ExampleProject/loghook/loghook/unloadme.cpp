#include "unloadme.h"
#include <string>
#include <thread>
#include <vector>
#include "Utils.h"
#include "windows.h"
#include "headers/errorlog.hpp"
using namespace std;

#define TAG_PREFIX "yyyyy_"
#define TAG "unloadme"
#define LOGC(FORMAT, ...) (logc(TAG_PREFIX TAG,FORMAT, ##__VA_ARGS__))

HMODULE GetSelfModuleHandle()
{
	MEMORY_BASIC_INFORMATION mbi;

	return ((::VirtualQuery(GetSelfModuleHandle, &mbi, sizeof(mbi)) != 0)
		? (HMODULE)mbi.AllocationBase : NULL);
}

void unloadMe()
{
	HMODULE hmod = GetSelfModuleHandle();
	LOGC("FreeLibrary %08X", hmod);
	FreeLibrary(hmod); //free the injector reference 
	LOGC("FreeLibraryAndExitThread %08X", hmod);
	FreeLibraryAndExitThread(hmod, 0);
}
void PopErrMsg()
{
	string msg=PLH::ErrorLog::singleton().pop().msg;
	if(!msg.empty())
	{
		LOGC("[PLH ERR]%s",msg.c_str());
	}
}
typedef vector<LifeBase *> Life_LIST;
void LifeManager::LoopCheckUnloadMe()
{
	thread t([this]
		{
			this_thread::sleep_for(chrono::milliseconds(1000));

			while (true)
			{
				HANDLE hev = OpenEvent(SYNCHRONIZE, NULL, "YYAHOOK_UnloadMe");
				if (hev)
				{
					CloseHandle(hev);
					//unhook all and free the list
					if(m_lifeList)
					{
						Life_LIST *hookList=(Life_LIST *)m_lifeList;
						for(auto phook:*hookList)
						{
							phook->freeMe();
							delete phook;
						}
						delete hookList;
					}
					LOGC("unloading me..");
					unloadMe();
					LOGC("unloaded.");
					//ResetEvent(hev);
				}
				PopErrMsg();
				this_thread::sleep_for(chrono::milliseconds(100));
			}
		});
	t.detach();
}

void LifeManager::FreeMeWhileUnload(LifeBase* hook)
{
	if(!m_lifeList)
	{
		m_lifeList=(void *)new Life_LIST;
	}
	Life_LIST *hookList=(Life_LIST *)m_lifeList;
	hookList->push_back(hook);
}
