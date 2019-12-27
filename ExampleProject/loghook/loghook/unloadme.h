#include "HookBase.h"
class LifeManager
{
public:
	void LoopCheckUnloadMe();
	void FreeMeWhileUnload(LifeBase *hook);
private:
	void *m_lifeList=nullptr;
};