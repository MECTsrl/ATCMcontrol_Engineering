package COM.softing.fc.CC.commFB;

import COM.softing.fc.CC.system.StructVar;
import COM.softing.fc.CC.io.*;
import COM.softing.fc.CC.util.*;
import COM.softing.fc.CC.utility.*;




public class UDPSocketHandle extends StructVar
{
   private IODatagram       socket;
   private byte             sdata[];
   private byte             rdata[];
   private boolean          initialized = false;



   // some constants also defined in library
   // for use in 4C
   // command consts
   private final int UDP_CMD_OPEN  = 0;
   private final int UDP_CMD_CLOSE = 1;
   private final int UDP_CMD_SEND  = 2;
   private final int UDP_CMD_RCV   = 3;
   private final int UDP_CMD_LOOKUP = 4;
    
   // error consts (these constants are declared again in serialPeer.cpp (DLL)
   //               an in ATCM Communication library)
   private final int UDP_ERR_SOCK_INIT_FAILED  = -201;
   private final int UDP_ERR_SOCK_NOT_OPENED   = -202;
   private final int UDP_ERR_UNKNOWNHOST       = -203;
   private final int UDP_ERR_SEND              = -204;
   private final int UDP_ERR_RECV              = -205;
   private final int UDP_ERR_RECV_MSG_LONGER   = -206;
   private final int UDP_ERR_SEND_MSG_TOO_LONG = -207;
   private final int UDP_ERR_SEND_HOST_UNREACH = -208;
   private final int UDP_ERR_LOOKUP_RUNNING    = -209;
   private final int UDP_ERR_LOOKUP_HN_CHANGE  = -210;
   private final int UDP_ERR_MULTCAST_PARAM    = -211;
   private final int UDP_ERR_PORTNR_OUTOFRANGE = -212;
   
   private final String UDP_ERR_STRING_SOCK_INIT_FAILED = "udp socket communication not initialized";
   private final String UDP_ERR_STRING_SOCK_NOT_OPENED  = "udp socket not open";
   private final String UDP_ERR_STRING_UNKNOWN_HOST     = "unknown host";
   private final String UDP_ERR_STRING_SEND             = "udp send IO error";
   private final String UDP_ERR_STRING_RECV             = "udp receive IO error";
   private final String UDP_ERR_STRING_RECV_MSG_LONGER  = "received packet truncated, because it was been longer than expected";
   private final String UDP_ERR_STRING_SEND_MSG_TOO_LONG= "message too long";
   private final String UDP_ERR_STRING_SEND_HOST_UNREACH= "partner host unreachable";
   private final String UDP_ERR_STRING_LOOKUP_RUNNING   = "address lookup is still running";
   private final String UDP_ERR_STRING_LOOKUP_HN_CHANGE = "host name changed since last lookup";
   private final String UDP_ERR_STRING_MULTCAST_PARAM   = "invalid multicast paramter";
   private final String UDP_ERR_STRING_PORTNR_OUTOFRANGE = "port nr out of range";

   
   //  init (overloaded from ObjectDirectory) 
   public boolean init(ObjectDescriptor objDescr, 
                       FcObject         parent, 
                       ObjectDirectory  oldObj)
   {
       if (oldObj!=null && oldObj!=this) 
       {
           // be shure that all open sockets get closed
           oldObj.release();
           oldObj=null;
       }
       return super.init(objDescr, parent, oldObj);
   }
   
 

    //  release (overloaded from functionBlock)
    public void release()
    {
        super.release();
        // close an open socket
        closeSock();
    }


 
    public void assignFrom(ObjectDirectoryEx other)
    { 
        super.assignFrom(other); 
        socket = ((UDPSocketHandle)other).socket;
        initialized = ((UDPSocketHandle)other).initialized;
    }



