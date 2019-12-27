#include "HookBase.h"
void hooktxlog();
class QQLogHook :public HookBase
{
public:
	QQLogHook();
	virtual ~QQLogHook();
	// Inherited via HookBase
	virtual void hook() override;
	virtual void unhook() override;
protected:
	void* m_detour = nullptr;
};