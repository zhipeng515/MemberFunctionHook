#pragma once

#include <functional>
#include <vector>
#include "../../CommonFunction/Singleton.h"
#include "BaseHook.h"

typedef std::function<LRESULT(int nCode, WPARAM wParam, LPARAM lParam)> HookProcFunc;

class HookItem : public BaseHook
{
public:
	HookItem(HookTypes HookType, DWORD ThreadId, HookProcFunc HookProcFunc);
	virtual LRESULT /*CALLBACK*/ HookProcess(int nCode, WPARAM wParam, LPARAM lParam);

private:
	HookProcFunc m_HookProcFunc;
};

class HookManager : public Singleton<HookManager>
{
public:
	HookManager();
	~HookManager();

	void AddHook(BaseHook::HookTypes HookType, DWORD ThreadId, HookProcFunc HookProcFunc);
	void RemoveHook(BaseHook::HookTypes HookType, DWORD ThreadId);

private:
	std::vector<BaseHook*> m_Hooks;
};
