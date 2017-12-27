@echo off

SET BUILDTOOL=C:\Program Files (x86)\Microsoft Visual Studio 2017\Common7\IDE\devenv.com

"%BUILDTOOL%" "%cd%\DuiLib.sln" /Rebuild "Debug|x86"
if %errorlevel% NEQ 0 (
	echo "build [Debug|x86] failed!"
	pause&exit
)

"%BUILDTOOL%" "%cd%\DuiLib.sln" /Rebuild "Debug_Lib|x86"
if %errorlevel% NEQ 0 (
	echo "build [Debug_Lib|x86] failed!"
	pause&exit
)

"%BUILDTOOL%" "%cd%\DuiLib.sln" /Rebuild "Debug_MT_Lib|x86"
if %errorlevel% NEQ 0 (
	echo "build [Debug_MT_Lib|x86] failed!"
	pause&exit
)

"%BUILDTOOL%" "%cd%\DuiLib.sln" /Rebuild "Release|x86"
if %errorlevel% NEQ 0 (
	echo "build [Release|x86] failed!"
	pause&exit
)

"%BUILDTOOL%" "%cd%\DuiLib.sln" /Rebuild "Release_Lib|x86"
if %errorlevel% NEQ 0 (
	echo "build [Release_Lib|x86] failed!"
	pause&exit
)

"%BUILDTOOL%" "%cd%\DuiLib.sln" /Rebuild "Release_MT_Lib|x86"
if %errorlevel% NEQ 0 (
	echo "build [Release_MT_Lib|x86] failed!"
	pause&exit
)

"%BUILDTOOL%" "%cd%\DuiLib.sln" /Rebuild "Debug|x64"
if %errorlevel% NEQ 0 (
	echo "build [Debug|x64] failed!"
	pause&exit
)

"%BUILDTOOL%" "%cd%\DuiLib.sln" /Rebuild "Debug_Lib|x64"
if %errorlevel% NEQ 0 (
	echo "build [Debug_Lib|x64] failed!"
	pause&exit
)

"%BUILDTOOL%" "%cd%\DuiLib.sln" /Rebuild "Debug_MT_Lib|x64"
if %errorlevel% NEQ 0 (
	echo "build [Debug_MT_Lib|x64] failed!"
	pause&exit
)

"%BUILDTOOL%" "%cd%\DuiLib.sln" /Rebuild "Release|x64"
if %errorlevel% NEQ 0 (
	echo "build [Release|x64] failed!"
	pause&exit
)

"%BUILDTOOL%" "%cd%\DuiLib.sln" /Rebuild "Release_Lib|x64"
if %errorlevel% NEQ 0 (
	echo "build [Release_Lib|x64] failed!"
	pause&exit
)

"%BUILDTOOL%" "%cd%\DuiLib.sln" /Rebuild "Release_MT_Lib|x64"
if %errorlevel% NEQ 0 (
	echo "build [Release_MT_Lib|x64] failed!"
	pause&exit
)

echo "build success!!!"

rmdir /q /s package
mkdir package\include\duilib
mkdir package\bin

xcopy /e src package\include\duilib\ /exclude:exclude_src.txt
xcopy /e output package\lib\ /exclude:exclude_lib.txt
xcopy package\lib\x86\*.dll package\bin\
xcopy package\lib\x86\*.pdb package\bin\
xcopy package\lib\x64\*.dll package\bin\x64\
xcopy package\lib\x64\*.pdb package\bin\x64\
del package\lib\x86\*.dll
del package\lib\x86\*.pdb
del package\lib\x64\*.dll
del package\lib\x64\*.pdb

exit