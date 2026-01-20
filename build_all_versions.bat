@echo off
chcp 65001 >nul
cd /d "D:\OneDrive\Software\Maple车\GlacialTool_Angel"

set MSBUILD="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe"

echo ========================================
echo 编译 Angel 多版本 (通过 ProjVer.h)
echo ========================================

:: 编译 NL 版本
echo.
echo [1/3] 编译 NL 版本...
echo #pragma once > ProjVer.h
echo #define NL >> ProjVer.h
%MSBUILD% Angi.sln /t:Rebuild /p:Configuration=Release /p:Platform=Win32 /m /v:minimal
if exist "Release\Angi.exe" copy /Y "Release\Angi.exe" "Release\Angi_NL.exe"

:: 编译 Angel 版本
echo.
echo [2/3] 编译 Angel 版本...
echo #pragma once > ProjVer.h
echo #define Angel >> ProjVer.h
%MSBUILD% Angi.sln /t:Rebuild /p:Configuration=Release /p:Platform=Win32 /m /v:minimal
if exist "Release\Angi.exe" copy /Y "Release\Angi.exe" "Release\Angi_Angel.exe"

:: 编译 Adele 版本
echo.
echo [3/3] 编译 Adele 版本...
echo #pragma once > ProjVer.h
echo #define Adele >> ProjVer.h
%MSBUILD% Angi.sln /t:Rebuild /p:Configuration=Release /p:Platform=Win32 /m /v:minimal
if exist "Release\Angi.exe" copy /Y "Release\Angi.exe" "Release\Angi_Adele.exe"

:: 恢复默认
echo #pragma once > ProjVer.h
echo #define NL >> ProjVer.h

dir /b Release\Angi_*.exe
pause
