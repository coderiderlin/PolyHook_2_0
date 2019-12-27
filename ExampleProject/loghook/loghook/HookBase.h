#pragma once
#include "LifeBase.h"
class HookBase:public LifeBase
{
public:
	HookBase();
	virtual ~HookBase();
	virtual void hook()=0;
	virtual void unhook()=0;
	bool IsHook(){return m_isHook;}
protected:
	bool m_isHook=false;
	void *m_dis=nullptr;

	// Inherited via LifeBase
	virtual void freeMe() override;
};

