#pragma once
#include "LifeBase.h"
class ConsoleMan:public LifeBase
{
public:
	ConsoleMan();
	virtual ~ConsoleMan();
	// Inherited via LifeBase
	virtual void freeMe() override;
protected:

};

