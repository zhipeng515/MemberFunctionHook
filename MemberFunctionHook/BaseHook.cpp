/********************************************************* {COPYRIGHT-TOP} *
* RealZYC Confidential
* OCO Source Materials
*
* (C) Copyright RealZYC Corp. 2011 All Rights Reserved.
*
* The source code for this program is not published or otherwise
* divested of its trade secrets, irrespective of what has been
* deposited with the China Copyright Office.
********************************************************** {COPYRIGHT-END} */
#include "stdafx.h"
#include "BaseHook.h"
#include "MemberFunctionToNormalFunction.h"

/***************************************
*class BaseHook
****************************************/
/***************************************
* Static
****************************************/
#pragma region Static
HINSTANCE BaseHook::hangle_HinstDLL = NULL;
#pragma endregion
/***************************************
* Property
****************************************/
#pragma region Property
//Set / get the hook type
BaseHook::HookTypes BaseHook::GetHookType() { return int_HookType; }
void BaseHook::SetHookType(HookTypes HookType) { int_HookType = HookType; }
//Set / get the hook object thread id, give 0 for all thread
DWORD BaseHook::GetThreadId() { return dword_ThreadId; }
void BaseHook::SetThreadId(DWORD ThreadId) { dword_ThreadId = ThreadId; }
//Set / get whether the hook is running
bool BaseHook::GetEnabled() { return (point_HookID != NULL); }
void BaseHook::SetEnabled(bool Enabled)
{
	if (Enabled != GetEnabled())
	{
		if (Enabled)Start();
		else Stop();
	}
}
//Set / get dll hinst
HINSTANCE BaseHook::GetHinstDll()
{
	try
	{
		return (hangle_HinstDLL != NULL) ? hangle_HinstDLL :
			GetModuleHandle((LPCTSTR)"WinHook");
	}
	catch (...)
	{
		return NULL;
	}
}
void BaseHook::SetHinstDll(HINSTANCE HinstDLL) { hangle_HinstDLL = HinstDLL; }
#pragma endregion
/***************************************
* New
****************************************/
#pragma region New
/***************************************
/// <summary>
/// Initial function
/// </summary>
/// <param name="HookType">The hook type</param>
/// <param name="ThreadId">The hook object thread id, give 0 for all thread</param>
****************************************/
BaseHook::BaseHook(BaseHook::HookTypes HookType, DWORD ThreadId)
{
	point_HookID = NULL;
	SetHook(HookType, ThreadId);
}
//Dispose function
BaseHook::~BaseHook(void)
{
	SetEnabled(false);
}
#pragma endregion
/***************************************
* Sub / Function
****************************************/
#pragma region Sub / Function
/***************************************
///<summary>
/// The defination of core hook process
///</summary>
///<param name="nCode">Specifies the hook code passed to the current hook procedure. The next hook procedure uses this code to determine how to process the hook information</param>
///<param name="wParam">Specifies the wParam value passed to the current hook procedure. The meaning of this parameter depends on the type of hook associated with the current hook chain</param>
///<param name="lParam">Specifies the lParam value passed to the current hook procedure. The meaning of this parameter depends on the type of hook associated with the current hook chain</param>
///<returns>The nCode, use 0 to pass the information to next hook, others to ignore the current information</returns>
///<remarks>Use for SetWindowsHookEx</remarks>
****************************************/
LRESULT /*CALLBACK*/ BaseHook::CoreHookProcess(int nCode, WPARAM wParam, LPARAM lParam)
{
	//调用模式：如果使用__stdcall (即加入CALLBACK关键字)，
	//									那么this指针是通过push this来传送的, 非法调用时会产生堆栈的不平衡, 会在函数返回时出错
	//                       如果使用__thiscall，那么this指针是通过ecx来来传送的, 不会产生堆栈变化, 测试成功
	//参考自：http://wenku.baidu.com/view/ad102ccf05087632311212ad.html
	LRESULT Result = this->HookProcess(nCode, wParam, lParam); //此时访问时, 由于设置了this指针,成功运行
	if (Result == NULL)
		Result = CallNextHookEx(point_HookID, nCode, wParam, lParam);
	return Result; //由于注释掉CALLBACK，不会产生堆栈不平衡的问题，函数返回后无报错
}
/***************************************
/// <summary>
/// Set the hook data
/// </summary>
/// <param name="HookType">The hook type</param>
/// <param name="ThreadId">The hook object thread id, give 0 for all thread</param>
/// <remarks>Restart the hook after the hook data changed</remarks>
****************************************/
void BaseHook::SetHook(BaseHook::HookTypes HookType, DWORD ThreadId)
{
	int_HookType = HookType;
	dword_ThreadId = ThreadId;
}
/***************************************
/// <summary>
/// Start the hook
/// </summary>
/// <remarks>Check the Enabled for operation result</remarks>
****************************************/
void BaseHook::Start()
{
	try
	{
		if (GetEnabled())SetEnabled(false);
		HINSTANCE hMod = NULL;
		if (GetThreadId() == 0)
		{
			hMod = GetHinstDll();
		}
		else
		{
			if (GetCurrentThreadId() == GetThreadId())
			{
				hMod = NULL;
			}
			else hMod = GetHinstDll();
		}

		//===============================================================
		//将类成员函数指针转化为一般函数指针
		LRESULT(CALLBACK * PCoreHookProcess)(int nCode, WPARAM wParam, LPARAM lParam);
		MEMBERFUNCTION_TO_NORMALFUNCTION(this, &BaseHook::CoreHookProcess, &PCoreHookProcess);
		//===============================================================
		point_HookID = SetWindowsHookEx(GetHookType(),
			PCoreHookProcess,
			hMod,
			GetThreadId());
	}
	catch (...)
	{
		point_HookID = NULL;
	}
}
/***************************************
/// <summary>
/// Stop the hook
/// </summary>
/// <remarks>Check the Enabled for operation result</remarks>
****************************************/
void BaseHook::Stop()
{
	try
	{
		UnhookWindowsHookEx(point_HookID);
	}
	catch (...)
	{
	}
	point_HookID = NULL;
}
/***************************************
/// <summary>
/// The user defined hook process
/// </summary>
/// <param name="nCode">Specifies the hook code passed to the current hook procedure. The next hook procedure uses this code to determine how to process the hook information</param>
/// <param name="wParam">Specifies the wParam value passed to the current hook procedure. The meaning of this parameter depends on the type of hook associated with the current hook chain</param>
/// <param name="lParam">Specifies the lParam value passed to the current hook procedure. The meaning of this parameter depends on the type of hook associated with the current hook chain</param>
/// <returns>The nCode, use 0 to pass the information to next hook, others to ignore the current information</returns>
/// <remarks>Use for CoreHookProcess</remarks>
****************************************/
LRESULT /*CALLBACK*/ BaseHook::HookProcess(int nCode, WPARAM wParam, LPARAM lParam)
{
	return NULL;
}
#pragma endregion
