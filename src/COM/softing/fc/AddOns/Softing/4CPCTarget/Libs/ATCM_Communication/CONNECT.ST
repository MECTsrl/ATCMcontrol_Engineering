
(*[[
    UserURL= "ATCM:CONNECT.htm"
]]*)

(* Communication Function Blocks, see IEC 1131-5
 * Implements the CONNECT communication function block
 * 
*)


FUNCTION_BLOCK CONNECT      (* Connection management *)
VAR_INPUT
	EN_C    : BOOL := FALSE;   (* enable/startup connection *)
	PARTNER : COMM_PARTNER;    (* identifies communication partner and comm. deployment *)
END_VAR
VAR_OUTPUT
	VALID  : BOOL := FALSE;    (* connection is valid/established *)
	ERROR  : BOOL := FALSE;    (* new non-zero STATUS received *)
    STATUS : DINT := CFB_NoError;   (* last detected status *)
	ID     : COMM_CHANNEL;     (* established communication channel *)
END_VAR

VAR 
    iType       : DINT;
    hChannel    : DINT;
    sParam      : WSTRING; 
    bError   : BOOL;            (* true if error occured *)
    iNewStatus : DINT;          (* new status *)
    iFbState : DINT := 0;       (* state of function block:*)
                                (*  0: INIT
                                    1: IDLE
                                    2: CONNECTING
                                    3: CHECKING
                                    4: CONNECTED
                                    5: CLOSING
                                    6: CONCLUDING
                                    7: RETRY
                                    8: ABORT
                                    10: ERROR
                                *)
END_VAR


VAR_NATIVE 
    static CommFBFactory g_commFBFact = new CommFBFactory();
    private CommFB comm = null;
    private int hChannel;
    private int iType;
    public void release()
    {
        if (comm != null)
        {
            CommFBFactory.removeCommFB(hChannel, iType);
            comm.closeConn();
            comm.release();
            comm = null;
        }
        super.release();
    }
END_VAR

VAR_NATIVE#movetop
import COM.softing.fc.CC.commFB.*;
END_VAR

(*
NATIVE
END_VAR
*)

(* Init local variables *)
hChannel := ID.hChannel;
iType := ID.iCommType;
bError := FALSE;
iNewStatus := CFB_NoError;

(* Init OutVars *)
ERROR := FALSE;

(* INIT funciton Block *)
IF iFbState = 0 THEN
    VALID := FALSE;
    STATUS := CFB_NoError;

    (* fast initialization => next fb state: idle *)
    iFbState := 1;
END_IF;

(* IDLE until new connect request *)
IF iFbState = 1 THEN
    IF EN_C THEN
        VALID := FALSE;
        ID.hChannel := -1;  (* not valid *)
        hChannel := -1;
    
        iType := PARTNER.iCommType;
        sParam := PARTNER.sParam;
        ID.iCommType := iType;
        ID.iTimeout := PARTNER.iTimeout;
        (* next state connecting *)
        iFbState := 2;
    ELSE 
        (* Idle...*)
        RETURN;
    END_IF;
END_IF;

(* CONNECTING *)
IF iFbState = 2 THEN
    IF EN_C THEN
        (* Open connection *)
        NATIVE
        //if there is an old commFB, KILL IT NOW!
        if (comm != null)
        {
            CommFBFactory.removeCommFB(hChannel, iType);
            comm.closeConn();
            comm.release();
            comm = null;
        }
        // create new CommFB
        comm = CommFBFactory.createCommFB(iType = _itype.getInt());
        if (comm == null)
        {
            _berror.putBool(true);
            _inewstatus.putInt(-4); // CFB_ErrOutOfMemory
        }
        else
        {
            // open connection
            if (comm.openConn(_sparam.getString()))
            {
                hChannel = CommFBFactory.addCommFBToMap(comm);
                _hchannel.putInt(hChannel);
            }
            else
            {
                _berror.putBool(true);
                _inewstatus.putInt(comm.getConnStatus()); // CFB_...
                comm = null;
            }
        }
        END_NATIVE;

        IF bError THEN
            (* next state error *)
            iFbState := 10;
            GOTO error;
        ELSE
            (* next state checking *)
            iFbState := 3;
        END_IF;

    ELSE (* EN_C = FALSE *)
        (* Abort => next state idle *)
        iFbState := 1;
        RETURN;
    END_IF;
