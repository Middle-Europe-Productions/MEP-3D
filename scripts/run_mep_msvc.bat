IF [%1] == [] (
    ECHO "[MEP] Missing target!" 
    goto :eof
)
IF NOT [%2] == [] (
    set mode=%2
) ELSE (
    set mode="Debug"
)
IF NOT exist %1\%1.sln (
    ECHO "[MEP] Target %1 does not exist!"
    goto :eof
)
ECHO "[MEP] Building MEP %1" 
msbuild %1\%1.sln /p:Configuration=%mode%
if errorlevel 1 (
    ECHO "[MEP] Compilation failed!"
    goto :eof
)
ECHO "[MEP] Launching MEP"
ECHO %result%
start /d "%1/" Debug\%1.exe
