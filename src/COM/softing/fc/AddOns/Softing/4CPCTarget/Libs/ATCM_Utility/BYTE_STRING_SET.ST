
(*[[
    UserURL= "ATCM:BYTE_STRING_SET.htm"
]]*)

FUNCTION BYTE_STRING_SET
VAR_IN_OUT 
    BSTR  : BYTE_STRING; 
END_VAR
VAR_INPUT 
    INDEX : DINT;
    VALUE : BYTE;
END_VAR

inline{((COM.softing.fc.CC.utility.ByteString)%1).setByteAt(%2,%3)}

END_FUNCTION