END_IF;

(* CHECKING *)
IF iFbState = 3 THEN
    NATIVE
    if (comm == null)
    {
        _berror.putBool(true);
        _inewstatus.putInt(7);  // CFB_ErrRemoteWrongState
        // next state error
        _ifbstate.putInt(10);
    }
    else if (!comm.checkConn())
    {
        _berror.putBool(true);
        _inewstatus.putInt(1);  // CFB_ErrLowLayer
        // next state abort
        _ifbstate.putInt(8);
    }
    END_NATIVE;

    IF bError = FALSE THEN
        ID.hChannel := hChannel;
        VALID := TRUE;
        (* next state connected *)
        iFbState := 4;
        (* wait for next cycle... *)
        RETURN;
    END_IF;
END_IF;

(* CONNECTED *)
IF iFbState = 4 THEN
    IF EN_C = FALSE THEN
        (* next state closing *)
        iFbState := 5;
    ELSE
        NATIVE
        if (comm == null)
            _berror.putBool(true);
        END_NATIVE;

        IF bError THEN
            iNewStatus := CFB_ErrRemoteWrongState;
            (* next state error *)
            iFbState := 10;
            GOTO error;
        ELSE
            (* check if connection is closed by the remote station *)
            NATIVE
            if (comm.isRemoteClosing())
            {
                // next state concluding
                _ifbstate.putInt(6);
                // wait for next cycle...
                return;
            }
            END_NATIVE;
    
            (* check if connection is still alive *)
            NATIVE
            if (!comm.checkConn())
            {
                _berror.putBool(true);
                _inewstatus.putInt(1); // CFB_ErrLowLayer
                // next state retry
                _ifbstate.putInt(7);
            }
            END_NATIVE;
        END_IF;
    END_IF;
END_IF;

(* CLOSING *)
IF iFbState = 5 THEN
    NATIVE
    if (comm != null)
    {
        CommFBFactory.removeCommFB(hChannel, iType);
        comm.closeConn();
        comm.release();
        comm = null;
    }
    END_NATIVE;

    VALID := FALSE;
    ID.hChannel := -1;
    (* next state idle *)
    iFbState := 1;
END_IF;


(* CONCLUDING *)
IF iFbState = 6 THEN
    NATIVE
    if (comm != null)
    {
        CommFBFactory.removeCommFB(hChannel, iType);
        comm.closeConn();
        comm.release();
        comm = null;
    }
    END_NATIVE;

    VALID := FALSE;
    ID.hChannel := -1;
    STATUS := CFB_ErrResetReceived;
    (* next state idle *)
    iFbState := 1;
END_IF;

(* RETRY *)
IF iFbState = 7 THEN
    NATIVE
    if (comm != null)
    {
        CommFBFactory.removeCommFB(hChannel, iType);
        comm.closeConn();
        comm.release();
        comm = null;
    }
    END_NATIVE;
    (* next state error *)
    iFbState := 10;
    GOTO error;
END_IF;

(* ABORT *)
IF iFbState = 8 THEN
    NATIVE
    if (comm != null)
    {
        CommFBFactory.removeCommFB(hChannel, iType);
        comm.closeConn();
        comm.release();
        comm = null;
    }
    END_NATIVE;
    (* next state error *)
    iFbState := 10;
    GOTO error;
END_IF;

(* ERROR *)
error:
IF iFbState = 10 THEN
    NATIVE
    if (comm != null)
    {
        CommFBFactory.removeCommFB(hChannel, iType);
        comm.closeConn();
        comm.release();
        comm = null;
    }
    END_NATIVE;

    VALID := FALSE;
    ERROR := TRUE;
    STATUS := iNewStatus;

    iFbState := 1;
    RETURN;
END_IF;   

END_FUNCTION_BLOCK

