
package COM.softing.fc.CC.commFB;

import java.io.*;
import COM.softing.fc.CC.util.*;
import COM.softing.fc.CC.utility.*;

/* CommFB
** contains abstract methods called by the function blocks CONNECT, READ and WRITE.
** The abstrct interface must be implemented by a communication specific CommFB, 
** e.g. SerialCommFB
*/
public interface CommFB
{
    /** 
     * openConn
     * Opens a connection. 
     * Param:   sParam  string with communication specific parameters, e.g. PortNb.
     *                  If parameters are invalid defaults are used.
     * Return:  true    if connection is opened 
     *          false   if error occured
    */
    public abstract boolean openConn(String sParam);

    /** 
     * checkConn
     * Checks a connection. 
     * Param:   -
     * Return:  true    if connection is valid 
     *          false   if connection cannot be established
    */
    public abstract boolean checkConn();

    /** 
     * closeConn
     * Closes a connection. 
     * Param:   -
     * Return:  true    if connection is closed 
     *          false   if error occured
    */
    public abstract boolean closeConn();

    /** 
     * isRemoteClosing
     * Checks if the remote partner wants to close the connnection
     * Param:   -
     * Return:  true    if request to close the connection is received 
     *          false   otherwise
    */
    public abstract boolean isRemoteClosing();

    /** 
     * readAsync
     * Starts read request to read data asynchronously into internal buffer.
     * Call hasReadCompleted() to check if request has finished.
     * Param:   varLen  length of data to read
     *          readAll true: all available bytes are to be read (min. varLen)
     *          outVar  destination for read data
     * Return:  true    if read request is started
     *          false   if error occured
    */
    public abstract boolean readAsync(FcInt varLen, FcBool readAll, ByteString outVar);


    /**
     * writeAsnc
     * Starts write request to write data asynchronously to the remote partner
     * Call hasWriteCompleted() to check if request has finished.
     * Param:   inVar   data to write
     * Return:  true    if write request is started
     *          false   if error occured
    */
    public abstract boolean writeAsync(ByteString inVar);


    /**
     * verifyData
     * Verify data type match of data read by readAsync()
     * Param:   -
     * Return:  true    if data type OK
     *          false   if data invalid
    */
    public abstract boolean verifyData();


    /** 
     * depositData
     * Copy read data from internal buffer to outVar
     * Param:   -
     * Return:  -
    */
    public abstract void depositData();


    /**
     * hasReadCompleted
     * Checks if async. read request has finished
     * Param:   -
     * Return:  true    if request has finished
     *          false   if request is pending
    */
    public abstract boolean hasReadCompleted();


    /**
     * hasWriteCompleted
     * Checks if async. write request has finished
     * Param:   -
     * Return:  true    if request has finished
     *          false   if request is pending
    */
    public abstract boolean hasWriteCompleted();

    
    /**
     * getConnStatus
     * Gets the last local status.
     * Param:   -
     * Return:  0       if OK
     *          function dependend error information
    */
    public abstract int getConnStatus();


    /**
     * getReadStatus
     * Gets the last local status, e.g. error information of a read request.
     * Param:   -
     * Return:  0       if OK
     *          function dependend error information
    */
    public abstract int getReadStatus();

    /**
     * getWriteStatus
     * Gets the last local status, e.g. error information of a write request.
     * Param:   -
     * Return:  0       if OK
     *          function dependend error information
    */
    public abstract int getWriteStatus();

    /**
     * isReadActive()
     * Checks if read request is active
     * Param:   -
     * Return:  true    if read request is pending
     *          false   otherwise
    */
    public abstract boolean isReadActive();

    
    /**
     * isWriteActive()
     * Checks if write request is active
     * Param:   -
     * Return:  true    if write request is pending
     *          false   otherwise
    */
    public abstract boolean isWriteActive();

    
    /**
     * setReadActive()
     * Checks if read request is active
     * Param:   bIsActive   true if request is started
     *                      false if request has completed
     * Return:  -
    */
    public abstract void setReadActive(boolean bIsActive);

    
    /**
     * setWriteActive()
     * Checks if write request is active
     * Param:   bIsActive   true if request is started
     *                      false if request has completed
     * Return:  -
    */
    public abstract void setWriteActive(boolean bIsActive);

    /**
     * cancelIORequest()
     * cancels pending read and write request
     * Param:   -
     * Return:  -
    */
    public abstract void cancelIORequest();
    
    /**
     * cancelReadRequest()
     * cancels pending read request
     * Param:   -
     * Return:  -
    */
    public abstract void cancelReadRequest();

    /**
     * cancelWriteRequest()
     * cancels pending write request
     * Param:   -
     * Return:  -
    */
    public abstract void cancelWriteRequest();

    /**
     * release
     * called by the framework to release the function block
     * Param:   -
     * Return:  -
    */
    public abstract void release();
};

