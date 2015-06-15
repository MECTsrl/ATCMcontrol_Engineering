
(*[[
    UserURL= "ATCM:FA_ASYNC_CREATEDIRECTORY.htm"
]]*)

(* 
 *  ATCMControl FileAccess Functionblock FA_ASYNC_CREATEDIRECTORY
 *  Purpose: Create a new directory.
 *)
FUNCTION_BLOCK FA_ASYNC_CREATEDIRECTORY
VAR_INPUT
    EXEC        : BOOL;             (* raising edge starts operation *)
END_VAR
VAR_IN_OUT
    FILE        : FA_ASYNC_FILE;    (* file instance to operate on *)
END_VAR
VAR_OUTPUT
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
    _done.putBool( ((COM.softing.fclib.flib.AsyncFLib)_file).createDir(_exec.getBool(),
                        _ireq.getBool(),
                        _token,
                        this,
                        _error,
                        _errcd,
                        _errtxt) );
END_NATIVE;

END_FUNCTION_BLOCK
