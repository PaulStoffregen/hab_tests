@echo off

set PROJECT_NAME=FBL

call ..\..\mk\setenv.bat

%DRIVE%
cd %PROJECT_DIR%
start "%PROJECT_NAME% Build Environment" cmd
