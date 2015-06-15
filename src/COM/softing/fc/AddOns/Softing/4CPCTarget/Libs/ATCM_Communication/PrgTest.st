(*[[
]]*)
PROGRAM PrgTest 
var
    bstr     : byte_string;
    pos      : dint;
    valueIn  : wstring := "Umlaute: äöüßÄÖÜ andere Sachen: ´`°^";
    valueOut : wstring := "Umlaute: äöüßÄÖÜ andere Sachen: ´`°^";
    bError   : bool;
end_var

    pos := 0;
    COPY_WSTRING_TO_BYTE_STRING(valueIn, pos, bstr);

native
    try
    {
        byte byteString[] = ((COM.softing.fc.CC.utility.ByteString)_bstr).getByteString();
        java.io.FileOutputStream fi = new java.io.FileOutputStream("test.txt", false);
        fi.write(byteString);
        fi.close();
    }
    catch(java.io.IOException ioe)
    {
        _berror.putBool(true);
    }
end_native;




native
    try
    {
        byte byteString[] = ((COM.softing.fc.CC.utility.ByteString)_bstr).getByteString();
        java.io.FileInputStream fi = new java.io.FileInputStream("test.txt");
        fi.read(byteString);
        fi.close();
    }
    catch(java.io.IOException ioe)
    {
        _berror.putBool(true);
    }
end_native;

    pos := 0;
    valueOut := COPY_WSTRING_FROM_BYTE_STRING(bstr.size, pos, bstr);

END_PROGRAM
