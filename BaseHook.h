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
#pragma once
#include <windows.h>
/***************************************
*The basic defination of windows hook
****************************************/
class BaseHook
{
	/***************************************
	* Enum
	****************************************/
#pragma region Enum
public:
	/***************************************
	*The available types of windows hook
	****************************************/
	enum HookTypes : int
	{
		//Message filter hook - WH_MSGFILTER = -1
		MsgFilter = -1,
		//Journal record hook - WH_JOURNALRECORD = 0
		JournalRecord = 0,
		//Journal playback hook - WH_JOURNALPLAYBACK = 1
		JournalPlayback = 1,
		//Keyboard hook - WH_KEYBOARD = 2
		Keyboard = 2,
		//Get message hook - WH_GETMESSAGE = 3
		GetMessage = 3,
		//Call wnd proc hook - WH_CALLWNDPROC = 4
		CallWndProc = 4,
		//CBT hook - WH_CBT = 5
		CBT = 5,
		//System message filter hook - WH_SYSMSGFILTER = 6
		SysMsgFilter = 6,
		//Mouse hook - WH_MOUSE = 7
		Mouse = 7,
		//Hardware hook - WH_HARDWARE = 8
		Hardware = 8,
		//Debug hook - WH_DEBUG = 9
		Debug = 9,
		//Shell hook - WH_SHELL = 10
		Shell = 10,
		//Fore ground idle hook - WH_FOREGROUNDIDLE = 11
		ForeGroundIdle = 11,
		//Call wnd proc ret hook - WH_CALLWNDPROCRET = 12
		CallWndProcRet = 12,
		//Keyboard low level hook - WH_KEYBOARD_LL = 13
		KeyboardLL = 13,
		//Mouse low level hook - WH_MOUSE_LL = 14
		MouseLL = 14
	};
#pragma endregion
	/***************************************
	* Value
	****************************************/
#pragma region Value
protected:
	//The hook type
	HookTypes int_HookType;
	//The hook object thread id, give 0 for all thread
	DWORD dword_ThreadId;
	//The hook id, give 0 for not set
	HHOOK point_HookID;
	//Dll entrance
	static HINSTANCE hangle_HinstDLL;

#pragma endregion
	/***************************************
	* New
	****************************************/
#pragma region New
public:
	/***************************************
	''' <summary>
	''' Initial function
	''' </summary>
	''' <param name="HookType">The hook type</param>
	''' <param name="ThreadId">The hook object thread id, give 0 for all thread</param>
	****************************************/
	BaseHook(HookTypes HookType, DWORD ThreadId);
	//Dispose function
	~BaseHook(void);
#pragma endregion
	/***************************************
	* Property
	****************************************/
#pragma region Property
public:
	//Set / get the hook type
	inline HookTypes GetHookType();
	inline void SetHookType(HookTypes HookType);
	//Set / get the hook object thread id, give 0 for all thread
	inline DWORD GetThreadId();
	inline void SetThreadId(DWORD ThreadId);
	//Set / get whether the hook is running
	bool GetEnabled();
	void SetEnabled(bool Enabled);
	//Set / get dll hinst
	static HINSTANCE GetHinstDll();
	static void SetHinstDll(HINSTANCE HinstDLL);
#pragma endregion
	/***************************************
	* Sub / Function
	****************************************/
#pragma region Sub / Function
protected:
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
	LRESULT /*CALLBACK*/ CoreHookProcess(int nCode, WPARAM wParam, LPARAM lParam);
public:
	/***************************************
	/// <summary>
	/// Set the hook data
	/// </summary>
	/// <param name="HookType">The hook type</param>
	/// <param name="ThreadId">The hook object thread id, give 0 for all thread</param>
	/// <remarks>Restart the hook after the hook data changed</remarks>
	****************************************/
	void SetHook(HookTypes HookType, DWORD ThreadId);
	/***************************************
	/// <summary>
	/// Start the hook
	/// </summary>
	/// <remarks>Check the Enabled for operation result</remarks>
	****************************************/
	void Start();
	/***************************************
	/// <summary>
	/// Stop the hook
	/// </summary>
	/// <remarks>Check the Enabled for operation result</remarks>
	****************************************/
	void Stop();
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
	virtual LRESULT /*CALLBACK*/ HookProcess(int nCode, WPARAM wParam, LPARAM lParam);
#pragma endregion
};
