@ECHO OFF

setlocal

set CWD=%~d0%~p0 
cd /D %CWD%

call GenTarget FCTPAC020_01

:end
endlocal