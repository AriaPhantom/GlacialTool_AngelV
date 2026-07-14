@echo off
setlocal
chcp 65001 >nul
cd /d "%~dp0"

set "MSBUILD="
set "TOOLSET=v145"
set "SDK_VERSION=10.0.26100.0"
if exist "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" set "MSBUILD=C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe"
if not defined MSBUILD if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" (
  set "MSBUILD=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe"
  set "TOOLSET=v142"
  set "SDK_VERSION=10.0"
)
if not defined MSBUILD (
  echo ERROR: MSBuild.exe not found.
  if /I not "%CI%"=="1" pause
  endlocal
  exit /b 1
)
set "VCPKG_ROOT="
if exist "C:\vcpkg\installed" set "VCPKG_ROOT=C:\vcpkg"
if not defined VCPKG_ROOT if exist "F:\vcpkg\installed" set "VCPKG_ROOT=F:\vcpkg"
if not defined VCPKG_ROOT (
  echo ERROR: vcpkg installed tree not found under C:\vcpkg or F:\vcpkg.
  if /I not "%CI%"=="1" pause
  endlocal
  exit /b 1
)
set "MSBUILD_PROPS=/p:WindowsTargetPlatformVersion=%SDK_VERSION% /p:PlatformToolset=%TOOLSET% /p:VcpkgRoot=%VCPKG_ROOT%\ /p:VcpkgInstalledDir=%VCPKG_ROOT%\installed\"
echo ========================================
echo Building AngelV Multi-Version (NL / Angel / Adele)
echo Compile-only mode (no zip packaging)
echo ========================================

echo.
echo [1/3] Building NL...
echo #pragma once> ProjVer.h
echo #define NL>> ProjVer.h
"%MSBUILD%" Angi.sln /t:Build /p:RunPostBuildEvent=Always /p:Configuration=Release /p:Platform=x64 %MSBUILD_PROPS% /m /v:minimal
if errorlevel 1 goto build_failed
if exist "x64\Release\Angi.exe" copy /Y "x64\Release\Angi.exe" "x64\Release\Angi_NL.exe" >nul

echo.
echo [2/3] Building Angel...
echo #pragma once> ProjVer.h
echo #define Angel>> ProjVer.h
"%MSBUILD%" Angi.sln /t:Build /p:RunPostBuildEvent=Always /p:Configuration=Release /p:Platform=x64 %MSBUILD_PROPS% /m /v:minimal
if errorlevel 1 goto build_failed
if exist "x64\Release\Angi.exe" copy /Y "x64\Release\Angi.exe" "x64\Release\Angi_Angel.exe" >nul

echo.
echo [3/3] Building Adele...
echo #pragma once> ProjVer.h
echo #define Adele>> ProjVer.h
"%MSBUILD%" Angi.sln /t:Build /p:RunPostBuildEvent=Always /p:Configuration=Release /p:Platform=x64 %MSBUILD_PROPS% /m /v:minimal
if errorlevel 1 goto build_failed
if exist "x64\Release\Angi.exe" copy /Y "x64\Release\Angi.exe" "x64\Release\Angi_Adele.exe" >nul

rem Restore default to NL
echo #pragma once> ProjVer.h
echo #define NL>> ProjVer.h

echo.
echo ========================================
echo Done
echo ========================================
dir /b x64\Release\Angi_*.exe 2>nul
if /I not "%CI%"=="1" pause
endlocal
exit /b 0

:msbuild_missing
echo MSBuild not found: %MSBUILD%
endlocal
exit /b 1

:build_failed
echo Build failed.
endlocal
exit /b 1
