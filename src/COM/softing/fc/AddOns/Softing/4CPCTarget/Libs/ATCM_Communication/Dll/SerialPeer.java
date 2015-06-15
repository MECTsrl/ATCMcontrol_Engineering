
package COM.softing.fc.CC.io;

import java.io.*;

public class  SerialPeer
{
    private int	    m_comHandle;		// comm. handle

    private int     m_portNb;           // port number
    private int     m_baudrate;         // currently/last selected baudrate
    private int     m_databits;         // current databits per byte: 4-8
    private int     m_parity;           // current parity per byte: no,odd,even,mark,space => 0-4
    private int     m_stopbits;         // current stopbits per byte: 1, 1.5, 2 => 0, 1, 2
    private int     m_protocol;         // current protocol: no, software, hardware => 0, 1, 2

    private int	    m_statusConn;       // connection status information
                                        // NO_ERROR                     0
                                        // ERROR_FILE_NOT_FOUND         2
                                        // ERROR_ACCESS_DENIED          5
                                        // ERROR_INVALID_HANDLE         6
                                        // ERROR_NOT_ENOUGH_MEMORY      8
                                        // ERROR_INVALID_PARAMETER      87
                                        // ERROR_OPEN_FAILED            110
                                        // ERROR_FILE_INVALID           1006

    private int     m_statusRead;       // read status info 
                                        // NO_ERROR                     0
                                        // ERROR_NOT_ENOUGH_MEMORY      8
                                        // ERROR_READ_FAULT             30
                                        // ERROR_DEVICE_NOT_CONNECTED   1167
                                        // -CE_RXOVER                   -1
                                        // -CE_OVERRUN                  -2
                                        // -CE_RXPARITY                 -4
                                        // -CE_FRAME                    -8
                                        // -CE_BREAK                    -16
    
    private int     m_statusWrite;      // write status info
                                        // NO_ERROR                     0
                                        // ERROR_NOT_ENOUGH_MEMORY      8
                                        // ERROR_WRITE_FAULT            29
                                        // ERROR_DEVICE_NOT_CONNECTED   1167
                                        // -CE_OVERRUN                  -2
                                        // -CE_TXFULL                   -256

    private int     m_commErrRead;      // relevate read errors get by ClearCommError() 
                                        // CE_RXOVER, CE_OVERRUN, CE_RXPARITY, CE_FRAME, CE_BREAK, CE_IOE, ...
    private int     m_commErrWrite;     // relevate write errors get by ClearCommError() 
                                        // CE_TXFULL, CE_IOE

	private boolean m_isOpen;

    private int     m_pAsyncReadStruct; // pointer to structure needed for async. read and write requests 
    private int     m_pAsyncWriteStruct;//      e.g. OVERLAPPED for Win32 SDK

    private byte[]  m_readBuf;          // byte array object for read data, 
                                        //      created in serialPeer.cpp
    private byte[]  m_writeBuf;         // byte array object for write request, 
                                        //      created by serialCommFB.java
    private int     m_readLen;          // length of data to read 
    private int     m_writeLen;         // length of data to write
    private int     m_pAsyncReadBuf;    // pointer to read data buffer, 
                                        //      allocated by serialpeer.cpp
    private int     m_pAsyncWriteBuf;   // pointer to read data buffer, 
                                        //      allocated by serialpeer.cpp



   /** The peer constructor.
    ** @param port the port ID number for this serial peer.
    */
    public SerialPeer()
	{
		m_comHandle = 0;
		m_portNb = 1;
		m_baudrate = 9600;
		m_databits = 8;
		m_parity = 0;
		m_stopbits = 0;
		m_statusConn = 0;
        m_statusRead = 0;
        m_statusWrite = 0;
		m_isOpen = false;
        m_pAsyncReadStruct = 0;
        m_pAsyncWriteStruct = 0;

        initDll();
	}


	/** open connection 
     ** @param sParam the communication parameters as a string: portNb, baudrate, etc.
     ** @returns true if port can be opened, fals otherwise
     */
    public boolean openConn()
    {
        if (m_pAsyncReadStruct == 0 || m_pAsyncWriteStruct == 0)
        {
            // m_statusConn set by initDll()
            return false;
        }

        m_comHandle = openPort();
		if (m_comHandle == 0)
		{
            // m_statusConn set by openPort()
			return false;
		}
        return true;
	}


