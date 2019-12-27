#pragma once
class LifeBase
{
public:
	LifeBase()=default;
	virtual ~LifeBase()=default;
	virtual void freeMe()=0;
};

