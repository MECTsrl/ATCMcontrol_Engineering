package COM.softing.fc.CC.io;

import java.net.InetAddress;
import java.net.UnknownHostException;

public class IODatagram
  implements Runnable
{
    private int     m_port;
    private int     m_packLen;
    private byte    m_packet[];
    private String  m_toAddr;
    private int     m_toPort;
    private String  m_fromAddr;
    private int     m_fromPort;
    private int     m_anzRead;
    private boolean m_broadcast;
    private boolean m_multicast;
    private String  m_mcAddr;
    private int     m_mcTTL;
    private String  m_localHostAddr;
    private int     m_localPortNr;

    private int     m_luStatus;  // 0 Ready; 1 Running; -1 Error
    private String  m_luHostName;
    private String  m_luHostAddr;
    private int     m_luError;
   
    int /*Socket*/ socket;

     
    public IODatagram()
	{
        initDll();
	}

    public void release()
    {
        releaseDll();
    }

    protected void finalize()
    {
        release();
    }

    public synchronized int open(int port, boolean broadcast, 
                                 boolean multcast, String multicastAddr, int multicastTTL)
    {
        m_port       = port;
        m_broadcast  = broadcast;
        m_multicast  = multcast;
        m_mcAddr     = multicastAddr;
        m_mcTTL      = multicastTTL;
        m_luHostName = "";
        m_luHostAddr = "";
        m_localHostAddr = "";
        m_localPortNr   = 0;
        return udp_open();
    }

    public synchronized int close()
    {
        return udp_close();
    }

    public synchronized int send(byte packet[], int len, String toAddr, int toPort)
    {
        if (packet==null)
        {
            return -1;
        }

        m_packet  = packet;
        m_packLen = len;
        m_toAddr  = toAddr;
        m_toPort  = toPort;

        return udp_send();
    }

    public synchronized int recv(byte packet[], int len)
    {
        int ret;

        // byte array has to be allocated here, will only be filled with data
        // this should never happen if called from UDP_SocketHandle.java
        if (packet==null || packet.length<len)
        {
            return -1;
        }

        m_fromAddr = new String("");

        m_packet = packet;
        m_packLen = len;
        
        ret = udp_recv();

        return ret;
    }

    public String getFromAddr()
    {
        return m_fromAddr;
    }

    public int getFromPort()
    {
        return m_fromPort;
    }

    public int getAnzRead()
    {
        return m_anzRead;
    }

    // do host name lookup
    public void run()
    {
        int err = udp_lookup();
        
        if (err == 0) 
        {
            m_luError = 0;
            m_luStatus = 0; // ready
        }
        else
        {        
            m_luError = err;
            m_luHostAddr = new String("");;
            m_luStatus = -1; // error
        }
    }

    public void startLookup(String hostName)
    {
        m_luStatus   = 1; // running
        m_luHostName = hostName;
        m_luError    = 0;

        Thread luThread = new Thread(this);
        luThread.start();
    }

    public int getLookupStatus()
    {
        return m_luStatus;
    }

    public String getLookupHostName()
    {
        return m_luHostName;
    }

    public String getLookupHostAddr()
    {
        return m_luHostAddr;
    }

    public int getLookupError()
    {
        return m_luError;
    }

    public String getLocalHostAddress()
    {
        return m_localHostAddr;
    }

    public int getLocalPortNr()
    {
        return m_localPortNr;
    }
   
    private native int udp_open();
    private native int udp_close();
    private native int udp_send();
    private native int udp_recv();
    private native int udp_lookup();

    private native void initDll();
    
    private native void releaseDll();


    // load native DLL
    static
    {
        // load DLL as peer to OS serial device
        System.loadLibrary("serialPeer");
    }
    
}