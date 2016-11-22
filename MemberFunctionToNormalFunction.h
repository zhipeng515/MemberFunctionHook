#pragma once

// 将类的成员函数转为全局函数
// 使用内联汇编将成员函数地址动态写入数据段，并将this指针压栈
// 此模块需要运行数据段代码，需要关闭数据执行保护 /NXCOMPAT:NO
#include <vector>

template<typename dst_type, typename src_type>
static dst_type pointer_cast(src_type src)
{
	return *static_cast<dst_type*>(static_cast<void*>(&src));
}

class MemberFunctionToNormalFunction
{
public:
	template<typename Cls, typename Func>
	static MemberFunctionToNormalFunction * NewInstance(Cls * ClassInstance, Func MemberFunction)
	{
		MemberFunctionToNormalFunction * Instance = new MemberFunctionToNormalFunction(ClassInstance, MemberFunction);
		FunctionVector.push_back(Instance);
		return Instance;
	}

	template<typename Cls, typename Func>
	MemberFunctionToNormalFunction(Cls * ClassInstance, Func MemberFunction)
	{
		//******************************************//
		//初始化委托
		byte * DelegatePointer = DelegateThisCall;
		DelegateThisCall[0] = 0xb9; //__asm mov ecx, this
		DelegateThisCall[5] = 0xe9; //__asm jmp, MemberFunction
		__asm
		{
			push eax					//保护eax
			push ebx					//保护ebx
			mov eax, ClassInstance		//取得this地址
			mov ebx, DelegatePointer	//获取DelegateThisCall地址
			mov dword ptr[ebx + 1], eax	//this 地址, 双字(DWORD)
		}

		//函数指针转void*，因为继承的C++类使用比普通指针长一倍的大小保存函数地址和对this指针的修正值
		//参考：http://www.oschina.net/translate/wide-pointers
		void * MemberFunctionPtr = pointer_cast<void*>(MemberFunction);
		__asm //计算jmp地址参考:http://zhidao.baidu.com/question/105950930.html
		{
			mov eax, MemberFunctionPtr		//取得MemberFunction地址值
			mov ebx, DelegatePointer
			add ebx, 5+5					//JMP地址= MemberFunction – [(DelegatePoint+5) + 5]
			sub eax, ebx					//JMP地址=目标地址-(本句地址+本句指令字节数)
			mov dword ptr[ebx - 4], eax		//MemberFunction 地址, 双字(DWORD)
			pop ebx							//还原ebx
			pop eax							//还原eax
		}
		//******************************************//
	}

	template<typename Func>
	void ConvertTo(Func NormalFunction)
	{
		byte * DelegatePointer = DelegateThisCall;
		void * NormalFunctionPtr = pointer_cast<void*>(NormalFunction);
		__asm 
		{ 
			push eax
			push ebx
			mov eax, DelegatePointer	//获取DelegateThisCall地址
			mov ebx, NormalFunctionPtr	//获取NormalFunction地址
			mov dword ptr[ebx], eax		//将数据代码写入函数地址
			pop ebx
			pop eax
		}
	}

	byte * GetDelegate()
	{
		return DelegateThisCall;
	}

	static void ReleaseAll()
	{
		for (auto it = FunctionVector.begin(); it != FunctionVector.end(); it++)
		{
			delete *it;
		}
		FunctionVector.clear();
	}

protected:
	byte DelegateThisCall[10];
	static std::vector<MemberFunctionToNormalFunction*> FunctionVector;
};

#define MEMBERFUNCTION_TO_NORMALFUNCTION(ClassInstance, MemberFunction, NormalFunction) \
	{ \
		MemberFunctionToNormalFunction * Instance = MemberFunctionToNormalFunction::NewInstance(ClassInstance, MemberFunction); \
		Instance->ConvertTo(NormalFunction); \
	}

#define MEMBERFUNCTION_TO_NORMALFUNCTION_RELEASEALL() \
	MemberFunctionToNormalFunction::ReleaseAll();


