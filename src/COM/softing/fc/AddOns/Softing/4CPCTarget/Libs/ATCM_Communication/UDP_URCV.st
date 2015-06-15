
(*[[
    UserURL= "ATCM:UDP_URCV.htm"
]]*)

(*
 *  UDP datagram communication Function Blocks
 *  UDP_URCV allows to receive a message from a partner
 *  through an open socket, URCV looks if there is a new msg
 *  available. If yes it returns the msg data, else NDR shows 
 *  that no new msg is available
 *)

FUNCTION_BLOCK UDP_URCV

VAR_INPUT
    EN_R     : BOOL;            (* true: look for a new msg, false: do nothing *)
    DATA_LEN : DINT;            (* the maximum number of bytes that want to be received *)
                                (*  udp is a datagram based protocol, that means the
                                    data will be received in packets. If the available packet is 
                                    longer than DATA_LEN, only DATA_LEN bytes will be returend,
                                    the rest is lost *)
END_VAR

VAR_IN_OUT
    SOCKET_HANDLE : UDP_SOCKET_HANDLE;  (* a structure that describes the socket *)
END_VAR

VAR_OUTPUT
    RD_1         : BYTE_STRING;  (* contains the msg data, if a new msg has been received *)
    PARTNER_ADDR : WSTRING;      (* the address and port of the communication partner *)
    PARTNER_PORT : DINT;         (*    that sent the received msg *)
    NDR          : BOOL;         (* shows if a new msg has been received *)
    ERROR        : BOOL;         (* error information *)
    ERROR_NR     : DINT;
    ERROR_STRING : WSTRING;
END_VAR

VAR_NATIVE#movetop
    import COM.softing.fc.CC.commFB.*;
END_VAR


if (EN_R) then
    native
        ((COM.softing.fc.CC.commFB.UDPSocketHandle)_socket_handle).receive(
                _data_len.getInt(), 
                _rd_1,
                _partner_addr,
                _partner_port, 
                _error,
                _error_nr,
                _error_string);
    end_native;
    if (RD_1.size>0) then
        NDR := true;
    else
        NDR := false;
    end_if;
end_if;
;
END_FUNCTION_BLOCK
