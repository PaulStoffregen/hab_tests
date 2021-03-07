@echo off
rem ##### DRIVE #####
set DRIVE=%~d0

rem ##### BASE DIR #####
set BASE_DIR=%~dp1

rem ##### PROJECT DIR #####
set PROJECT_DIR=%BASE_DIR%


rem ##### TOOLS DIR #####
set TOOLS_DIR=%DRIVE%\dvpt\tools


set CST_HOME=%DRIVE%\dvpt\tools\cst-3.3.1
set CST_BIN=%CST_HOME%\mingw32\bin
set PATH=%CST_BIN%;%PATH%

rem ##### MINGW DIR #####
::echo "Setting directory to MINGW..."
set MINGW_DIR=%TOOLS_DIR%\MinGW
set PATH=%MINGW_DIR%\bin;%PATH%

rem ##### MSYS DIR #####
::echo "Setting directory to MSYS..."
set MSYS_DIR=%TOOLS_DIR%\MinGW\msys\1.0
set PATH=%MSYS_DIR%\bin;%PATH%
set TZ=CET-1CEST

rem ##### GIT ENV #####
set GIT_HOME=%DRIVE%\dvpt\tools\git-2.25.0
set GIT_ROOT=%GIT_HOME%\bin
set PATH=%GIT_ROOT%;%PATH%

