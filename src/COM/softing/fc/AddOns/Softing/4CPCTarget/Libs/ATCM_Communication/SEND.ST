
(*[[
]]*)

(* Communication Function Blocks, see IEC 1131-5
 * Implements the SEND communication function block
 * 
*)

NATIVE
import COM.softing.fc.CC.commFB.*;
END_NATIVE

FUNCTION_BLOCK SEND     (* Interlocked control, requester*)
VAR_INPUT                                                  
    REQ    : BOOL (*R_EDGE*);       (* Request to send with raising edge*)              
    R      : BOOL (*R_EDGE*);       (* Reset local, remote function *) 
    ID     : COMM_CHANNEL;      (* Communication channel *)        
    R_ID   : WSTRING;           (* Remote function block *)        
    SD_1   : WSTRING;  (*ANY*)  (* User data to send *)            
END_VAR                                                    
                                                        
VAR_OUTPUT                                                 
    NDR    : BOOL;          (* New user data received *)       
    ERROR  : BOOL;        (* New non-zero STATUS received *) 
    STATUS : DINT;        (* Last detected status *)         
(*    RD_1   : ANY;*)         (* Received user data *)           
END_VAR                 (*   either m, n, or both may be 0*)

    (* is not implemented yet !! *)
    ERROR := TRUE;
    STATUS := CFB_ErrNotImpl;

NATIVE
END_NATIVE;

END_FUNCTION_BLOCK
