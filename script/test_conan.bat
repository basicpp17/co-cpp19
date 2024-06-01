@echo off
setlocal enabledelayedexpansion

:: default arguments:
::   script/test_conan.bat array19
if "%~1"=="" (
  set ConanCase=array19
) else (
  set ConanCase=%1
)

set "BUILD_PATH=build/conan-%ConanCase%"
set "SOURCE_PATH=conan/tests/%ConanCase%"
set "BASE_DIR=%~dp0.."
set "ConfigPreset=conan-default"
set "BuildPreset=conan-release"
set "TestPreset=conan-release"

pushd "%~dp0.."

mkdir "%BUILD_PATH%"
cd "%BUILD_PATH%"

conan install "%BASE_DIR%/%SOURCE_PATH%" --output-folder=. %2 %3 %4 %5
if !errorlevel! neq 0 exit /b !errorlevel!

cmake "%BASE_DIR%/%SOURCE_PATH%" --preset "%ConfigPreset%"
if !errorlevel! neq 0 exit /b !errorlevel!

cmake --build --preset "%BuildPreset%"
if !errorlevel! neq 0 exit /b !errorlevel!

ctest --preset "%TestPreset%"
if !errorlevel! neq 0 exit /b !errorlevel!

popd
