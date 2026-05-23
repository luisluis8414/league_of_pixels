@echo off
setlocal

set "ROOT=%~dp0.."
set "CONFIG=%~1"
if "%CONFIG%"=="" set "CONFIG=Debug"

pushd "%ROOT%" || exit /b 1

echo Generating Visual Studio 2022 project files...
".\vendor\premake5\premake5.exe" vs2022
if errorlevel 1 goto :error

set "MSBUILD="
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if exist "%VSWHERE%" (
  for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -requires Microsoft.Component.MSBuild -find MSBuild\Current\Bin\MSBuild.exe`) do (
    set "MSBUILD=%%i"
  )
)

if "%MSBUILD%"=="" (
  for /f "tokens=*" %%i in ('where msbuild 2^>nul') do (
    set "MSBUILD=%%i"
    goto :found_msbuild
  )
)

:found_msbuild
if "%MSBUILD%"=="" (
  echo MSBuild was not found. Install Visual Studio 2022 with the Desktop development with C++ workload.
  popd
  exit /b 1
)

echo Building LeagueOfPixels %CONFIG% x64...
"%MSBUILD%" LeagueOfPixels.sln /p:Configuration=%CONFIG% /p:Platform=x64 /m
if errorlevel 1 goto :error

echo Build complete: bin\%CONFIG%\LeagueOfPixels.exe
popd
exit /b 0

:error
echo Build failed.
popd
exit /b 1
