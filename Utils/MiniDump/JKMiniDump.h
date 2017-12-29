#pragma once
#include <Windows.h>

class JKMiniDump
{
public:
	JKMiniDump(void);
	~JKMiniDump(void);

protected:
	static LONG WINAPI UnhandledExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo);
	static int GenerateMiniDump(HANDLE hFile, PEXCEPTION_POINTERS pExceptionPointers);
};

