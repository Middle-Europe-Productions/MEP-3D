@echo off

set target=""
set mode=Debug

:INPUT_LOOP
if "%~1" neq "" (
    IF "%1"=="--target" (
        SHIFT
        if "%2" neq "" (
            set target=%2
            SHIFT
        )
        goto :INPUT_LOOP
    )
    IF "%1"=="-t" (
        SHIFT
        if "%2" neq "" (
            set target=%2
            SHIFT
        )
        goto :INPUT_LOOP
    )
    IF "%1"=="--mode" (
        SHIFT
        if "%2" neq "" (
            set mode=%2
            SHIFT
        )
        goto :INPUT_LOOP
    )
    IF "%1"=="-m" (
        SHIFT
        if "%2" neq "" (
            set mode=%2
            SHIFT
        )
        goto :INPUT_LOOP
    )
)

IF %target% equ "" (
    ECHO "[MEP] Missing target!" 
    goto :eof
)

IF NOT exist %target%\%target%.sln (
    ECHO "[MEP] Target %1%target% does not exist!"
    goto :eof
)
ECHO "[MEP] Building MEP %target% with mode %mode%" 
msbuild %target%\%target%.sln /p:Configuration=%mode%
if errorlevel 1 (
    ECHO "[MEP] Compilation failed!"
    goto :eof
)
ECHO "[MEP] Launching MEP %target% with mode %mode%"
start /d "%target%/" %mode%\%target%.exe %*
