package COM.softing.fclib.flib;

import COM.softing.fc.CC.system.StructVar;
import COM.softing.fc.CC.utility.ByteString;
import COM.softing.fc.CC.util.*;
import java.io.*;

/**
 * class FLibReadFile
 */
public class FLibReadFile
extends Thread
implements IFLib, IFLibGetData
{
	private static boolean ENABLE_DEBUG_PRINT = false;	// set false for release compilation
	
	private FcBool              m_bResourceBusy;
	private RandomAccessFile    m_file;
	private boolean             m_bTextmode;
	private byte                m_byBuffer[];
	private int                 m_iLength;
	private int                 m_iPosition	= 0;
	private boolean             m_error		= true;
	private String              m_errtxt	= FLibError.CFA_TimeoutStr;
	private int                 m_errcd		= FLibError.CFA_Timeout;
	
	public FLibReadFile(FcBool bResourceBusy, RandomAccessFile file, boolean textmode, int position, int iLength)
	{
		m_bResourceBusy = bResourceBusy;
		m_file = file;
		m_bTextmode = textmode;
		m_iPosition = position;
		m_iLength = iLength;
        setName("FLibReadFile");
	}
	
	public RandomAccessFile getFile()
	{
		return m_file;
	}
	
	public int getData(FcObject data)
	{
		if(m_byBuffer != null) {
			((ByteString)data).setByteString(m_byBuffer);
			return m_byBuffer.length;
		} else {
			((ByteString)data).initFromString("");
			return 0;
		}
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
		position.putInt(m_iPosition);
	}
	
	
	public void run()
	{
		long lSize;
		try {
			lSize = m_file.length();
			
			// set position
			if(m_iPosition >= lSize) {
				m_iPosition = FLibMode.CFA_EOF;
				m_file.seek(lSize);
				m_iLength = 0;

				// done
				m_errtxt = "";
				m_errcd = 0;
				m_error = false;
				return;
			} else if(m_iPosition == FLibMode.CFA_EOF) {
				m_iLength = 0;				

				// done
				m_errtxt = "";
				m_errcd = 0;
				m_error = false;
				return;
			} else {
				m_file.seek(m_iPosition);
			}
			
			// read
			if(m_iLength == FLibMode.CFA_EOL) {
				// read one single line
				byte felix;
				int i = 0;
				byte[] byBuffer = new byte[(int)lSize];
				try {
					// read until '\r' AND until EOF = 0xFF = -1
					while( (felix = (byte)m_file.read()) != FLibMode.CFA_CARRIAGERETURN && felix != -1 ) {
						byBuffer[i++] = felix;
					}
					if(m_bTextmode)
						m_file.read(); // jump over '\n'
				} catch (EOFException e) {
					// NOP
				} finally {
					m_byBuffer = new byte[i];
					System.arraycopy(byBuffer, 0, m_byBuffer, 0, i);
				}
			} else {
				// set length and read
				
				// set length
				if(m_iLength == FLibMode.CFA_EOF) {
					// read until 'End Of File'
					m_iLength = (int)lSize - m_iPosition;
				} else if(lSize - m_iPosition < m_iLength) {
					// don't read over 'End Of File'
					m_iLength = (int)lSize - m_iPosition;
				}
				
				// read
				int i = 0;
				if(!m_bTextmode && m_iLength != 0) {
					// read binary
                    m_byBuffer = new byte[m_iLength];
					i = m_file.read(m_byBuffer, 0, m_iLength);
				} else if(m_bTextmode && m_iLength != 0) {
					// read text; convert escape-sequence
					int offset = 0;
    				byte [] byBuffer = new byte[m_iLength];
					byBuffer[i++] = (byte)m_file.read();
					for(; i+offset < m_iLength; i++) {
						byBuffer[i] = (byte)m_file.read();
						if(byBuffer[i-1] == FLibMode.CFA_CARRIAGERETURN && byBuffer[i] == 10) {
							// jump over "cariage return"
							byBuffer[i-1] = byBuffer[i--];	// replace 'cr' with 'nl'
							offset++;
						} else if(byBuffer[i-1] == -1) {
							// found "EOF"
							i--;
							break;
						}
					}
					m_byBuffer = new byte[i];
					System.arraycopy(byBuffer, 0, m_byBuffer, 0, i);
				}
				if(ENABLE_DEBUG_PRINT) {
					System.out.println("\t\tm_iLength: " + i);
				}
			}
			
			m_iPosition = (int)m_file.getFilePointer();
			if(m_iPosition == lSize)
				m_iPosition = FLibMode.CFA_EOF;

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
