@echo off
if "%1" == "" goto usage

if "%1" == "." (
	set TmpDir="%CD%"
) else (
	set TmpDir="%1"
)

if not exist %DDKROOT%\bin\setenv.bat goto usage

CALL %DDKROOT%\bin\setenv.bat %DDKROOT% %2 %3

cd /d %TmpDir%

build.exe -b -w -g %4 %5 %6 %7 %8

cd ..
copy2sys.bat

goto exit

:usage
echo
echo usage: MakeDrv Source_Dir w2k/wxp [free/checked]or[f/c] [build options]
echo 

:exit
@echo on