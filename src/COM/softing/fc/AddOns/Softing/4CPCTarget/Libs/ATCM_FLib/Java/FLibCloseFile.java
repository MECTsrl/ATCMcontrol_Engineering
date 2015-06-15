package COM.softing.fclib.flib;

import COM.softing.fc.CC.system.StructVar;
import COM.softing.fc.CC.utility.ByteString;
import COM.softing.fc.CC.util.*;
import java.io.*;

/**
 * class FLibCloseFile
 */
public class FLibCloseFile
extends Thread
implements IFLib
{
	private static boolean ENABLE_DEBUG_PRINT = false;	// set false for release compilation
	public	long		debug_m_starttime = System.currentTimeMillis();
	
	private FcBool              m_bResourceBusy;
    private FcBool              m_bIsOpen;
	private RandomAccessFile    m_file;
	private boolean             m_error     = true;
	private String              m_errtxt    = FLibError.CFA_TimeoutStr;
	private int                 m_errcd     = FLibError.CFA_Timeout;

	public FLibCloseFile(FcBool bResourceBusy, FcBool bFileOpen, RandomAccessFile file)
	{
		m_bResourceBusy = bResourceBusy;
        m_bIsOpen = bFileOpen;
		m_file = file;
        setName("FLibCloseFile");
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
			System.out.println("\t\tFLibCloseFile.getResult() - error: " + m_error);
        }
		error.putBool(m_error);
		errcd.putInt(m_errcd);
		errtxt.putString(m_errtxt);
	}
	
	
	public void run()
	{
		debug_m_starttime = System.currentTimeMillis();
		try {
			m_file.close();
		    	
			// done
            setResult(false, 0, "");
            
            // unlock instance for 'createFile'
            m_bIsOpen.putBool(false);
		} catch(NullPointerException e) {
			// there is no file to be closed; no error
            setResult(false, 0, "");
            m_bIsOpen.putBool(false);
		} catch(ThreadDeath e) {
			if(ENABLE_DEBUG_PRINT)
				System.out.println("\t\tFLibCloseFile - Timeout");
            setResult(true, FLibError.CFA_Timeout, FLibError.CFA_TimeoutStr);
			throw e;
		} catch(Exception e) {
		    if(ENABLE_DEBUG_PRINT)
		    	System.out.println("\t\tFLibCloseFile - Exception " + e.getMessage());
            setResult(true, FLibError.CFA_CommonError, e.getMessage());
		} finally {
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