	/** close connection
     */
    public boolean closeConn()
	{
		boolean bRet;

        if (!isPortOpen())
			return true;

        cancelIORequest();

        bRet = closePort();
        return bRet;
	}

    
    /** check if connection is open
     ** @returns true if open, fals otherwise
     */
	public boolean isPortOpen()
	{
		return m_isOpen;
	}


    /** put portNb
     */
    public void putPortNb(int portNb)
    {
        m_portNb = portNb;
    }


    /** put baudrate
     */
    public void putBaudrate(int baudrate)
    {
        m_baudrate = baudrate;
    }


    /** put databits
     */
    public void putDatabits(int databits)
    {
        m_databits = databits;
    }


    /** put parity
     */
    public void putParity(int parity)
    {
        m_parity = parity;
    }


    /** put stopbits
     */
    public void putStopbits(int stopbits)
    {
        m_stopbits = stopbits;
    }

    /** put protocol
     */
    public void putProtocol(int protocol)
    {
        m_protocol = protocol;
    }

    /** get status of connection
     */
    public int getStatusConn()
    {
        return(m_statusConn);
    }

    /** get status of read request
     */
    public int getStatusRead()
    {
        return(m_statusRead);
    }

    /** get status of write request
     */
    public int getStatusWrite()
    {
        return(m_statusWrite);
    }


    /** read data asynchronously from serial port, 
     ** call hasReadCompleted() to check if request has finished
     ** @return true if read request is running, false if error occured
     */
    public boolean readDataAsync(int iLen, boolean bReadAll)
    {
        int     iReadLen;

        if (!isPortOpen())
            return false;

        m_readBuf = null;
        m_statusRead = 0;
        m_commErrRead = 0;

        if (bReadAll)
        {
            m_readLen = dataAvailable();
            if (m_readLen < iLen)
                m_readLen = iLen;
        }
        else
        {
            m_readLen = iLen;
        }

        // Test if dataAvailable() detected an error
        if (m_statusRead != 0)
        {
            cancelReadRequest();
            return false;
        }

        if (!getDataAsync())
        {
            cancelReadRequest();
            return false;
        }

        return true;
    }


    /** write data asynchronously to serial port
     ** @return true if write request is running, false if error occured
     */
    public boolean writeDataAsync(byte[] data, int iLen)
    {
        if (!isPortOpen())
            return false;

        m_writeBuf = data;
        m_writeLen = iLen;
        m_statusWrite = 0;
        m_commErrWrite = 0;

        if (!putDataAsync())
        {
            cancelWriteRequest();
            return false;
        }

        return true;
    }


   /**
     * depositData
     * gets data read form remote partner
     * Param:   -
     * Return:  array of bytes
    */
    public byte[] depositData()
    {
        // get data 
        return m_readBuf;
    }

    public void release()
    {
        if (isPortOpen())
        {
            closeConn();
        }
        releaseDll();
    }

    protected void finalize()
    {
        release();
    }

    // native methods

	/** check if the established connection is OK
     ** @returns true if connection OK, false otherwise
     */
    public native boolean checkConn();


	/** native method to open the port
     ** @returns a HANDLE if the port can be openened, otherwise 0
     */
	private native int openPort();


	/** native method to close the port
     ** @returns true if the port is closed, false otherwise
     */
	private native boolean closePort();


	/** native method to check if data is available for reading
     ** @returns the number of bytes available in the input buffer
     */
	public native int dataAvailable();

	
	/** native method to read data asynchronously
     ** @returns true if read request ist running, false otherwise
     */
	private native boolean getDataAsync();


	/** native method to write data asynchronously
     ** @returns true if write request ist running, false otherwise
     */
    private native boolean putDataAsync();


	/** native method to check if read request has finished
     ** @returns true if request has completed, false otherwise
     */
    public native boolean hasReadCompleted();

	/** native method to check if write request has finished
     ** @returns true if request has completed, false otherwise
     */
    public native boolean hasWriteCompleted();

    /** method to cancel a pending IO request
     */
    public void cancelIORequest()
    {
        cancelReadRequest();
        cancelWriteRequest();
    }

    /** native method to cancel a pending IO request
     */
    public native void cancelReadRequest();

    /** native method to cancel a pending IO request
     */
    public native void cancelWriteRequest();

    private native void initDll();

    private native void releaseDll();


    // load native DLL
    static
    {
        // load DLL as peer to OS serial device
        System.loadLibrary("serialPeer");
    }
    
}


