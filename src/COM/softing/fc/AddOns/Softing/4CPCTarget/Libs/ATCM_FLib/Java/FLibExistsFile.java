package COM.softing.fclib.flib;

import COM.softing.fc.CC.system.StructVar;
import COM.softing.fc.CC.utility.ByteString;
import COM.softing.fc.CC.util.*;
import java.io.*;

/**
 * class FLibExistsFile
 */
public class FLibExistsFile
extends Thread
implements IFLib, IFLibGetData
{
	private FcBool		m_bResourceBusy;
	private String		m_strFilename;
    private boolean     m_bExists;
	private boolean		m_error		= true;
	private String		m_errtxt	= FLibError.CFA_TimeoutStr;
	private int			m_errcd		= FLibError.CFA_Timeout;

	public FLibExistsFile(FcBool bResourceBusy, String strFilename)
	{
		m_bResourceBusy = bResourceBusy;
		m_strFilename = strFilename;
        setName("FLibExistsFile");
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
	
	public int getData(FcObject data)
	{
		((FcBool)data).putBool(m_bExists);
		return 0;
	}
	
	public void run()
	{
		try {
            File fileobj = new File(m_strFilename);
			m_bExists = fileobj.exists();

            // done
            setResult(false, 0, "");
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
