@echo off
setlocal

set KEY=HKEY_LOCAL_MACHINE\SOFTWARE\Softing\4ControlV2\2.0
set KEY_REMOVE=HKEY_LOCAL_MACHINE\SOFTWARE\Softing
set VAL=LibPath

REM Check for presence of key first.
reg query %KEY% /v %VAL% >nul 2>nul || (goto removekey)

:getpathvalue
REM query the value. pipe it through findstr in order to find the matching line that has the value. 
REM only grab token 3 and the remainder of the line. %%b is what we are interested in here.
set KEY_VAL=
for /f "tokens=2,*" %%a in ('reg query %KEY% /v %VAL% ^| findstr %VAL%') do (
    set KEY_VAL=%%b
)
rem echo %KEY_VAL%
echo ATCMControl is installed, keeping registry key!
echo.
goto end

:removekey
echo Removing ATCMControl registry key if exists!
echo.
reg delete %KEY_REMOVE% /f >nul 2>nul
goto end

:end
endlocal