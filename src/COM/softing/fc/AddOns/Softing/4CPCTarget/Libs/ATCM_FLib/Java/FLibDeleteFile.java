package COM.softing.fclib.flib;

import COM.softing.fc.CC.system.StructVar;
import COM.softing.fc.CC.utility.ByteString;
import COM.softing.fc.CC.util.*;
import java.io.*;

/**
 * class FLibDeleteFile
 */
public class FLibDeleteFile
extends Thread
implements IFLib
{
	private FcBool		m_bResourceBusy;
	private String		m_strFilename;
	private boolean		m_error		= true;
	private String		m_errtxt	= FLibError.CFA_TimeoutStr;
	private int			m_errcd		= FLibError.CFA_Timeout;

	public FLibDeleteFile(FcBool bResourceBusy, String filename)
	{
		m_bResourceBusy = bResourceBusy;
		m_strFilename = filename;
        setName("FLibDeleteFile");
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
		try {
            File fileobj = new File(m_strFilename);
            
            // is writeable ?
            if(!fileobj.canWrite() & fileobj.exists()) {
                setResult(true, FLibError.CFA_CommonError, FLibError.CFA_FileIsWriteprotected + "; " + m_strFilename);
                return;
            }
            
            // is directory ?
            if(fileobj.isDirectory()) {
                String strDirList[] = fileobj.list();
                // is empty ?
                if(strDirList.length != 0) {
                    setResult(true, FLibError.CFA_CommonError, FLibError.CFA_DirectoryIsNotEmpty + "; " + m_strFilename);
                    return;
                }
            }
            
            // delete
			if( !fileobj.delete() )
            {
                // operation failed
                setResult(true, FLibError.CFA_CommonError, FLibError.CFA_DeleteOpFailedStr);
                return;
            } else {
                // operation succeeded
                setResult(false, 0, "");
                return;
            }
		} catch(ThreadDeath e) {
            setResult(true, FLibError.CFA_Timeout, FLibError.CFA_TimeoutStr);
			throw e;
		} catch(Exception e) {
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
