REM Registry properly ATCM dlls after build process
@echo off
setlocal

set KEY=HKEY_LOCAL_MACHINE\SOFTWARE\Softing\4ControlV2\2.0
set KEY_REMOVE=HKEY_LOCAL_MACHINE\SOFTWARE\Softing
set VAL=BinPath

echo.
echo Checking ATCMControl install status.
echo -------------------------------------------------------------------------
echo.

REM Check for presence of key first.
reg query %KEY% /v %VAL% >nul 2>nul || (goto notinstalled)

set KEY_VAL=
for /f "tokens=2,*" %%a in ('reg query %KEY% /v %VAL% ^| findstr %VAL%') do (
    set KEY_VAL=%%b
)

echo.
echo Installation found! Registering DLLs.
echo -------------------------------------------------------------------------
echo.

echo cg_4claccess.dll
regsvr32 /s %KEY_VAL%\cg_4claccess.dll

echo.
echo cg_iecfront.dll
regsvr32 /s %KEY_VAL%\cg_iecfront.dll

echo.
echo cg_console.dll
regsvr32 /s %KEY_VAL%\cg_console.dll

echo.
echo All DLLs registered successful.
echo -------------------------------------------------------------------------
goto end

:notinstalled
echo.
echo Registry unnecessary, ATCMControl is not installed.
echo -------------------------------------------------------------------------
echo.
goto end

:end
exit /B 0
endlocal
