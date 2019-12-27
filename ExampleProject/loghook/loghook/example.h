#include "HookBase.h";
int polyhook_example();


class ExampleHook:public HookBase
{
public:
	ExampleHook();
	virtual ~ExampleHook();
	
	// Inherited via HookBase
	virtual void hook() override;
	virtual void unhook() override;
protected:
	void *m_detour=nullptr;


};
