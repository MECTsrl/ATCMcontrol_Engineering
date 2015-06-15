
(*[[
    UserURL= "ATCM:UDP_LOOKUP.htm"
]]*)


(*
 *  UDP datagram communication Function Blocks
 *  UDP_LOOKUP looks up the ip-address (abc.def.ghi.jkl) 
 *  for a hostname, this has to be used in UDP_USEND
 *  UDP_LOOKUP works asynchron because lookup can take a while
 *)

FUNCTION_BLOCK UDP_LOOKUP

VAR_INPUT
    EN_L          : Bool;               (* if ENABLE_Lookup = true, do lookup else 
                                            do nothing *)
    HOST_NAME     : WString;            (* the name of the host to look up *)
END_VAR

VAR_IN_OUT
    SOCKET_HANDLE : UDP_SOCKET_HANDLE;  (* a handle to a opened socket *)
END_VAR

VAR_OUTPUT
    READY         : Bool;               (* shows that the lookup thread has finished *)
    HOST_ADDRESS  : WString;            (* if the lookup succeeded, the address of the host *)
    ERROR         : Bool;               (* error information if an error happened *)
    ERROR_NR      : DInt;
    ERROR_STRING  : WString;
END_VAR

VAR_NATIVE#movetop
    import COM.softing.fc.CC.commFB.*;
END_VAR

 
native
    ((COM.softing.fc.CC.commFB.UDPSocketHandle)_socket_handle).lookup(
                                    _en_l.getBool(), 
                                    _host_name.getString(),
                                    _ready,
                                    _host_address,
                                    _error, 
                                    _error_nr, 
                                    _error_string);
end_native;

;
END_FUNCTION_BLOCK
