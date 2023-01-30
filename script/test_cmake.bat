@echo off

:: default arguments:
::   script/test_cmake.bat msvc debug
if "%~1"=="" (
  set ConfigPreset=msvc
) else (
  set ConfigPreset=%1
)
if "%~2"=="" (
  set Config=debug
) else (
  set Config=%2
)
set "BuildPreset=%ConfigPreset%-%Config%"
set "TestPreset=test-%BuildPreset%"

set "BUILD_PATH=build/%ConfigPreset%"

pushd "%~dp0.."
if not exist "%BUILD_PATH%/compile_commands.json" (
  cmake -S . --preset "%ConfigPreset%"
)
cmake --build --preset "%BuildPreset%"
ctest --preset "%TestPreset%"

popd
