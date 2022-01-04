@echo off

set PROJECT_NAME=DLCF
set PROJECT_DIR=%~dp0

call ..\..\mk\setenv.bat %PROJECT_DIR%

%DRIVE%
cd %PROJECT_DIR%
start "%PROJECT_NAME% Build Environment" cmd
