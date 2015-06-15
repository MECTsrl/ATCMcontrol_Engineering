@echo off

setlocal

set strFile=%1
set strSignCode=\\sfiler\iad_4control\ISO\PM\Zertifikate\VeriSign\Nov2007\signcode.exe
set strSignCodePwd=\\sfiler\iad_4control\ISO\PM\Zertifikate\VeriSign\Nov2007\signcode-pwd.exe
set strSpc=\\sfiler\iad_4control\ISO\PM\Zertifikate\VeriSign\Nov2007\myCredentials_2007.spc
set strPvk=\\sfiler\iad_4control\ISO\PM\Zertifikate\VeriSign\Nov2007\myprivatekey_2007.pvk

set strPwd=lortnoc4
set strUrl= http://www.softing.com
set strTimeStampUrl= http://timestamp.verisign.com/scripts/timstamp.dll
	
echo Start signcode-pwd.exe
%strSignCodePwd% -m %strPwd%
	
echo Execute signcode.exe
echo %strSignCode% -spc %strSpc% -v %strPvk% -i %strUrl% -t %strTimeStampUrl% %strFile%
     %strSignCode% -spc %strSpc% -v %strPvk% -i %strUrl% -t %strTimeStampUrl% %strFile%
	
if errorlevel 0 goto end_success
	
:end_error
echo Error occured while signing
%strSignCodePwd% -t
endlocal
exit 1

:end_success
echo Signing was successfull
%strSignCodePwd% -t
endlocal

