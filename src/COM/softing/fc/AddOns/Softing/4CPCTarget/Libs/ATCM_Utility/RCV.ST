
(* Communication Function Blocks, see IEC 1131-5
 * Implements the RCV communication function block
 * 
*)
FUNCTION_BLOCK RCV      (* Interlocked control, responder*)
VAR_INPUT                                                  
    EN_R   : BOOL;          (* Enable to receive data *)
    RESP   : BOOL (*R_EDGE*);   (* Request to send response data *)
    ID     : COMM_CHANNEL;  (* Communication channel *)
    R_ID   : WSTRING;       (* Remote function block *)
    EXPECT : DINT;          (* non-IEC: number of bytes to receive *)
(*    SD_1   : ANY;*)           (* User data to send *)
(*    SD_m   : ANY; *)
END_VAR                                                    
                                                        
VAR_OUTPUT                                                 
NDR    : BOOL;              (* New user data received *)       
ERROR  : BOOL;              (* New non-zero STATUS received *) 
STATUS : DINT;              (* Last detected status *)         

RD_1   : WSTRING; (*ANY*)       (* Received user data *)           
(*RD_n   : ANY;*)         (*  It is not required that m=n *) 
END_VAR(*  either m, n, or both may be 0*)

VAR_NATIVE#movetop
import COM.softing.fc.CC.commFB.*;
END_VAR


    (* is not implemented yet !! *)
    ERROR := TRUE;
    STATUS := CFB_ErrNotImpl;

NATIVE
END_NATIVE;

END_FUNCTION_BLOCK
