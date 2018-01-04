#include "stdafx.h"
#include "JKMiniDump.h"
#include <DbgHelp.h>
#include <strsafe.h>

JKMiniDump::JKMiniDump(void)
{
	SetErrorMode(SEM_FAILCRITICALERRORS);
	SetUnhandledExceptionFilter(&JKMiniDump::UnhandledExceptionFilter);  
}


JKMiniDump::~JKMiniDump(void)
{
}


LONG WINAPI JKMiniDump::UnhandledExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo)
{
	if(IsDebuggerPresent()) 
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}

	return GenerateMiniDump(NULL, lpExceptionInfo);
}

int JKMiniDump::GenerateMiniDump(HANDLE hFile, PEXCEPTION_POINTERS pExceptionPointers)
{
	char szErrorMsg[MAX_PATH];
	StringCchPrintfA(szErrorMsg, MAX_PATH, "An unexpected error has occured: \n\nFETAL ERROR!\n\nException: 0x%08x at 0x%p\n\nPressing OK will terminate the application and save the helpful debugging information that may help us resolve this issue in the future.", pExceptionPointers->ExceptionRecord->ExceptionCode, pExceptionPointers->ExceptionRecord->ExceptionAddress);
	if (IDOK == MessageBoxA(NULL, szErrorMsg, "Fetal Error!", MB_OK | MB_ICONERROR))
	{

		BOOL bOwnDumpFile = FALSE;
		HANDLE hDumpFile = hFile;    
		MINIDUMP_EXCEPTION_INFORMATION ExpParam;

		typedef BOOL (WINAPI * MiniDumpWriteDump)( 
			HANDLE,
			DWORD ,
			HANDLE ,
			MINIDUMP_TYPE ,
			PMINIDUMP_EXCEPTION_INFORMATION ,
			PMINIDUMP_USER_STREAM_INFORMATION ,
			PMINIDUMP_CALLBACK_INFORMATION
			);

		MiniDumpWriteDump pfnMiniDumpWriteDump = NULL;
		HMODULE hDbgHelp = LoadLibraryA("DbgHelp.dll");
		if (hDbgHelp)
			pfnMiniDumpWriteDump = (MiniDumpWriteDump)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
		else
			MessageBoxA(NULL, "Error", "File 'DbgHelp.dll' failed to load", MB_OK | MB_ICONERROR);

		if (pfnMiniDumpWriteDump)
		{
			if (hDumpFile == NULL || hDumpFile == INVALID_HANDLE_VALUE)
			{
				char szPath[MAX_PATH] = {0};
				GetModuleFileNameA(NULL, szPath, MAX_PATH);  
				char* pChar = strrchr(szPath, '\\');  
				if (NULL != pChar)  
				{  
					int iPos = pChar - szPath;  
					szPath[iPos + 1] = '\0';  
				} 

				char szFileName[MAX_PATH] = {0};
				const char* szAppName = "JKStockToll Crash Report-"; 
				SYSTEMTIME stLocalTime; 
				GetLocalTime(&stLocalTime); 

				StringCchPrintfA(szFileName, MAX_PATH, "%s%s-%04d%02d%02d-%02d%02d%02d.dmp", szPath, szAppName,
					stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay, 
					stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond); 

				hDumpFile = CreateFileA(szFileName, GENERIC_READ|GENERIC_WRITE, 
					FILE_SHARE_WRITE|FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

				bOwnDumpFile = TRUE;
			}

			if (hDumpFile!=INVALID_HANDLE_VALUE)
			{
				ExpParam.ThreadId = GetCurrentThreadId(); 
				ExpParam.ExceptionPointers = pExceptionPointers; 
				ExpParam.ClientPointers = FALSE;

				pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), 
					hDumpFile, MiniDumpWithDataSegs, (pExceptionPointers ? &ExpParam : NULL), NULL, NULL);

				if (bOwnDumpFile)
					CloseHandle(hDumpFile);
			}
		}        

		if (hDbgHelp!=NULL)
			FreeLibrary(hDbgHelp);

		return EXCEPTION_EXECUTE_HANDLER; 
	}
	else
		return EXCEPTION_CONTINUE_EXECUTION; 
}