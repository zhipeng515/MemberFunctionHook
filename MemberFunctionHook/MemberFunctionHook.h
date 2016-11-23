#pragma once

#include "MemberFunctionToNormalFunction.h"
#include "BaseHook.h"
#include "HookManager.h"

#ifndef _LIB
#ifdef _DEBUG
#pragma comment (lib, "../Debug/MemberFunctionHook.lib")
#else
#pragma comment (lib, "../Release/MemberFunctionHook.lib")
#endif
#endif