@echo off
chcp 65001 >nul
cd /d "D:\OneDrive\Software\Maple³µ\GlacialTool_AngelV"

set MSBUILD="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe"

echo ========================================
echo ±àÒë Angel ¶à°æ±¾ (Í¨¹ý ProjVer.h)
echo ========================================

:: ±àÒë NL °æ±¾
echo.
echo [1/3] ±àÒë NL °æ±¾...
echo #pragma once > ProjVer.h
echo #define NL >> ProjVer.h
%MSBUILD% Angi.sln /t:Build /p:RunPostBuildEvent=Always /p:Configuration=Release /p:Platform=x64 /m /v:minimal
if exist "x64\Release\Angi.exe" copy /Y "x64\Release\Angi.exe" "x64\Release\Angi_NL.exe"

:: ±àÒë Angel °æ±¾
echo.
echo [2/3] ±àÒë Angel °æ±¾...
echo #pragma once > ProjVer.h
echo #define Angel >> ProjVer.h
%MSBUILD% Angi.sln /t:Build /p:RunPostBuildEvent=Always /p:Configuration=Release /p:Platform=x64 /m /v:minimal
if exist "x64\Release\Angi.exe" copy /Y "x64\Release\Angi.exe" "x64\Release\Angi_Angel.exe"

:: ±àÒë Adele °æ±¾
echo.
echo [3/3] ±àÒë Adele °æ±¾...
echo #pragma once > ProjVer.h
echo #define Adele >> ProjVer.h
%MSBUILD% Angi.sln /t:Build /p:RunPostBuildEvent=Always /p:Configuration=Release /p:Platform=x64 /m /v:minimal
if exist "x64\Release\Angi.exe" copy /Y "x64\Release\Angi.exe" "x64\Release\Angi_Adele.exe"

:: »Ö¸´Ä¬ÈÏ
echo #pragma once > ProjVer.h
echo #define NL >> ProjVer.h

dir /b x64\Release\Angi_*.exe
pause