    //  open UDP connection 
    public synchronized void open(int              portnr,
                                  boolean          broadcast,
                                  /*boolean          multcast,*/
                                  FcBool           error,
                                  FcInt            errornr,
                                  FcString         errorstring )
    {
        //////////////// get variables /////////////////////
        FcInt            _localportnr       = (FcInt)locVars[0];
        FcString         _localaddr         = (FcString)locVars[1];
        FcInt            _lastpartnerportnr = (FcInt)locVars[2];
        FcString         _lastpartneraddr   = (FcString)locVars[3];
        FcInt            _lastcmd           = (FcInt)locVars[4];
        FcBool           _lasterror         = (FcBool)locVars[5];
        FcInt            _lasterrornr       = (FcInt)locVars[6];
        FcString         _lasterrorstring   = (FcString)locVars[7];
        /*FcString         _multicastaddr     = (FcString)locVars[10];
        FcInt            _multicastttl      = (FcInt)locVars[11];*/

        int err = 0;
       
        // reset errors
        error.putBool(false);
        errornr.putInt(0);
        errorstring.putString("");
        _lasterror.putBool(false);
        _lasterrornr.putInt(0);
        _lasterrorstring.putString("");

        _localportnr.putInt(0);
        _localaddr.putString("");
        _lastpartnerportnr.putInt(0);
        _lastpartneraddr.putString("");

        // initialize last cmd
        _lastcmd.putInt(UDP_CMD_OPEN);

        if (initialized)
        {
            closeSock();
            initialized = false;
        }
        
        if (portnr<0 || portnr>65535)
        {
            error.putBool(true);
            errornr.putInt(UDP_ERR_PORTNR_OUTOFRANGE);
            errorstring.putString(UDP_ERR_STRING_PORTNR_OUTOFRANGE);
            _lasterror.putBool(true);
            _lasterrornr.putInt(UDP_ERR_PORTNR_OUTOFRANGE);
            _lasterrorstring.putString(UDP_ERR_STRING_PORTNR_OUTOFRANGE);
            return;
        }

        /*
        if (multcast)
        {
           if (_multicastttl.getInt()<=0 || _multicastttl.getInt()>255 ||
                 !isMulticastAddress(_multicastaddr.getString()))
            {
                error.putBool(true);
                errornr.putInt(UDP_ERR_MULTCAST_PARAM);
                errorstring.putString(getErrorMsg(UDP_ERR_MULTCAST_PARAM));
                _lasterror.putBool(true);
                _lasterrornr.putInt(UDP_ERR_MULTCAST_PARAM);
                _lasterrorstring.putString(getErrorMsg(UDP_ERR_MULTCAST_PARAM));
                return;
            }        
        }
        */

        // try to open socket on the specified port
        socket = new  IODatagram();
       
        //err = socket.open(portnr, broadcast, 
        //                  multcast, _multicastaddr.getString(), _multicastttl.getInt());
        err = socket.open(portnr, broadcast, 
                          false, "", 0);


        if (err!=0)
        {
            error.putBool(true);
            errornr.putInt(err);
            errorstring.putString(getErrorMsg(err));
            _lasterror.putBool(true);
            _lasterrornr.putInt(err);
            _lasterrorstring.putString(getErrorMsg(err));
            initialized = false;
        }
        else
        {
            initialized = true;
            _localportnr.putInt(socket.getLocalPortNr());           
            _localaddr.putString(socket.getLocalHostAddress());
        }        
    }

	 
	 

    //  close UDP connection 
    public synchronized void close(FcBool           error,
                                   FcInt            errornr,
                                   FcString         errorstring )
    {
        //////////////// get variables /////////////////////
        FcInt            _localportnr       = (FcInt)locVars[0];
        FcString         _localaddr         = (FcString)locVars[1];
        FcInt            _lastpartnerportnr = (FcInt)locVars[2];
        FcString         _lastpartneraddr   = (FcString)locVars[3];
        FcInt            _lastcmd           = (FcInt)locVars[4];
        FcBool           _lasterror         = (FcBool)locVars[5];
        FcInt            _lasterrornr       = (FcInt)locVars[6];
        FcString         _lasterrorstring   = (FcString)locVars[7];

        // reset errors
        error.putBool(false);
        errornr.putInt(0);
        errorstring.putString("");
        _lasterror.putBool(false);
        _lasterrornr.putInt(0);
        _lasterrorstring.putString("");

        _localportnr.putInt(0);
        _localaddr.putString("");
        _lastpartnerportnr.putInt(0);
        _lastpartneraddr.putString("");

        // initialize last cmd
        _lastcmd.putInt(UDP_CMD_CLOSE);

        closeSock();
    }



    // do the work of closing the socket
    void closeSock()
    {
        if (initialized)
        {
           if (socket!=null) 
           {
                socket.close();
                socket = null; 
           }
           initialized=false;
        }
    }



