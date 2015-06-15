package COM.softing.fclib.flib;

import COM.softing.fc.CC.system.StructVar;
import COM.softing.fc.CC.utility.ByteString;
import COM.softing.fc.CC.util.*;
import java.io.*;

/**
 * class FLibDeleteFile
 */
public class FLibRenameFile
extends Thread
implements IFLib, IFLibGetData
{
    private FcBool      m_bResourceBusy;
    private boolean     m_bIsOpen;
    private String      m_strOldFilename;
    private String      m_strNewFilename;
    private boolean     m_error     = true;
    private String      m_errtxt    = FLibError.CFA_TimeoutStr;
    private int         m_errcd     = FLibError.CFA_Timeout;

    public FLibRenameFile(FcBool bResourceBusy,
                          boolean bFileOpen,
                          String strOldFilename, 
                          String strNewFilename)
    {
        m_bResourceBusy = bResourceBusy;
        m_bIsOpen = bFileOpen;
        m_strOldFilename = strOldFilename;
        m_strNewFilename = strNewFilename;
        setName("FLibRenameFile");
    }
	
    public int getData(FcObject data)
    {
        ((FcString)data).putString(m_strOldFilename);
        return 0;
    }
	
    public RandomAccessFile getFile()
    {
        return null;
    }
	
    public void getResult(FcBool error,
                            FcInt errcd,
                            FcString errtxt,
                            FcInt position)
    {
        error.putBool(m_error);
        errcd.putInt(m_errcd);
        errtxt.putString(m_errtxt);
    }
	
    public void run()
    {
        if(!m_bIsOpen) {
            // rename file
            try {
                File oldfileobj = new File(m_strOldFilename);
                File newfileobj = new File(m_strNewFilename);
                if( !oldfileobj.renameTo(newfileobj) ) {
                    // operation failed
                    setResult(true, FLibError.CFA_CommonError, FLibError.CFA_RenameOpFailedStr);
                } else {
                    // operation succeeded
                    setResult(false, 0, "");
                    m_strOldFilename = m_strNewFilename;
               }
            } catch(ThreadDeath e) {
                setResult(true, FLibError.CFA_Timeout, FLibError.CFA_TimeoutStr);
                throw e;
            } catch(Exception e) {
                setResult(true, FLibError.CFA_CommonError, e.getMessage());
            } finally {
                m_bResourceBusy.putBool(false);
            }
        } else {
            // instace is locked; cannot rename file if open
		    m_error = true;
		    m_errcd = FLibError.CFA_CommonError;
		    m_errtxt = FLibError.CFA_RenameOpFailedStr;
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
