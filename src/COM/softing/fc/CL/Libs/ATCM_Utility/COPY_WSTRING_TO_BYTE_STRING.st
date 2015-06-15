(*[[
    UserURL= "ATCMFile:COPY_WSTRING_TO_BYTE_STRING.htm"
]]*)

(* Copies the value VALUE to the byte string BSTR starting at the position POS.
 * The unicode wstring VALUE will be converted to a multibyte representation
 * according to the current code page. 
 * POS will be incremented by the actual number of multi bytes written to BSTR.
 * 
 * Note: If a unicode char cannot be converted, the result is undefined.
 *)
FUNCTION COPY_WSTRING_TO_BYTE_STRING
VAR_INPUT
    VALUE : WSTRING;
END_VAR
VAR_IN_OUT
    POS : DINT;
    BSTR : BYTE_STRING;
END_VAR
VAR
    STRLEN : DINT;
END_VAR

native
    byte multibytes[] = _value.getBytes(); 
    _strlen = multibytes.length;
end_native;

    IF (BSTR.SIZE < POS+STRLEN) THEN
        BSTR.SIZE := POS+STRLEN;
    END_IF;

native
    byte byteString[] = ((COM.softing.fc.CC.utility.ByteString)_bstr).getByteString();
	int iStartPos = _pos.getInt();

    for(int c=0; c<_strlen; c++) 
    {
        byteString[iStartPos+c] = multibytes[c];
	}
    _pos.putInt(iStartPos+_strlen);
END_NATIVE;

END_FUNCTION
