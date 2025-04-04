@echo off
chcp 65001 > nul
setlocal enabledelayedexpansion

set ROOT_PATH=%~dp0

for /f "usebackq delims=" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath`) do (
    set VS_PATH=%%i
)

if not defined VS_PATH (
    echo Visual studio path not found.
    exit /b 1
)

call "%VS_PATH%\Common7\Tools\VsDevCmd.bat"

set "SOLUTION_FILE=%ROOT_PATH%\ModalLoopSurprise.sln"
for %%P in (x64 Win32) do (
    for %%C in (Debug Release) do (
        set "Platform=%%P"
        set "Configuration=%%C"
        echo Building !Configuration! - !Platform!...
        MSBuild "%SOLUTION_FILE%" /p:Configuration=!Configuration! /p:Platform=!Platform!
        if errorlevel 1 (
            echo Build failed for !Configuration! - !Platform!.
            pause
            exit /b 1
        )
    )
)

echo Build Succeeded.
pause