(*
    Purpose: Gets the computername of the local PC.
*)
FUNCTION GetComputerName : WSTRING
NATIVE

    try{
        java.net.InetAddress  iaddr = java.net.InetAddress.getLocalHost();
        _getcomputername = iaddr.toString() ;//yields myPC1[.dhcp.softing.com]/129.26.193.3
        //if WinNT: strip .dhcp.softing.com (win2K does not need ??..)
        int idx = Math.min(_getcomputername.lastIndexOf('/'), _getcomputername.indexOf('.'));
        if(idx>0)
            _getcomputername = _getcomputername.substring(0, idx);
    }catch(java.net.UnknownHostException e)
    {
        _getcomputername = e.toString();
    }

END_NATIVE;

END_FUNCTION
