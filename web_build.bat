@echo off
echo =====================================
echo   NESKVIK WEB BUILD STARTING
echo =====================================


:: Clean up any previous build if you want
REM rmdir /s /q build\web

:: Configure with emcmake, turning on BUILD_WEB
emcmake cmake -S . -B build/web -DBUILD_WEB=ON -DCMAKE_BUILD_TYPE=Release

IF %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Configure failed.
    pause
    exit /b %ERRORLEVEL%
)

:: Actually build
cmake --build build/web

IF %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Build failed.
    pause
    exit /b %ERRORLEVEL%
)

echo.
echo =====================================
echo   NESKVIK WEB BUILD COMPLETE
echo =====================================
pause
