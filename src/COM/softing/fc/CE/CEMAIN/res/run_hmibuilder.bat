@echo off
setlocal

if not exist .\HMIBuilder\machine goto check_hmi_7z
goto open_compile_env

:check_hmi_7z
if not exist .\HMIBuilder.7z goto msg_copy_hmi_builder
goto uncompress_hmi_builder


:uncompress_hmi_builder
echo.
echo -------------------------------- INFO ---------------------------------- 
echo It's the first time that you run HMIBuilder call. 
echo Please wait while the HMIBuilder environment is being prepared.
echo This proccess may take several minutes depending of your computer.
echo -------------------------------------------------------------------------
echo.
echo Processing files .......
.\HMIBuilder\bins\Compressor\7Zip\7z.exe x .\HMIBuilder.7z -o%CD% -y > nul 2>&1
echo All file were processed
del /Q /S /F .\HMIBuilder.7z
echo.
pause
goto open_compile_env

:open_compile_env
start explorer .\HMIBuilder
goto end

:msg_copy_hmi_builder
echo.
echo -------------------------------- ALERT ---------------------------------- 
echo Please, copy the file HMIBuilder.7z to %CD%
echo for run HMI compile environment. You will find it in Installation DVD.
echo -------------------------------------------------------------------------
pause
goto end


:end
endlocal