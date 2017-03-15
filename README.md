# MemberFunctionHook

主要功能是SetWindowsHookEx函数中可以使用C++成员函数作为回调函数。
主要思想及实现代码来自http://blog.csdn.net/realzyc8847/article/details/6229350
我对他进行了简单的分解，将核心的成员函数转普通函数功能提取出来以便适用于更多场景。

* 此模块需要运行数据段代码，需要关闭数据执行保护 /NXCOMPAT:NO
   


例子
```C++

GamePageUi::GamePageUi()
{
	HookManager::Instance()->AddHook(BaseHook::Keyboard, GetCurrentThreadId(),
	std::bind(&GamePageUi::KeyboardHookProc, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}
LRESULT GamePageUi::KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	//屏蔽F10
	if (wParam == VK_F10)
		return 1;
	return 0;
}
```

转换功能例子
```C++
//===============================================================
//将类成员函数指针转化为一般函数指针
LRESULT(CALLBACK * PCoreHookProcess)(int nCode, WPARAM wParam, LPARAM lParam);
MEMBERFUNCTION_TO_NORMALFUNCTION(this, &BaseHook::CoreHookProcess, &PCoreHookProcess);
//===============================================================
point_HookID = SetWindowsHookEx(GetHookType(), PCoreHookProcess, hMod, GetThreadId());
```

  
  依赖
  
  * https://github.com/zhipeng515/CommonFunction.git
