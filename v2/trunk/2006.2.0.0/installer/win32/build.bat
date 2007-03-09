@echo OFF
set COV=1
set QTDIR=c:\qt\4.2.2\
set NSIS="%PROGRAMFILES%\NSIS"
set NSIS_OPT=
set QMAKE_CONFIG=release
if %COV%==1 (
set NSIS_OPT=%NSIS_OPT% /DCOVERAGE
set QMAKE_CONFIG=%QMAKE_CONFIG% CoverageMeter
)

set VISUALSTUDIO="%PROGRAMFILES%\Microsoft Visual Studio 8\VC\bin"
call "%PROGRAMFILES%\Microsoft Visual Studio 8\VC\bin\vcvars32.bat"
call %QTDIR%\bin\qtvars.bat vsvars
set path=%NSIS%;%path%
set path=%qtdir%\bin;%path%

pushd ..\..
qmake CONFIG+="%QMAKE_CONFIG%"
nmake qmake
cd plugins
qmake CONFIG+="%QMAKE_CONFIG%"
nmake qmake
cd bridges
qmake CONFIG+="%QMAKE_CONFIG%"
nmake qmake
cd ..\childs
qmake CONFIG+="%QMAKE_CONFIG%"
nmake qmake
cd ..\compilers
qmake CONFIG+="%QMAKE_CONFIG%"
nmake qmake
cd ..\projects
qmake CONFIG+="%QMAKE_CONFIG%"
nmake qmake
cd ..\workspace
qmake CONFIG+="%QMAKE_CONFIG%"
nmake qmake
popd

pushd ..\..
nmake clean
nmake
popd

makensis %NSIS_OPT% monkeystudio.nsi
pause