    //  send UDP datagram 
    public synchronized void send(String           addr,
                                  int              port,
                                  ObjectDirectoryEx dataIn,
                                  FcBool           error,
               	                  FcInt            errornr,
               	                  FcString         errorstring )
    {
        //////////////// get variables /////////////////////
        FcInt            _lastcmd           = (FcInt)locVars[4];
        FcBool           _lasterror         = (FcBool)locVars[5];
        FcInt            _lasterrornr       = (FcInt)locVars[6];
        FcString         _lasterrorstring   = (FcString)locVars[7];

        int len;
        int err = 0;

        // reset errors
        error.putBool(false);
        errornr.putInt(0);
        errorstring.putString("");
        _lasterror.putBool(false);
        _lasterrornr.putInt(0);
        _lasterrorstring.putString("");

        // initialize last cmd
        _lastcmd.putInt(UDP_CMD_SEND);

        if (!initialized)
        {
            error.putBool(true);
            errornr.putInt(UDP_ERR_SOCK_NOT_OPENED);
            errorstring.putString(UDP_ERR_STRING_SOCK_NOT_OPENED); 
            _lasterror.putBool(true);
            _lasterrornr.putInt(UDP_ERR_SOCK_NOT_OPENED);
            _lasterrorstring.putString(UDP_ERR_STRING_SOCK_NOT_OPENED);
            return;
        }

        if (port<0 || port>65535)
        {
            error.putBool(true);
            errornr.putInt(UDP_ERR_PORTNR_OUTOFRANGE);
            errorstring.putString(UDP_ERR_STRING_PORTNR_OUTOFRANGE);
            _lasterror.putBool(true);
            _lasterrornr.putInt(UDP_ERR_PORTNR_OUTOFRANGE);
            _lasterrorstring.putString(UDP_ERR_STRING_PORTNR_OUTOFRANGE);
            return;
        }
        
        sdata = ((ByteString)dataIn).getByteString();
        len = ((FcInt)(dataIn.getObjectByID(0))).getInt();
        if (len>sdata.length)
        {
            len = sdata.length;
        }
        err = socket.send(sdata, len, addr, port);
           
        if (err!=0)
        {
            error.putBool(true);
            errornr.putInt(err);
            errorstring.putString(getErrorMsg(err));
            _lasterror.putBool(true);
            _lasterrornr.putInt(err);
            _lasterrorstring.putString(getErrorMsg(err));
        }
        
    }




    //  receive UDP datagram 
    public synchronized void receive(int               datalen,
                                     ObjectDirectoryEx dataOut,
                                     FcString          fromaddr,
                                     FcInt             fromport,   
                                     FcBool            error,
                                     FcInt             errornr,
                                     FcString          errorstring )
    {
        //////////////// get variables /////////////////////
        FcInt            _lastpartnerportnr = (FcInt)locVars[2];
        FcString         _lastpartneraddr   = (FcString)locVars[3];
        FcInt            _lastcmd           = (FcInt)locVars[4];
        FcBool           _lasterror         = (FcBool)locVars[5];
        FcInt            _lasterrornr       = (FcInt)locVars[6];
        FcString         _lasterrorstring   = (FcString)locVars[7];

        int len;
        int err = 0;

        // reset errors
        error.putBool(false);
        errornr.putInt(0);
        errorstring.putString("");
        _lasterror.putBool(false);
        _lasterrornr.putInt(0);
        _lasterrorstring.putString("");

        // initialize last cmd
        _lastcmd.putInt(UDP_CMD_RCV);
        fromaddr.putString("");
        fromport.putInt(0);
        _lastpartneraddr.putString("");
        _lastpartnerportnr.putInt(0);
        ((FcInt)dataOut.getObjectByID(0)).putInt(0);

        if (!initialized)
        {
            error.putBool(true);
            errornr.putInt(UDP_ERR_SOCK_NOT_OPENED);
            errorstring.putString(UDP_ERR_STRING_SOCK_NOT_OPENED); 
            _lasterror.putBool(true);
            _lasterrornr.putInt(UDP_ERR_SOCK_NOT_OPENED);
            _lasterrorstring.putString(UDP_ERR_STRING_SOCK_NOT_OPENED); 
            ((FcInt)dataOut.getObjectByID(0)).putInt(0);
            return;
        }
    
       
        if (rdata==null || datalen!=rdata.length)
        {
            rdata = new byte[datalen];
        }
                
        err = socket.recv(rdata, datalen);

        if (socket.getAnzRead()>0)
        {
            fromaddr.putString(socket.getFromAddr());
            fromport.putInt(socket.getFromPort());
            _lastpartneraddr.putString(socket.getFromAddr());
            _lastpartnerportnr.putInt(socket.getFromPort());
            len = socket.getAnzRead();
            ((ByteString)dataOut).setByteString(rdata);
            ((FcInt)dataOut.getObjectByID(0)).putInt(len);
        }
        if (err!=0)
        {
            error.putBool(true); 
            errornr.putInt(err);
            errorstring.putString(getErrorMsg(err));
            _lasterror.putBool(true); 
            _lasterrornr.putInt(err);
            _lasterrorstring.putString(getErrorMsg(err));
        }        
    }


