
(*[[
    UserURL= "ATCM:UDP_USEND.htm"
]]*)

(*
 *  UDP datagram communication Function Blocks
 *  UDP_USEND allows to send a message to a partner
 *  through an open socket
 *)

FUNCTION_BLOCK UDP_USEND

VAR_INPUT
    REQ          : BOOL;             (* true: send the msg, false: do nothing *)
    PARTNER_ADDR : WSTRING;          (* the host address of the communication partner *)
    PARTNER_PORT : DINT;             (* the port number where the comm. partner receives its msgs *)
    SD_1         : BYTE_STRING;      (* the data of the msg that will be sent *)
END_VAR

VAR_IN_OUT
    SOCKET_HANDLE : UDP_SOCKET_HANDLE; (* a structure that describes the socket *)
END_VAR

VAR_OUTPUT
    ERROR        : BOOL;             (* error information *)
    ERROR_NR     : DINT;
    ERROR_STRING : WSTRING;
END_VAR

VAR_NATIVE#movetop
    import COM.softing.fc.CC.commFB.*;
END_VAR



if (REQ) then
    native
        ((COM.softing.fc.CC.commFB.UDPSocketHandle)_socket_handle).send(
            _partner_addr.getString(), 
            _partner_port.getInt(),
            _sd_1, 
            _error,
            _error_nr, 
            _error_string);
    end_native;
end_if;


END_FUNCTION_BLOCK
