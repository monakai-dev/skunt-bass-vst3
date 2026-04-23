@echo off
REM Build SKUNT BASS VST3 on Windows
REM Requires: Visual Studio 2022 with C++ workload, CMake 3.22+

echo ==========================================
echo SKUNT BASS - Windows Build Script
echo ==========================================

REM Check for CMake
cmake --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: CMake not found. Install from https://cmake.org/download/
    exit /b 1
)

REM Check for Visual Studio
where cl >nul 2>&1
if errorlevel 1 (
    echo Setting up Visual Studio environment...
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    if errorlevel 1 (
        echo ERROR: Visual Studio 2022 not found. Install from https://visualstudio.microsoft.com/
        exit /b 1
    )
)

REM Create build directory
if not exist build mkdir build
cd build

REM Generate project
echo Generating Visual Studio project...
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 (
    echo ERROR: CMake configuration failed
    exit /b 1
)

REM Build
echo Building SKUNT BASS VST3...
cmake --build . --target SkuntBass_VST3 --config Release --parallel
if errorlevel 1 (
    echo ERROR: Build failed
    exit /b 1
)

REM Package
echo Packaging...
cd SkuntBass_artefacts\Release\VST3
if exist "SKUNT BASS.vst3" (
    echo Build complete!
    echo Location: %CD%\SKUNT BASS.vst3
    echo.
    echo Install to:
    echo   %%LOCALAPPDATA%%\Programs\Common\VST3\  (user)
    echo   C:\Program Files\Common Files\VST3\      (system)
) else (
    echo ERROR: VST3 bundle not found
    exit /b 1
)

cd ..
cd ..
cd ..
