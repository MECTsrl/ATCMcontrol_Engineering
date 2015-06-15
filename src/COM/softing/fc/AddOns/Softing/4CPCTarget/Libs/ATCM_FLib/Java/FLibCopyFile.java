package COM.softing.fclib.flib;

import COM.softing.fc.CC.system.StructVar;
import COM.softing.fc.CC.utility.ByteString;
import COM.softing.fc.CC.util.*;
import java.io.*;

/**
 * class FLibReadFile
 */
public class FLibCopyFile
extends Thread
implements IFLib
{
	private static boolean ENABLE_DEBUG_PRINT = false;	// set false for release compilation
	
	private FcBool              m_bResourceBusy;
    private boolean             m_bIsOpen;
	private String              m_strSrcFilename;
	private String              m_strDestFilename;
    private boolean             m_bOverwrite;
    
	private RandomAccessFile    m_file;
    
	private boolean             m_error		= true;
	private String              m_errtxt	= FLibError.CFA_TimeoutStr;
	private int                 m_errcd		= FLibError.CFA_Timeout;
	
	public FLibCopyFile(FcBool bResourceBusy,
                          boolean bFileOpen,
                          String strSrcFilename,
                          String strDestFilename,
                          boolean bOverwrite)
	{
		m_bResourceBusy = bResourceBusy;
        m_bIsOpen = bFileOpen;
        m_strSrcFilename = strSrcFilename;
		m_strDestFilename = strDestFilename;
        m_bOverwrite = bOverwrite;
        setName("FLibCopyFile");
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
		if(ENABLE_DEBUG_PRINT)
			System.out.println("\t\tFLibReadFile.getResult() - error: " + m_error);
		error.putBool(m_error);
		errcd.putInt(m_errcd);
		errtxt.putString(m_errtxt);
	}
	
	
	public void run()
	{
        if(!m_bIsOpen) {
		    try {
                // open destination file
                File destFileObj = new File(m_strDestFilename);
                if(destFileObj.exists() & !m_bOverwrite) {
                    // file already exists
                    setResult(true, FLibError.CFA_CommonError, FLibError.CFA_FileAlreadyExistsStr + "; " + m_strDestFilename);
                    return;
                }
                RandomAccessFile destFile = new RandomAccessFile(destFileObj, "rw");
                
                // open source file
                RandomAccessFile srcFile = new RandomAccessFile(new File(m_strSrcFilename), "r");
                
                byte[] byBuffer = new byte[(int)srcFile.length()];
                
                srcFile.read(byBuffer, 0, (int)srcFile.length() );
                destFile.write(byBuffer, 0, (int)srcFile.length() );
                
                srcFile.close();
                destFile.close();
                
                // done
                setResult(false, 0, "");
		    } catch(EOFException e) {
		    	// done, no error
                setResult(false, 0, "");
		    } catch(NullPointerException e) {
                setResult(true, FLibError.CFA_FileNotOpenError, FLibError.CFA_FileNotOpenErrorStr);
		    } catch(ThreadDeath e) {
		        if(ENABLE_DEBUG_PRINT)
		        	System.out.println("\t\tFLibReadFile - Timeout");
                setResult(true, FLibError.CFA_Timeout, FLibError.CFA_TimeoutStr);
		    	throw e;
		    } catch(Exception e) {
		        if(ENABLE_DEBUG_PRINT)
		        	System.out.println("\t\tFLibReadFile - Exception " + e.getMessage());
                if(m_strDestFilename == "") {
                    setResult(true, FLibError.CFA_InvalidArgument, FLibError.CFA_InvalidArgumentStr);
                } else {
                    setResult(true, FLibError.CFA_CommonError, e.getMessage());
                }
		    } finally {
		    	m_bResourceBusy.putBool(false);
		    }
        } else {
            // instace is locked; cannot copy file if open
            setResult(true, FLibError.CFA_CommonError, FLibError.CFA_CopyOpFailedStr);
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
