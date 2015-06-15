
(*[[
    UserURL= "ATCM:FA_ASYNC_GETSIZE.htm"
]]*)

(* 
 *  ATCMControl FileAccess Functionblock FA_ASYNC_GETSIZE
 *  Purpose: Retrieves the length in bytes of a file.
 *)
FUNCTION_BLOCK FA_ASYNC_GETSIZE
VAR_INPUT
    EXEC        : BOOL;             (* raising edge starts operation *)
END_VAR
VAR_IN_OUT
    FILE        : FA_ASYNC_FILE;    (* file instance to operate on *)
END_VAR
VAR_OUTPUT
    SIZE        : LINT;             (* file size in bytes *)
    DONE        : BOOL;             (* operation finished *)
    ERROR       : BOOL;             (* error flag *)
    ERRCD       : DINT;             (* error code *)
    ERRTXT      : WSTRING;          (* error text *)
END_VAR

VAR
    rtrigExec   : R_TRIG;           (* execute *)
    token       : BOOL;             (* instance access token *)
    ireq        : BOOL;             (* execute request *)
END_VAR

rtrigExec(CLK := EXEC);
ireq := rtrigExec.Q;

NATIVE
    _done.putBool( ((COM.softing.fclib.flib.AsyncFLib)_file).getSize(_exec.getBool(),
                        _ireq.getBool(),
                        _token,
                        this,
                        (FcObject)_size,
                        _error,
                        _errcd,
                        _errtxt) );
END_NATIVE;

END_FUNCTION_BLOCK
