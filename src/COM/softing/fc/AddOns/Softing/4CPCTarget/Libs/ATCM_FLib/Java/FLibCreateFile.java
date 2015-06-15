package COM.softing.fclib.flib;

import COM.softing.fc.CC.system.StructVar;
import COM.softing.fc.CC.utility.ByteString;
import COM.softing.fc.CC.util.*;
import java.io.*;

/**
 * class FLibCreateFile
 */
public class FLibCreateFile
extends Thread
implements IFLib
{
	private static boolean ENABLE_DEBUG_PRINT = false;	// set false for release compilation

    private File                m_fileObj;
	private FcBool              m_bResourceBusy;
    private FcBool              m_bIsOpen;
	public	RandomAccessFile    m_file;
	private String              m_strFilename;
	private int                 m_iMode;
	private boolean             m_error     = true;
	private String              m_errtxt    = FLibError.CFA_TimeoutStr;
	private int                 m_errcd     = FLibError.CFA_Timeout;

	public FLibCreateFile(FcBool bResourceBusy,
                          FcBool bFileOpen,
                          RandomAccessFile file,
                          String strFilename,
                          int iMode)
	{
		m_bResourceBusy = bResourceBusy;
        m_bIsOpen = bFileOpen;
		m_file = file;
		m_strFilename = strFilename;
		m_iMode = iMode;
        setName("FLibCreateFile");
	}
	
	public RandomAccessFile getFile()
	{
		return m_file;
	}
	
	public void getResult(FcBool error,
						  FcInt errcd,
						  FcString errtxt,
						  FcInt position)
	{
        if(ENABLE_DEBUG_PRINT) {
            System.out.println("\t\tFLibCreateFile.getResult() - error: " + m_error);
        }
        error.putBool(m_error);
        errcd.putInt(m_errcd);
        errtxt.putString(m_errtxt);

        if(m_iMode == FLibMode.CFA_APPEND) {
            // set position to 'End Of File'
            position.putInt(FLibMode.CFA_EOF);
        } else {
            // set position to 'Begin Of File'
            position.putInt(FLibMode.CFA_BOF);
        }
	}
	
	
	public void run()
	{
		if(ENABLE_DEBUG_PRINT)
			System.out.println("\t\tFLibCreateFile");

        if(!m_bIsOpen.getBool()) {
            // open file
		
            String strMode;
            switch(m_iMode) {
            case FLibMode.CFA_READ:
                strMode = "r";
                break;
            case FLibMode.CFA_READ_WRITE:
                strMode = "rw";
                break;
            case FLibMode.CFA_APPEND:
                strMode = "rw";
                break;
            default:
				// error
				m_error = true;
				m_errcd = FLibError.CFA_InvalidArgument;
				m_errtxt = "InvalidArgument: mode";
				m_bResourceBusy.putBool(false);
				return;
            }
		    try {
                m_fileObj = new File(m_strFilename);
		    	m_file = new RandomAccessFile(m_fileObj, strMode);
                                  
                // lock instance for 'createFile' unless 'closeFile' is called
                m_bIsOpen.putBool(true);
                
		    	// done
                setResult(false, 0, "");
		    } catch(ThreadDeath e) {
		    	if(ENABLE_DEBUG_PRINT)
		    		System.out.println("\t\tFLibCreateFile - Timeout");
                setResult(true, FLibError.CFA_Timeout, FLibError.CFA_TimeoutStr);
		    	throw e;
		    } catch(IOException e) {
		    	if(ENABLE_DEBUG_PRINT)
		    		System.out.println("\t\tFLibCreateFile - IOException");
                if(!m_fileObj.exists()) {
                    // file does not exist
                    setResult(true, FLibError.CFA_CommonError, FLibError.CFA_FileNotPresentStr + "; " + e.getMessage());
                } else if(!m_fileObj.canWrite()) {
                    // file is write protected
                    setResult(true, FLibError.CFA_CommonError, FLibError.CFA_FileIsWriteprotected + "; " + e.getMessage());
                } else {
                    // unknown error
                    setResult(true, FLibError.CFA_CommonError, e.getMessage());
                }                
		    } catch(Exception e) {
                if(ENABLE_DEBUG_PRINT) {
                  System.out.println("\t\tFLibCreateFile - Exception");
                }
                setResult(true, FLibError.CFA_CommonError, e.getMessage());
		    } finally {
		    	m_bResourceBusy.putBool(false);
		    }
        } else {
            // instace is locked; file is already open
		    m_error = true;
		    m_errcd = FLibError.CFA_FileAlreadyOpen;
		    m_errtxt = FLibError.CFA_FileAlreadyOpenStr;
	    	m_bResourceBusy.putBool(false);
        }
	}

	public void setResult(boolean bError, int iErrcd, String strErrtxt)
    {
        m_error = bError;
        m_errcd = iErrcd;
        m_errtxt = strErrtxt;
        m_bResourceBusy.putBool(false);
    }
}
