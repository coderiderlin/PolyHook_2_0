#include "HookBase.h"

#include <cstdarg>
#include "headers/CapstoneDisassembler.hpp"
#include "headers/Detour/x86Detour.hpp"
#include "headers/ErrorLog.hpp"
#include "Utils.h"


#define TAG_PREFIX "yyyyy_"
#define TAG "HookBase"
#define LOGC(FORMAT, ...) (logc(TAG_PREFIX TAG,FORMAT, ##__VA_ARGS__))

HookBase::HookBase()
{
	LOGC("on %s", __FUNCTION__);
	m_dis = (void*)new PLH::CapstoneDisassembler(PLH::Mode::x86);
}

HookBase::~HookBase()
{
	LOGC("on %s", __FUNCTION__);
	delete ((PLH::CapstoneDisassembler*)m_dis);
}

void HookBase::freeMe()
{
	unhook();
}
