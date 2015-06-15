
(*[[
    UserURL= "ATCM:SER_WRITE.htm"
]]*)

(* Communication Function Blocks, see IEC 1131-5 for serial communication
 * Derived from the WRITE communication function block
 * differs in SD_1 from WRITE in IEC 1131-5!
*)

FUNCTION_BLOCK SER_WRITE            (* Parametric control *)      
VAR_INPUT                                                  
    REQ    : BOOL R_EDGE := FALSE;  (* Request *)                      
    ID     : COMM_CHANNEL;          (* Communication channel *)        
    SD_1   : BYTE_STRING ;          (* value of to be set *) 
END_VAR                                                    
                                                        
VAR_OUTPUT                                                 
    DONE   : BOOL;        (* Function performed *)       
    ERROR  : BOOL;        (* New non-zero STATUS received *) 
    STATUS : DINT;        (* Last detected status *)         
END_VAR  

VAR
    hChannel : DINT;
    iType    : DINT;
    bError   : BOOL;            (* true if error occured *)
    iNewStatus : DINT;          (* new status *)
    iFbState : DINT := 0;       (* state of function block:*)
                                (*  0: INIT
                                    1: IDLE
                                    2: WAITING
                                    4: HAVE_IT
                                    10: ERROR                
                                *)
    timer    : TON;             (* timer for write timeout *)
    timer_IN : BOOL;
    timer_PT : TIME;
    bTimeout : BOOL;      
END_VAR

VAR_NATIVE
    private CommFB comm;
END_VAR
VAR_NATIVE#movetop
import COM.softing.fc.CC.commFB.*;
import COM.softing.fc.CC.utility.*;
END_VAR


(* INIT local variables*)
hChannel := ID.hChannel;
iType := ID.iCommType;
bError := FALSE;

(* Init OutVars *)
DONE := FALSE;
ERROR := FALSE;


(* Init funciton Block *)
IF iFbState = 0 THEN
    STATUS := CFB_NoError;

    (* fast initialization => goto state IDLE *)
    iFbState := 1;
END_IF;

(* get CommFB *)
NATIVE
    comm = CommFBFactory.lookUpCommFB(_hchannel.getInt(), _itype.getInt());
    if (comm == null)
    {
        _berror.putBool(true);
    }
END_NATIVE;
IF bError THEN
    iNewStatus := CFB_ErrInvalidCommChannel;
    iFbState := 10;
    GOTO error;
END_IF;

(* IDLE until new write request at raising edge of REQ input *)
IF iFbState = 1 THEN
    IF REQ THEN
        NATIVE
        // check if write is not active
        synchronized(comm) {
          if (comm.isWriteActive())
            _berror.putBool(true);
          else
            comm.setWriteActive(true);
        }
        END_NATIVE;

        IF bError THEN
            iNewStatus := CFB_ErrRemoteWrongState;
            iFbState := 10;
            GOTO error;
        END_IF;

        (* Start write request asynchronously *)
        (* reset timer *)
        bTimeout := FALSE;
        timer_IN := FALSE;
        timer_PT := DINT_TO_TIME(ID.iTimeout);
        timer(IN := timer_IN, PT := timer_PT);

        NATIVE
        // source: locVars[2])
        if (!comm.writeAsync((ByteString)_sd_1))
        {
            _berror.putBool(true);
            _inewstatus.putInt(comm.getWriteStatus()); // CFB_...
            comm.setWriteActive(false);
        }
        END_NATIVE;

        IF bError THEN
            iFBState := 10;
            GOTO error;
        END_IF;

        (* init new timer *)
        IF ID.iTimeout > 0 THEN
            timer_IN := TRUE;
        ELSE
            timer_IN := FALSE;
        END_IF;

        (* Sext state WAITING *)
        iFbState := 2;

    ELSE 
        (* Idle...*)
        RETURN;
    END_IF;
END_IF;

(* WAITING: wait for completion *)
IF iFbState = 2 THEN
    (* refresh timer *)
    timer(IN := timer_IN, PT := timer_PT);
    bTimeout := timer.Q;

    (* Wait for completion *)
    NATIVE
    if (comm.hasWriteCompleted())
    {
        // check status if error occured
        int iStat = comm.getWriteStatus();
        if (iStat == 0)
        {
            // write finished: next state HAVE IT 
            _inewstatus.putInt(0);
            _ifbstate.putInt(4);
        }
        else
        {
            // error!
            _berror.putBool(true);
            _inewstatus.putInt(iStat); // CFB_...
            comm.setWriteActive(false);
        }
    }
    else
    {
        // request pending!
        if (_btimeout.getBool())
        {
            // cancel write request
            comm.cancelWriteRequest();
            _berror.putBool(true);
            comm.setWriteActive(false);
        }
        else
        {
            // wait for next cycle...
            return;
        }
    }
    END_NATIVE;
    IF bError THEN
        IF bTimeout THEN
            iNewStatus := CFB_ErrTimeout;
        ELSE
            ; (* iNewStatus already set *)
        END_IF;
        iFBState := 10;
        GOTO error;
    END_IF;

END_IF;

(* HAVE IT *)
IF iFbState = 4 THEN
    NATIVE
    comm.setWriteActive(false);
    END_NATIVE;
    DONE := TRUE;
    ERROR := FALSE;
    STATUS := 0;
    
    iFbState := 1;
    RETURN; 
END_IF;

(* ERROR *)
error:
IF iFbState = 10 THEN
    DONE := FALSE;
    ERROR := TRUE;
    STATUS := iNewStatus;

    iFbState := 1;
    RETURN;
END_IF;
                                               
END_FUNCTION_BLOCK
