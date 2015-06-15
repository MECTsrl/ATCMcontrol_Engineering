
(*[[
    UserURL= "ATCM:UDP_CONNECT.htm"
]]*)

(*
 *  UDP datagram communication Function Blocks
 *  UDP_CONNECT allows to open and close a socket
 *  this FunctionBlock should be called only once, when the
 *  socket should be opened or closed 
 *)


FUNCTION_BLOCK UDP_CONNECT

VAR_INPUT
    EN_C : BOOL;           (* true: open and connect the socket *)
                           (*   the field LocalPort of SOCKET_HANDLE has to be filled
                                with the local port number where the socket wants to
                                receive messages *)
                           (* false: close the socket *)
                           (* this FunctionBlock should be called only once, when the
                              socket should be opened or closed *)
END_VAR

VAR_IN_OUT
    SOCKET_HANDLE : UDP_SOCKET_HANDLE;  (* a structure that describes the socket *)
END_VAR

VAR_OUTPUT
    VALID        : BOOL;   (* shows if the socket is open or close *)
    ERROR        : BOOL;   (* error information *)
    ERROR_NR     : DINT;
    ERROR_STRING : WSTRING;
END_VAR




VAR
    portNr : DInt;
    bc     : BOOL;
END_VAR

VAR_NATIVE#movetop
    import COM.softing.fc.CC.commFB.*;
END_VAR


if EN_C then
    portNr := SOCKET_HANDLE.localPort;
    bc := SOCKET_HANDLE.broadcastSocket;
    native
        ((COM.softing.fc.CC.commFB.UDPSocketHandle)_socket_handle).open(
                                    _portnr.getInt(), 
                                    _bc.getBool(),
                                    _error, 
                                    _error_nr, 
                                    _error_string);
    end_native;
    VALID := not Error;
else
    native
        ((COM.softing.fc.CC.commFB.UDPSocketHandle)_socket_handle).close(
                                    _error, 
                                    _error_nr, 
                                    _error_string);
    end_native;
    VALID := false;
end_if;
;
END_FUNCTION_BLOCK
