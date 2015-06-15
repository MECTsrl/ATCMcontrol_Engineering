
package COM.softing.fclib.flib;

import COM.softing.fc.CC.system.StructVar;
import COM.softing.fc.CC.utility.ByteString;
import COM.softing.fc.CC.util.*;
import java.io.*;

/**
 * class FLibWriteFile
 */
public class FLibWriteFile
extends Thread
implements IFLib
{
	private FcBool              m_bResourceBusy;
	private RandomAccessFile    m_file;
	private boolean             m_bTextmode;
	private int                 m_iOldposition;
	private int                 m_iNewposition;
	private byte                m_byBuffer[];
	private boolean             m_error     = true;
	private String              m_errtxt    = FLibError.CFA_TimeoutStr;
	private int                 m_errcd     = FLibError.CFA_Timeout;

	public FLibWriteFile(FcBool bResourceBusy, RandomAccessFile file, boolean textmode, int position, FcObject data)
	{
		m_bResourceBusy = bResourceBusy;
		m_file = file;
		m_bTextmode = textmode;
		m_iOldposition = position;
		m_byBuffer = ((ByteString)data).getByteString();
        setName("FLibWriteFile");
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
		error.putBool(m_error);
		errcd.putInt(m_errcd);
		errtxt.putString(m_errtxt);
		position.putInt(m_iNewposition);
	}
	
	public void run()
	{
		long size;
		try {
			size = m_file.length();
			
			// set position
			if(m_iOldposition == FLibMode.CFA_EOF) {
				m_file.seek(size);
			} else {
				m_file.seek(m_iOldposition);
			}
			
			// write
			if(!m_bTextmode) {
				// write binary
				m_file.write(m_byBuffer);
			} else {
				// write text; expand escape-sequence
				int i;
				for(i=0; i<m_byBuffer.length; i++) {
					if(m_byBuffer[i] == FLibMode.CFA_NEWLINE) {
						// insert "carriage return"
						m_file.write(FLibMode.CFA_CARRIAGERETURN);
					}
					m_file.write(m_byBuffer[i]);
				}
				//m_file.flush();
			}
			m_iNewposition = (int)m_file.getFilePointer();

            setResult(false, 0, "");
		} catch(NullPointerException e) {
            setResult(true, FLibError.CFA_FileNotOpenError, FLibError.CFA_FileNotOpenErrorStr);
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