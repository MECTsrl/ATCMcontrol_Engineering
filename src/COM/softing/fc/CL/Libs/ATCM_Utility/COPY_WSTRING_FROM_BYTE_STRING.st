(*[[
    UserURL= "ATCMFile:COPY_WSTRING_FROM_BYTE_STRING.htm"
]]*)

(* Returns a wstring copied from the byte string BSTR starting 
 * at the position POS containing NUM bytes.
 * The NUM bytes are converted using the current codepage.
 *)
FUNCTION COPY_WSTRING_FROM_BYTE_STRING : WSTRING
VAR_INPUT
    NUM : DINT;
END_VAR
VAR_IN_OUT
    POS : DINT;
    BSTR : BYTE_STRING;
END_VAR

    COPY_WSTRING_FROM_BYTE_STRING := "";
    IF (BSTR.SIZE < POS+NUM) THEN 
        POS := -1;
    ELSE 
        NATIVE
            byte byteString[] = ((COM.softing.fc.CC.utility.ByteString)_bstr).getByteString();
            _copy_wstring_from_byte_string = new String(byteString, _pos.getInt(), _num);
            _pos.putInt(_pos.getInt()+_num);
        END_NATIVE;
    END_IF;

END_FUNCTION
