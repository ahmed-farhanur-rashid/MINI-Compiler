@echo off
REM Change to examples folder
cd /d "%~dp0"
cd ..

cd examples

REM Create output folder if it doesn't exist
if not exist "output" mkdir "output"

REM Compile each .mini file and move outputs
for %%f in (*.mini) do (
    echo Compiling %%f
    ..\src\mini.exe %%f
    move /Y "%%~nf.c" "output\%%~nf.c" >nul
    move /Y "%%~nf.exe" "output\%%~nf.exe" >nul
)

echo All files compiled and moved to output folder.

pause