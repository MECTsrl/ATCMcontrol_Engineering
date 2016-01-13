@ECHO OFF

setlocal

@ECHO.
@ECHO Prima.bat
@ECHO -------------------------------------------------------------------------

del .\COM\softing\fc\CC\*.*				/s /q /f		
if errorlevel 1 goto error

del .\COM\softing\fc\AddOns\Softing\4CPCTarget\4rtc\*.*				/s /q /f		
if errorlevel 1 goto error
del .\COM\softing\fc\AddOns\Softing\4CPCTarget\AddonHandler\*.*		/s /q /f		
if errorlevel 1 goto error
del .\COM\softing\fc\AddOns\Softing\4CPCTarget\ConfigDP\*.*			/s /q /f		
if errorlevel 1 goto error
del .\COM\softing\fc\AddOns\Softing\4CPCTarget\ConfigOPC\*.*		/s /q /f		
if errorlevel 1 goto error
del .\COM\softing\fc\AddOns\Softing\4CPCTarget\CSC_OnlSrv\*.cpp		/s /q /f		
if errorlevel 1 goto error
del .\COM\softing\fc\AddOns\Softing\4CPCTarget\CSC_OnlSrv\*.h		/s /q /f		
if errorlevel 1 goto error
del .\COM\softing\fc\AddOns\Softing\4CPCTarget\IO_CANopen\*.*		/s /q /f		
if errorlevel 1 goto error
del .\COM\softing\fc\AddOns\Softing\4CPCTarget\IO_DP\*.*			/s /q /f		
if errorlevel 1 goto error
del .\COM\softing\fc\AddOns\Softing\4CPCTarget\IO_ModbusIP\*.*		/s /q /f		
if errorlevel 1 goto error
del .\COM\softing\fc\AddOns\Softing\4CPCTarget\IO_OPC\*.*			/s /q /f		
if errorlevel 1 goto error
del .\COM\softing\fc\AddOns\Softing\4CPCTarget\IoManag\*.*			/s /q /f		
if errorlevel 1 goto error
del .\COM\softing\fc\AddOns\Softing\4CPCTarget\OPCutil\*.*			/s /q /f		
if errorlevel 1 goto error

del .\Target\4CWin\*.*				/s /q /f		
if errorlevel 1 goto error

del .\Tools\CDGen\*.*				/s /q /f		
if errorlevel 1 goto error

del .\TargetBase\FBConfig\*.*		/s /q /f		
if errorlevel 1 goto error

del .\TargetBase\BACnet\*.*					/s /q /f		
if errorlevel 1 goto error

goto end

del .\\*.*			/s /q /f		
if errorlevel 1 goto error





echo.
echo -------  D O N E  -------
echo.
goto end

@REM --------------------------------------------------------------------------


:error

echo.
echo -------  E R R O R  -------
echo.
echo Prima
echo.

:end

endlocal
