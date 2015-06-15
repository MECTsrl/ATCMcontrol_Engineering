
(*[[
    UserURL= "ATCM:SER_READ.htm"
]]*)

(* Communication Function Blocks, see IEC 1131-5 for serial communication
 * Derived from the READ communication function block
 * differs in RD_LEN1, resp. RD_1 from READ in IEC 1131-5!
*)

FUNCTION_BLOCK SER_READ     (* Polled data acquisition *)      
VAR_INPUT                                                  
    REQ      : BOOL R_EDGE := FALSE;    (* Request *)                      
    ID       : COMM_CHANNEL;    (* Communication channel *)        
    RD_LEN_1 : DINT;            (* Min. length of the data to be received *)      
    READ_ALL : BOOL := FALSE;   (* TRUE: all available data will be read, at least RD_LEN_1 *) 
END_VAR                                                    
                                                        
VAR_OUTPUT                                                 
    NDR    : BOOL;              (* New user data received *)       
    ERROR  : BOOL;              (* New non-zero STATUS received *) 
    STATUS : DINT;              (* Last detected status *)         
    RD_1   : BYTE_STRING;       (* Received user data *)
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
                                    3: CHECKING
                                    4: HAVE_DATA
                                    10: ERROR                
                                *)
    timer    : TON;             (* timer for read timeout *)
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



(* Init local variables*)
hChannel := ID.hChannel;
iType := ID.iCommType;
bError := FALSE;

(* Init OutVars *)
NDR := FALSE;
ERROR := FALSE;

(* INIT funciton Block *)
IF iFbState = 0 THEN
    STATUS := CFB_NoError;

    (* fast initialization => goto next state IDLE*)
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

(* IDLE until new read request at raising edge of REQ input *)
IF iFbState = 1 THEN
    IF REQ THEN
        NATIVE
        // check if read is not active
        synchronized(comm) {
          if (comm.isReadActive())
            _berror.putBool(true);
          else
            comm.setReadActive(true);
        }
        END_NATIVE;

        IF bError THEN
            iNewStatus := CFB_ErrRemoteWrongState;
            iFbState := 10;
            GOTO error;
        END_IF;
        (* Start read request asynchronously *)
        (* reset timer *)
        bTimeout := FALSE;
        timer_IN := FALSE;
        timer_PT := DINT_TO_TIME(ID.iTimeout);
        timer(IN := timer_IN, PT := timer_PT);

        NATIVE
        // start read request
        if (!comm.readAsync(_rd_len_1, _read_all, (ByteString)_rd_1)) 
        {
            _berror.putBool(true);
            _inewstatus.putInt(comm.getReadStatus()); // CFB_...
            comm.setReadActive(false);
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

        (* Next state WAITING *)
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

    (* Wait for data *)
    NATIVE
    if (comm.hasReadCompleted())
    {
        // check status if error occured
        int iStat = comm.getReadStatus();
        if (iStat == 0)
        {
            // read finished : next state CHECKING
            _inewstatus.putInt(0);
            _ifbstate.putInt(3);
        }
        else
        {
            // error! 
            _berror.putBool(true);
            _inewstatus.putInt(iStat); // CFB_...
            comm.setReadActive(false);
        }
    }
    else
    {
        // request pending!
        if (_btimeout.getBool())
        {
            // cancel read request
            comm.cancelReadRequest();
            _berror.putBool(true);
            comm.setReadActive(false);
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

(* CHECKING data type match *)
IF iFbState = 3 THEN
    NATIVE
    if (!comm.verifyData())
    {
        _berror.putBool(true);
        comm.setReadActive(false);
    }
    END_NATIVE;
    IF bError THEN
        iNewStatus := CFB_ErrTypeMismatch;
        iFBState := 10;
        GOTO error;
    ELSE
        (* next state: HAVE DATA *)
        iFBState := 4;
    END_IF;
END_IF;

(* HAVE DATA *)
IF iFbState = 4 THEN
    (* copy data to RD_1 *)
    NATIVE
    comm.depositData();
    comm.setReadActive(false);
    END_NATIVE;

    NDR := TRUE;
    ERROR := FALSE;
    STATUS := 0;
    
    (* next state: IDLE *)
    iFbState := 1;
    RETURN; 
END_IF;

(* ERROR *)
error:
IF iFbState = 10 THEN
    NDR := FALSE;
    ERROR := TRUE;
    STATUS := iNewStatus;

    iFbState := 1;
    RETURN;
END_IF;

END_FUNCTION_BLOCK
