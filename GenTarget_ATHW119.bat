@ECHO OFF

setlocal

set CWD=%~d0%~p0 
cd /D %CWD%

call GenTarget ATHW119

:end
endlocal
