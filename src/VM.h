#pragma once

#include <functional>
#include <memory>
#include <jampio/shared/shared.h>
#include "CvarSystem.h"
#include "CommandSystem.h"

class VM {
private:
	void *m_dllHandle;
	int (QDECL *m_entryPoint)(int callNum, ...);
	std::function<int(int *parms)> m_systemCall;
	std::function<int(int/*, ...*/)> m_dllSyscallWrap;
	int QDECL DllSyscall(int arg, ...);
public:
	VM();
	~VM();
	VM(const VM&) = delete;
	VM& operator=(const VM&) = delete;
	static std::unique_ptr<VM> Create(const char *module, std::function<int(int*)> systemCall);
	template <typename... Args>
	int operator()(int callnum, Args... args) {
		return m_entryPoint(callnum, args...);
	}
	template <typename... Args>
	int call(int callnum, Args... args) {
		return m_entryPoint(callnum, args...);
	}
	static void Init(CvarSystem& cvars, CommandSystem& cmd);
};