    public void lookup( boolean           enable,
                        String            hostName,
                        FcBool            ready,
                        FcString          hostAddress,   
                        FcBool            error,
                        FcInt             errornr,
                        FcString          errorstring )
    {
        FcInt            _lastcmd           = (FcInt)locVars[4];
        FcBool           _lasterror         = (FcBool)locVars[5];
        FcInt            _lasterrornr       = (FcInt)locVars[6];
        FcString         _lasterrorstring   = (FcString)locVars[7];

        if (!initialized)
        {
            error.putBool(true);
            errornr.putInt(UDP_ERR_SOCK_NOT_OPENED);
            errorstring.putString(UDP_ERR_STRING_SOCK_NOT_OPENED); 
            _lasterror.putBool(true); 
            _lasterrornr.putInt(UDP_ERR_SOCK_NOT_OPENED);
            _lasterrorstring.putString(UDP_ERR_STRING_SOCK_NOT_OPENED);
            _lastcmd.putInt(UDP_CMD_LOOKUP);
            return;
        }

        // reset errors
        error.putBool(false);
        errornr.putInt(0);
        errorstring.putString("");

        if (enable)
        {
            int status = socket.getLookupStatus();
            if (status==1) // still running
            {
                ready.putBool(false);
                hostAddress.putString("");
                return;
            }
            else  // else not running, -> error (-1) or ready (0)
            {
                // the input hostname has just been looked up:
                if (hostName.equals(socket.getLookupHostName()))
                {
                    if (status==0) // ready
                    {
                        ready.putBool(true);
                        hostAddress.putString(socket.getLookupHostAddr());
                        return; 
                    }
                    else // error
                    {
                        error.putBool(true);
                        errornr.putInt(socket.getLookupError());
                        errorstring.putString(getErrorMsg(socket.getLookupError()));
                        _lasterror.putBool(true); 
                        _lasterrornr.putInt(socket.getLookupError());
                        _lasterrorstring.putString(getErrorMsg(socket.getLookupError()));
                        _lastcmd.putInt(UDP_CMD_LOOKUP);
                        ready.putBool(true);
                        hostAddress.putString("");
                        return;
                    }
                }
                else  // new hostname : start lookup
                {
                    socket.startLookup(hostName);
                    ready.putBool(false);
                    hostAddress.putString("");
                    return;
                }
            }
        }
        else
        {
            // reset outputs
            error.putBool(false);
            errornr.putInt(0);
            errorstring.putString("");
            ready.putBool(false);
            hostAddress.putString("");
        }
        return;
    }


    private String getErrorMsg(int err)
    {
        switch (err)
        {
        case 0:
            return "";
        case UDP_ERR_SOCK_INIT_FAILED:
            return UDP_ERR_STRING_SOCK_INIT_FAILED;
        case UDP_ERR_SOCK_NOT_OPENED:
            return UDP_ERR_STRING_SOCK_NOT_OPENED;
        case UDP_ERR_UNKNOWNHOST:
            return UDP_ERR_STRING_UNKNOWN_HOST;
        case UDP_ERR_SEND:
            return UDP_ERR_STRING_SEND;
        case UDP_ERR_RECV:
            return UDP_ERR_STRING_RECV;
        case UDP_ERR_RECV_MSG_LONGER:
            return UDP_ERR_STRING_RECV_MSG_LONGER;
        case UDP_ERR_SEND_MSG_TOO_LONG:
            return UDP_ERR_STRING_SEND_MSG_TOO_LONG;
        case UDP_ERR_SEND_HOST_UNREACH:
            return UDP_ERR_STRING_SEND_HOST_UNREACH;
        case UDP_ERR_LOOKUP_RUNNING:
            return UDP_ERR_STRING_LOOKUP_RUNNING;
        case UDP_ERR_LOOKUP_HN_CHANGE:
            return UDP_ERR_STRING_LOOKUP_HN_CHANGE;
        case UDP_ERR_MULTCAST_PARAM:
            return UDP_ERR_STRING_MULTCAST_PARAM;
        case UDP_ERR_PORTNR_OUTOFRANGE:
            return UDP_ERR_STRING_PORTNR_OUTOFRANGE;
        }
        return "unknown error";
    }


    boolean isMulticastAddress(String addr)
    {
        int     posP;
        String  firstN;
        Integer nnr;

        posP = addr.indexOf('.');
        if (posP<=0 || posP>3)
        {
            return false;
        }
        
        firstN = addr.substring(0,posP);
        try
        {
            nnr = Integer.valueOf(firstN);
        }
        catch (NumberFormatException ne)
        {
            return false;
        }

        if (nnr.intValue()>=224 && nnr.intValue()<=239)
        {
            return true;
        }

        return false;

    }

}
