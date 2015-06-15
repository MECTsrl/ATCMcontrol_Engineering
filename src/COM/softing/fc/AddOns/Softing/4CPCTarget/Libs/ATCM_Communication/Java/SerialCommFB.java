package COM.softing.fc.CC.commFB;

import COM.softing.fc.CC.io.*;
import COM.softing.fc.CC.util.*;
import java.io.*;
import java.util.Vector;
import COM.softing.fc.CC.utility.*;

public class SerialCommFB
    implements CommFB
{
    private SerialPeer m_serPeer = new SerialPeer();    // object for serial I/O
    private ByteString m_ReadOutVar;                    // read:    destination 
    private ByteString m_WriteInVar;                    // write:   source  

    private int m_iConnStatus = 0;                      // ATCM error codes, see 4C_CommLib CONST CFB_...
    private int m_iReadStatus = 0;                      // ATCM error codes, see 4C_CommLib CONST CFB_...
    private int m_iWriteStatus = 0;                     // ATCM error codes, see 4C_CommLib CONST CFB_...
    private boolean m_bReadActive = false;              // only set by function block READ
    private boolean m_bWriteActive = false;             // only set by function block WRITE

    /** 
     * openConn
     * Opens a connection. 
     * Param:   sParam  string with communication specific parameters, e.g. PortNb.
     *                  If parameters are invalid defaults are used.
     * Return:  true    if connection is opened 
     *          false   if error occured
    */
    public synchronized boolean openConn(String sParam)
    {
        boolean bRet;
        // scan parameters
        if (!setParams(sParam))
        {
            // m_iConnStatus set in setParams()
            // e.g. CFB_Serial_ErrInvalidBaudrate 
            return false;
        }

        // open connection
        bRet = m_serPeer.openConn();
        if (bRet)
            m_iConnStatus = 0;          // CFB_NoError
        else
        {
            int iError = m_serPeer.getStatusConn();
            switch(iError)
            {
            case 2:     // ERROR_FILE_NOT_FOUND
                m_iConnStatus = -100;   // CFB_Serial_ErrInvalidPortNb
                break;
            case 5:     // ERROR_ACCESS_DENIED
                m_iConnStatus = -101;   // CFB_Serial_ErrPortInUse
                break;
            case 87:    // ERROR_INVALID_PARAMETER
                m_iConnStatus = -107;   // CFB_Serial_ErrInvalidParameters
                break;
            default: 
                m_iConnStatus = 2;      // CFB_ErrRegNegResponse
                break;
            }
        }
        // DebugManager.print("SerialCommFB: openConn(): " + bRet + "\n");
        return bRet;
    }

    /** 
     * checkConn
     * Checks a connection. 
     * Param:   -
     * Return:  true    if connection is valid 
     *          false   if connection cannot be established
    */
    public synchronized boolean checkConn()
    {
        boolean bRet;

        bRet = m_serPeer.checkConn();
        return bRet;
    }

    /** 
     * closeConn
     * Closes a connection. 
     * Param:   -
     * Return:  true    if connection is closed 
     *          false   if error occured
    */
    public synchronized boolean closeConn()
    {
        boolean bRet;

        bRet = m_serPeer.closeConn();

        // DebugManager.print("SerialCommFB: closeConn()" + bRet + "\n");
        return bRet;
    }

    /** 
     * isRemoteClosing
     * Checks if the remote partner wants to close the connnection
     * Param:   -
     * Return:  true    if request to close the connection is received 
     *          false   otherwise
    */
    public boolean isRemoteClosing()
    {
        return false;
    }

    /** 
     * readAsync
     * Starts read request to read data asynchronously into internal buffer.
     * Call hasIOCompleted() to check if request has finished.
     * Param:   varLen  length data to read
     *          readAll true: all available bytes are to be read (min. varLen)
     *          outVar  destination for read data
     * Return:  true    if read request is started
     *          false   if error occured
    */
    public synchronized boolean readAsync(FcInt varLen, FcBool readAll, ByteString outVar)
    {
        boolean bRet;
        boolean bReadAll;
        int readLen;

        m_iReadStatus = 0;  // CFB_NoError

        m_ReadOutVar = outVar;
        readLen = varLen.getInt();
        bReadAll = readAll.getBool();

        // start read request
        bRet = m_serPeer.readDataAsync(readLen, bReadAll);
        if (!bRet)
        {
            int iError = m_serPeer.getStatusRead();
            switch(iError)
            {
            case 1167:  // ERROR_DEVICE_NOT_CONNECTED
                m_iReadStatus = 7;      // CFB_ErrRemoteWrongState
                break;
            case 8:     // ERROR_NOT_ENOUGH_MEMORY
                m_iReadStatus = -4;     // CFB_ErrOutOfMemory
                break;
            case 30:    // ERROR_READ_FAULT
                m_iReadStatus = 2;      // CFB_ErrRegNegResponse
                break;
            case -1:    // CE_RXOVER
            case -2:    // CE_OVERRUN
                m_iReadStatus = -112;   // CFB_Serial_ErrReceiveOverrun
                break;
            case -4:    // CE_RXPARITY
                m_iReadStatus = -113;   // CFB_Serial_ErrReceiveParity
                break;
            case -8:    // CE_FRAME
                m_iReadStatus = -111;   // CFB_Serial_ErrReceiveFrame
                break;
            case -16:   // CE_BREAK
                m_iReadStatus = -110;   // CFB_Serial_ErrReceiveBreak
                break;
            default: 
                m_iReadStatus = 2;      // CFB_ErrRegNegResponse
                break;
            }
        }

        // DebugManager.print("SerialCommFB: readAsync ret = " + bRet + "\n");

        return bRet;
    }

    /**
     * writeAsync
     * Starts write request to write data asynchronously to the remote partner
     * Call hasIOCompleted() to check if request has finished.
     * Param:   inVar   data to write
     * Return:  true    if write request is started
     *          false   if error occured
    */
    public synchronized boolean writeAsync(ByteString inVar)
    {
        boolean bRet;
        byte[] writeBuf;
        int writeLen;

        m_iWriteStatus = 0;

        m_WriteInVar = inVar;
        writeBuf = m_WriteInVar.getByteString();
        writeLen = writeBuf.length;

        // start write request
        bRet = m_serPeer.writeDataAsync(writeBuf, writeLen);
        if (!bRet)
        {
            int iError = m_serPeer.getStatusWrite();
            switch(iError)
            {
            case 1167:  // ERROR_DEVICE_NOT_CONNECTED
                m_iWriteStatus = 7;      // CFB_ErrRemoteWrongState
                break;
            case 8:     // ERROR_NOT_ENOUGH_MEMORY
                m_iWriteStatus = -4;     // CFB_ErrOutOfMemory
                break;
            case 29:    // ERROR_WRITE_FAULT
                m_iWriteStatus = 2;      // CFB_ErrRegNegResponse
                break;
            case -2:    // CE_OVERRUN
            case -256:  // CE_TXFULL
                m_iWriteStatus = -114;   // CFB_Serial_ErrWriteFull
                break;
            default: 
                m_iWriteStatus = 2;      // CFB_ErrRegNegResponse
                break;
            }
        }
        // DebugManager.print("SerialCommFB: writeAsync " + bRet + "\n");
        return bRet;
    }

    /**
     * verifyData
     * Verify data type match of data read by readAsync()
     * Param:   -
     * Return:  true    if data type OK
     *          false   if data invalid
    */
    public boolean verifyData()
    {
        // no data type verification for serial com!
        return true;
    }

    /** 
     * depositData
     * Copy read data from internal buffer to outVar
     * Param:   -
     * Return:  -
    */
    public void depositData()
    {
        // set output-variable to byte array
        // already done...
    }

    /**
     * hasReadCompleted
     * Checks if async. read request has finished
     * Param:   -
     * Return:  true    if request has finished
     *          false   if request is pending
    */
    public synchronized boolean hasReadCompleted()
    {
        boolean bRet;
        bRet = m_serPeer.hasReadCompleted();
        if (bRet)
        {
            int iError = m_serPeer.getStatusRead();
            switch(iError)
            {
            case 0:     //  CFB_NoError
                m_iReadStatus = 0;
                m_ReadOutVar.setByteString(m_serPeer.depositData());
                break;
            case 1167:  // ERROR_DEVICE_NOT_CONNECTED
                m_iReadStatus = 7;      // CFB_ErrRemoteWrongState
                break;
            case 8:     // ERROR_NOT_ENOUGH_MEMORY
                m_iReadStatus = -4;     // CFB_ErrOutOfMemory
                break;
            case 30:    // ERROR_READ_FAULT
                m_iReadStatus = 2;      // CFB_ErrRegNegResponse
                break;
            case -1:    // CE_RXOVER
            case -2:    // CE_OVERRUN
                m_iReadStatus = -112;   // CFB_Serial_ErrReceiveOverrun
                break;
            case -4:    // CE_RXPARITY
                m_iReadStatus = -113;   // CFB_Serial_ErrReceiveParity
                break;
            case -8:    // CE_FRAME
                m_iReadStatus = -111;   // CFB_Serial_ErrReceiveFrame
                break;
            case -16:   // CE_BREAK
                m_iReadStatus = -110;   // CFB_Serial_ErrReceiveBreak
                break;
            default: 
                m_iReadStatus = 2;      // CFB_ErrRegNegResponse
                break;
            }
        }

        // DebugManager.print("SerialCommFB: hasReadCompleted " + bRet + "\n");
        return bRet;
    }

    /**
     * hasWriteCompleted
     * Checks if async. write request has finished
     * Param:   -
     * Return:  true    if request has finished
     *          false   if request is pending
    */
    public synchronized boolean hasWriteCompleted()
    {
        boolean bRet;
        bRet = m_serPeer.hasWriteCompleted();
        if (bRet)
        {
            int iError = m_serPeer.getStatusWrite();
            switch(iError)
            {
            case 0:     //  CFB_NoError
                m_iWriteStatus = 0;
                break;
            case 1167:  // ERROR_DEVICE_NOT_CONNECTED
                m_iWriteStatus = 7;      // CFB_ErrRemoteWrongState
                break;
            case 8:     // ERROR_NOT_ENOUGH_MEMORY
                m_iWriteStatus = -4;     // CFB_ErrOutOfMemory
                break;
            case 29:    // ERROR_WRITE_FAULT
                m_iWriteStatus = 2;      // CFB_ErrRegNegResponse
                break;
            case -2:    // CE_OVERRUN
            case -256:  // CE_TXFULL
                m_iWriteStatus = -114;   // CFB_Serial_ErrWriteFull
                break;
            default: 
                m_iWriteStatus = 2;      // CFB_ErrRegNegResponse
                break;
            }
        }

        // DebugManager.print("SerialCommFB: hasWriteCompleted " + bRet + "\n");
        return bRet;
    }

    /**
     * getConnStatus
     * Gets the last local status
     * Param:   -
     * Return:  0       if OK
     *          function dependend error information
    */
    public int getConnStatus()
    {
        return m_iConnStatus;
    }

    /**
     * getReadStatus
     * Gets the last local status, e.g. error information of a read request.
     * Param:   -
     * Return:  0       if OK
     *          function dependend error information
    */
    public int getReadStatus()
    {
        return m_iReadStatus;
    }

    /**
     * getWriteStatus
     * Gets the last local status, e.g. error information of a write request.
     * Param:   -
     * Return:  0       if OK
     *          function dependend error information
    */
    public int getWriteStatus()
    {
        return m_iWriteStatus;
    }

    /**
     * isReadActive()
     * Checks if read request is active
     * Param:   -
     * Return:  true    if read request is pending
     *          false   otherwise
    */
    public boolean isReadActive()
    {
        return m_bReadActive;
    }

    
    /**
     * isWriteActive()
     * Checks if write request is active
     * Param:   -
     * Return:  true    if write request is pending
     *          false   otherwise
    */
    public boolean isWriteActive()
    {
        return m_bWriteActive;
    }

    
    /**
     * setReadActive()
     * Checks if read request is active
     * Param:   bIsActive   true if request is started
     *                      false if request has completed
     * Return:  -
    */
    public void setReadActive(boolean bIsActive)
    {
        m_bReadActive = bIsActive;
    }

    
    /**
     * setWriteActive()
     * Checks if write request is active
     * Param:   bIsActive   true if request is started
     *                      false if request has completed
     * Return:  -
    */
    public void setWriteActive(boolean bIsActive)
    {
        m_bWriteActive = bIsActive;
    }

    /**
     * cancelIORequest()
     * cancels pending read and write request
     * Param:   -
     * Return:  -
    */
    public synchronized void cancelIORequest()
    {
        m_serPeer.cancelIORequest();
        // DebugManager.print("SerialCommFB: cancelIORequest \n");
    }

    /**
     * cancelReadRequest()
     * cancels pending read request
     * Param:   -
     * Return:  -
    */
    public synchronized void cancelReadRequest()
    {
        m_serPeer.cancelReadRequest();
    }

    /**
     * cancelWriteRequest()
     * cancels pending write request
     * Param:   -
     * Return:  -
    */
    public synchronized void cancelWriteRequest()
    {
        m_serPeer.cancelWriteRequest();
    }

    /**
     * release
     * called by the framework to release the function block
     * Param:   -
     * Return:  -
    */
    public synchronized void release()
    {
        m_serPeer.release();
    }

    ///////////////////////////////////////////////////////////////////////////
    // private functions

    /**
     * setDefaultParams
     * private function to set default parameters for serial communication
     * Param:   -
     * Return:  -
    */
    private void setDefaultParams()
    {
        m_serPeer.putPortNb(1);
        m_serPeer.putBaudrate(9600);
        m_serPeer.putDatabits(8);
        m_serPeer.putParity(0);     // N
        m_serPeer.putStopbits(0);   // 1
        m_serPeer.putProtocol(0);   // NO
    }

    /**
     * setParams
     * private function to scan parameter string and set parameters for serial communication;
     * sets m_iConnStatus if a given parameter is invalid
     * Param:   sParam  parameter string
     * Return:  true    valid parameter string 
     *          false   invalid parameter string 
    */
    private boolean setParams(String sParam)
	{
		int begIdx = 0;
		int endIdx = 0;
        int iParam;
		String subStr;

        // set default values for all parameters
        setDefaultParams();

        if (sParam.length() == 0)
        {
            // default parameters already set
            return true;
        }

        // scan parameters: 
        // portNb, baudrate, databits, parity, stopbits, protocol

		// portNb 
		endIdx = findChar(sParam, ',', begIdx);
		if (endIdx < 0)
        {
            // default parameters are used for all parameters left
			return true; 
        }
        subStr = sParam.substring(begIdx, endIdx);
        subStr = subStr.trim();
        if (!setPortNb(subStr))
        {
            m_iConnStatus = -100;    // CFB_Serial_ErrInvalidPortNb
            return false;
        }

		// baudrate
		begIdx = endIdx + 1;
		endIdx = findChar(sParam, ',', begIdx);
		if (endIdx < 0)
        {
            // default parameters are used for all parameters left
			return true; 
        }
        subStr = sParam.substring(begIdx, endIdx);
        subStr = subStr.trim();
        if (!setBaudrate(subStr))
        {
            m_iConnStatus = -102;    // CFB_Serial_ErrInvalidBaudRate
            return false;
        }

		// databits
		begIdx = endIdx + 1;
		endIdx = findChar(sParam, ',', begIdx);
		if (endIdx < 0)
        {
            // default parameters are used for all parameters left
			return true; 
        }
        subStr = sParam.substring(begIdx, endIdx);
        subStr = subStr.trim();
        if (!setDatabits(subStr))
        {
            m_iConnStatus = -103;    // CFB_Serial_ErrInvalidDatabits
            return false;
        }

		// parity
		begIdx = endIdx + 1;
		endIdx = findChar(sParam, ',', begIdx);
		if (endIdx < 0)
        {
            // default parameters are used for all parameters left
			return true; 
        }
        subStr = sParam.substring(begIdx, endIdx);
        subStr = subStr.trim();
        if (!setParity(subStr))
        {
            m_iConnStatus = -104;    // CFB_Serial_ErrInvalidParity
            return false;
        }

		// stopbits
		begIdx = endIdx + 1;
		endIdx = findChar(sParam, ',', begIdx);
		if (endIdx < 0)
        {
            // default parameters are used for all parameters left
			return true; 
        }
        subStr = sParam.substring(begIdx, endIdx);
        subStr = subStr.trim();
        if (!setStopbits(subStr))
        {
            m_iConnStatus = -105;    // CFB_Serial_ErrInvalidStopbits
            return false;
        }

        // protocol
		begIdx = endIdx + 1;
		endIdx = sParam.length();
		if (begIdx >= endIdx)
        {
            // default parameters are used for all parameters left
			return true; 
        }
        subStr = sParam.substring(begIdx);
        subStr = subStr.trim();
        if (!setProtocol(subStr))
        {
            m_iConnStatus = -106;    // CFB_Serial_ErrInvalidProtcol
            return false;
        }
        return true;
	}

    /**
     * findChar
     * private function to scan string for given character
     * Param:   sParam  string to scan
     *          iChar   char to find
     *          begIdx  indes to start with
     * Return:  index of character in string sParam
     *          -1 if char cannot be found
    */
	private int findChar(String sParam, int iChar, int begIdx)
	{
		int iIdx;
		if (begIdx >= sParam.length())
			return -1;
		iIdx = sParam.indexOf(iChar, begIdx);

		return iIdx;
	}

    /**
     * getIntFromString
     * private function to convert a string to an integer value
     * Param:   s   String to convert
     * Return:  Integer
    */
	private int getIntFromString(String s)
	{
		int i = 0;

		try
		{
			Integer iObj = new Integer(s);
            i = iObj.intValue();
		}
		catch (NumberFormatException e)
		{

		}

		return i;
	}

    /**
     * setPortNb
     * private function to check and set the port number
     * Param:   s
     * Return:  true    if string is a valid port nb
     *          false   if string is invalid
    */
    private boolean setPortNb(String s)
    {
        if (s.length() == 0)
        {
            // default already set
            return true;    
        }
        int i = getIntFromString(s);
        if (i < 0)
            return false;

        m_serPeer.putPortNb(i);
        return true;
    }

    /**
     * setBaudrate
     * private function to check and set the baudrate
     * Param:   s
     * Return:  true    if string is valid
     *          false   if string is invalid
    */
    private boolean setBaudrate(String s)
    {
        if (s.length() == 0)
        {
            // default already set
            return true;    
        }

        boolean bRet;
        int i = getIntFromString(s);
        switch (i)
        {
        case 110:
        case 300:
        case 600:
        case 1200:
        case 2400:
        case 4800:
        case 9600:
        case 14400:
        case 19200:
        case 38400:
        case 56000:
        case 57600:
        case 115200:
        case 128000:
        case 230400:
        case 256000:
        case 460800:
        case 921600:
            m_serPeer.putBaudrate(i);
            bRet = true;
            break;
        default:
            bRet = false;
            break;
        }
        return bRet;
    }

    /**
     * setDatabits
     * private function to check and set data bits
     * Param:   s
     * Return:  true    if string is valid
     *          false   if string is invalid
    */
    private boolean setDatabits(String s)
    {
        if (s.length() == 0)
        {
            // default already set
            return true;    
        }

        boolean bRet;
        int i = getIntFromString(s);
        switch (i)
        {
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            m_serPeer.putDatabits(i);
            bRet = true;
            break;
        default:
            bRet = false;
            break;
        }
        return bRet;
    }

    /**
     * setParity
     * private function to check and set the parity
     * Param:   s
     * Return:  true    if string is valid
     *          false   if string is invalid
    */
    private boolean setParity(String s)
    {
        if (s.length() == 0)
        {
            // default already set
            return true;    
        }

        if (s.length() != 1)
            return false;

        s = s.toUpperCase();
        boolean bRet = true;
        char c = s.charAt(0);

        switch (c)
        {
        case 'N':
            m_serPeer.putParity(0);
            break;
        case 'O':
            m_serPeer.putParity(1);
            break;
        case 'E':
            m_serPeer.putParity(2);
            break;
        case 'M':
            m_serPeer.putParity(3);
            break;
        case 'S':
            m_serPeer.putParity(4);
            break;
        default:
            bRet = false;
            break;
        }
        return bRet;
    }

    /**
     * setStopbits
     * private function to check and set stop bits
     * Param:   s
     * Return:  true    if string is valid
     *          false   if string is invalid
    */
    private boolean setStopbits(String s)
    {
        if (s.length() == 0)
        {
            // default already set
            return true;    
        }

        boolean bRet = true;

        if (s.compareTo("1") == 0)
            m_serPeer.putStopbits(0);
        else if (s.compareTo("1.5") == 0)
            m_serPeer.putStopbits(1);
        else if (s.compareTo("2") == 0)
            m_serPeer.putStopbits(2);
        else 
            bRet = false;

        return bRet;
    }

    /**
     * setProtocol
     * private function to check and set the protocol
     * Param:   s
     * Return:  true    if string is valid
     *          false   if string is invalid
    */
    private boolean setProtocol(String s)
    {
        if (s.length() == 0)
        {
            // default already set
            return true;    
        }

        if (s.length() != 2)
            return false;

        boolean bRet = true;

        s = s.toUpperCase();

        if (s.compareTo("NO") == 0)
            m_serPeer.putProtocol(0);
        else if (s.compareTo("SW") == 0)
            m_serPeer.putProtocol(1);
        else if (s.compareTo("HW") == 0)
            m_serPeer.putProtocol(2);
        else
            bRet = false;

        return bRet;
    }

}


