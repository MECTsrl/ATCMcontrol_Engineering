(*[[
    UserURL= "ATCMFile:BYTE_STRING_INIT.htm"
]]*)

(**
 * BYTE_STRING_INIT initializes the byte-string to the given value.
 * C-like \-quotes are supported to specify non-ascii characters.
 * example:
 * BYTE_STRING_INIT(bstr, "abc\x00\x01");
 *)

FUNCTION BYTE_STRING_INIT
VAR_IN_OUT
    BSTR  : BYTE_STRING;
END_VAR
VAR_INPUT
    VALUE : WSTRING; (* ASCII character string! *)
END_VAR
inline{((COM.softing.fc.CC.utility.ByteString)%1).initFromString(%2)}

END_FUNCTION
