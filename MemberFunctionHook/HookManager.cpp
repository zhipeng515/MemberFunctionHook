#include "stdafx.h"
#include "HookManager.h"

HookItem::HookItem(HookTypes HookType, DWORD ThreadId, HookProcFunc HookProcFunc) :
	BaseHook(HookType, ThreadId)
{
	m_HookProcFunc = HookProcFunc;
	SetHinstDll(GetModuleHandle(NULL));
}

LRESULT /*CALLBACK*/ HookItem::HookProcess(int nCode, WPARAM wParam, LPARAM lParam)
{
	return m_HookProcFunc(nCode, wParam, lParam);
}

HookManager::HookManager()
{
}


HookManager::~HookManager()
{
	for (auto it = m_Hooks.begin(); it != m_Hooks.end(); it++)
	{
		(*it)->SetEnabled(false);
	}
	m_Hooks.clear();
}

void HookManager::AddHook(BaseHook::HookTypes HookType, DWORD ThreadId, HookProcFunc HookFunc)
{
	HookItem * pHook = new HookItem(HookType, ThreadId, HookFunc);
	pHook->SetEnabled(true);
	m_Hooks.push_back(pHook);
}

void HookManager::RemoveHook(BaseHook::HookTypes HookType, DWORD ThreadId)
{
	for (auto it = m_Hooks.begin(); it != m_Hooks.end();)
	{
		if ((*it)->GetHookType() == HookType && (*it)->GetThreadId() == ThreadId)
		{
			(*it)->SetEnabled(false);
			it = m_Hooks.erase(it);
		}
		else
		{
			it++;
		}
	}
}

