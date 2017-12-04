cd /d %~dp0 

set Platform=x64
set Configuration=Debug
set OutPutExeFile=exe
if exist %OutPutExeFile% (
    del /S /Q %OutPutExeFile%
	rd /S /Q %OutPutExeFile%
	del /S /Q JKStockTool.exe
	mkdir %OutPutExeFile%
) else ( 
    mkdir %OutPutExeFile%
) 

XCopy ..\%Platform%\%Configuration%\platforms\*.dll .\%OutPutExeFile%\platforms\
XCopy ..\%Platform%\%Configuration%\*.dll .\%OutPutExeFile%\
XCopy ..\%Platform%\%Configuration%\*.exe .\%OutPutExeFile%\
XCopy vc_redist.x64_2015.exe .\%OutPutExeFile%\

"%InnoDIR%\Compil32.exe" /cc "JKStockTool.iss"