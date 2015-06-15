
(*[[
    UserURL= "ATCM:BYTE_STRING_TO_WSTRING.htm"
]]*)

(**
 * BYTE_STRING_TO_WSTRING converts a BYTE_STRING value into
 * human-readable form.
 * non-ascii characters will be escaped like "\xDD"
 *)

FUNCTION BYTE_STRING_TO_WSTRING : WSTRING
VAR_INPUT 
    BSTR  : BYTE_STRING;
END_VAR

inline{%1.toString()}

END_FUNCTION
