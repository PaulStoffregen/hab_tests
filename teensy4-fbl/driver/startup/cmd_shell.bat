@echo off

set PROJECT_NAME=Startup

call ..\..\mk\setenv.bat

%DRIVE%:
cd %PROJECT_DIR%
start "%PROJECT_NAME% Build Environment" cmd
