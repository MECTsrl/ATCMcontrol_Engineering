@ECHO OFF
setlocal
@ECHO.
@ECHO Fixing DLLs in case of Previous Installation
@ECHO -------------------------------------------------------------------------

IF NOT EXIST .\cg_4claccess.dll. goto end
@ECHO Registering cg_4claccess.dll
regsvr32 /s cg_4claccess.dll
IF NOT EXIST .\cg_iecfront.dll. goto end
@ECHO Registering cg_iecfront.dll
regsvr32 /s cg_iecfront.dll
IF NOT EXIST .\cg_console.dll. goto end
@ECHO Registering cg_console.dll
regsvr32 /s cg_console.dll
:end
endlocal