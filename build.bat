@echo off
echo =============================
echo Building Karachi Tales...
echo =============================

g++ main.cpp game.cpp player.cpp obstacle.cpp -o game.exe -lraylib -lopengl32 -lgdi32 -lwinmm

if %errorlevel% neq 0 (
    echo.
    echo ❌ Build failed!
    pause
    exit /b
)

echo.
echo ✅ Build successful!
pause