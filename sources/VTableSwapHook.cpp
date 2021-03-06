#include "headers/Virtuals/VTableSwapHook.hpp"

PLH::VTableSwapHook::VTableSwapHook(const char* Class, const VFuncMap& redirectMap) 
	: VTableSwapHook((uint64_t)Class, redirectMap)
{}

PLH::VTableSwapHook::VTableSwapHook(const uint64_t Class, const VFuncMap& redirectMap) 
	: m_class(Class)
	, m_redirectMap(redirectMap)
{}

bool PLH::VTableSwapHook::hook() {
	MemoryProtector prot(m_class, sizeof(void*), ProtFlag::R | ProtFlag::W);
	m_origVtable = *(uintptr_t**)m_class;
	m_vFuncCount = countVFuncs();
	if (m_vFuncCount <= 0)
		return false;

	m_newVtable.reset(new uintptr_t[m_vFuncCount]);

	// deep copy orig vtable into new
	memcpy(m_newVtable.get(), m_origVtable, sizeof(uintptr_t) * m_vFuncCount);

	for (const auto& p : m_redirectMap) {
		assert(p.first < m_vFuncCount);
		if (p.first >= m_vFuncCount)
			return false;

		// redirect ptr at VTable[i]
		m_origVFuncs[p.first] = (uint64_t)m_newVtable[p.first];
		m_newVtable[p.first] = (uintptr_t)p.second;
	}

	*(uint64_t**)m_class = (uint64_t*)m_newVtable.get();
	m_Hooked = true;
	return true;
}

bool PLH::VTableSwapHook::unHook() {
	assert(m_Hooked);
	if (!m_Hooked)
		return false;

	MemoryProtector prot(m_class, sizeof(void*), ProtFlag::R | ProtFlag::W);
	*(uint64_t**)m_class = (uint64_t*)m_origVtable;
	
	m_newVtable.reset();

	m_Hooked = false;
	m_origVtable = nullptr;
	return true;
}

uint16_t PLH::VTableSwapHook::countVFuncs() {
	uint16_t count = 0;
	for (;; count++) {
		// if you have more than 500 vfuncs you have a problem and i don't support you :)
		if (!IsValidPtr((void*)m_origVtable[count]) || count > 500)
			break;
	}
	return count;
}

const PLH::VFuncMap& PLH::VTableSwapHook::getOriginals() const {
	return m_origVFuncs;
}