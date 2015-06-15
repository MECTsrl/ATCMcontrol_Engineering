
(*[[
    UserURL= "ATCM:BYTE_STRING_GET.htm"
]]*)

FUNCTION BYTE_STRING_GET : BYTE
VAR_INPUT 
    BSTR  : BYTE_STRING;
    INDEX : DINT;
END_VAR

inline{((COM.softing.fc.CC.utility.ByteString)%1).getByteAt(%2)}

END_FUNCTION
