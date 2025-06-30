@echo off

REM Call from /wasm, but use root CMakeLists
emcmake cmake -S .. -B ../build/web -DBUILD_WEB=ON -DCMAKE_BUILD_TYPE=Release

IF %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Configure failed.
    pause
    exit /b %ERRORLEVEL%
)

cmake --build ../build/web

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