package COM.softing.fclib.flib;

import COM.softing.fc.CC.system.StructVar;
import COM.softing.fc.CC.util.*;
import COM.softing.fc.CC.utility.ByteString;
import java.io.*;

/**
 * class SyncFLib
 */
public class SyncFLib extends StructVar
{
    // set false for release compilation
	private static boolean ENABLE_DEBUG_PRINT = false;

	// File
	private RandomAccessFile    m_file;
	private	FcBool              m_bBusy         = new TempVarBool();

	// ST-Members:
    private FcString            m_strFilename;
    private int                 m_iMode;
    private boolean             m_bTextmode;
	private long                m_lTimeout;
    private FcInt               m_iPosition;
	private FcString            m_strLstop      = new TempVarString();
	private FcBool              m_bIserror;
	private	FcInt               m_iLsterrcd;
	private FcString            m_strLsterrtxt;
    private FcBool              m_bIsOpen;  // lock instance for 'createFile' unless 'closeFile' is called
	
	private	FcBool              m_bError        = new TempVarBool();
    private FcInt               m_iErrcd        = new TempVarInt();
	private TempVarString       m_strErrtxt     = new TempVarString();
        
    public SyncFLib()
    {
        m_strLstop.putString("");
        m_strErrtxt.putString("");
    }

	public boolean closeFile(boolean request)
	{
        if(request) {
            getStMembers();
            FLibCloseFile wthread = new FLibCloseFile(m_bBusy, m_bIsOpen, m_file);
            execute(wthread, FLibMode.CFA_SYNC_CLOSEFILE);
            m_file = wthread.getFile();
        } else {
            setError(false, 0, "");
        }
        return m_bError.getBool();
	} // closeFile
	
	public boolean createDir(boolean request)
	{
        if(request) {
            getStMembers();
            FLibCreateDir wthread = new FLibCreateDir(m_bBusy, m_strFilename.getString());
            execute(wthread, FLibMode.CFA_SYNC_CREATEDIRECTORY);
        } else {
            setError(false, 0, "");
        }
        return m_bError.getBool();
	} // createDir
	
	public boolean createFile(boolean request)
	{
        if(request) {
            getStMembers();
            FLibCreateFile wthread = new FLibCreateFile(m_bBusy,
                                                        m_bIsOpen,
                                                        m_file,
                                                        m_strFilename.getString(),
                                                        m_iMode);
            execute(wthread, FLibMode.CFA_SYNC_OPENFILE);
            m_file = wthread.getFile();
        } else {
            setError(false, 0, "");
        }
        return m_bError.getBool();
	} // createFile
	
	public boolean deleteFile(boolean request)
	{
        if(request) {
            getStMembers();
            FLibDeleteFile wthread = new FLibDeleteFile(m_bBusy, m_strFilename.getString());
            execute(wthread, FLibMode.CFA_SYNC_DELETEFILE);
        } else {
            setError(false, 0, "");
        }
        return m_bError.getBool();
	} // deleteFile
	
    private void execute(Thread wthread, String strFuncName)
    {
        try {
            if(m_lTimeout < 0) {
                throw new IllegalArgumentException();
            }
            wthread.start();
            wthread.join(m_lTimeout);
            wthread.stop();
        } catch(InterruptedException e) {
            setError(true, FLibError.CFA_InternalError, FLibError.CFA_InternalErrorStr);
        } catch(IllegalArgumentException e) {
            setError(true, FLibError.CFA_Timeout, FLibError.CFA_TimeoutStr);
        } catch(Exception e) {
            setError(true, FLibError.CFA_CommonError, e.getMessage());
        } finally {
            ((IFLib)wthread).getResult(m_bError, m_iErrcd, m_strErrtxt, m_iPosition);
            m_strLstop.putString(strFuncName);
            m_bIserror.putBool(m_bError.getBool());
            m_iLsterrcd.putInt(m_iErrcd.getInt());
            m_strLsterrtxt.putString(m_strErrtxt.getString());
        }
    } // execute
    
	public boolean existsFile(boolean request,
                           FcBool exists)
	{
        if(request) {
            getStMembers();
            FLibExistsFile wthread = new FLibExistsFile(m_bBusy, m_strFilename.getString());
            execute(wthread, FLibMode.CFA_SYNC_EXISTSFILE);
            wthread.getData((FcObject)exists);
        } else {
            setError(false, 0, "");
        }
        return m_bError.getBool();
	} // existsFile
	
	public boolean getSize(boolean request,
						FcLong size)
	{
        if(request) {
            getStMembers();
            FLibGetSize wthread = new FLibGetSize(m_bBusy, m_strFilename.getString());
            execute(wthread, FLibMode.CFA_SYNC_GETSIZE);
            wthread.getData((FcObject)size);
        } else {
            setError(false, 0, "");
        }
        return m_bError.getBool();
	} // getSize
	
	private void getStMembers()
	{
        m_strFilename   = ((FcString)locVars[0]);
        m_iMode         = ((FcInt)locVars[1]).getInt();
        m_bTextmode     = ((FcBool)locVars[2]).getBool();
        m_lTimeout      = ((FcLong)locVars[3]).getLong();
        m_iPosition     = (FcInt)locVars[4];
        m_strLstop      = (FcString)locVars[5];
        m_bIserror      = (FcBool)locVars[6];
        m_iLsterrcd     = (FcInt)locVars[7];
        m_strLsterrtxt  = (FcString)locVars[8];
        m_bIsOpen       = (FcBool)locVars[9];
	} // getStMembers
	
	public boolean readFile(boolean request,
						   FcInt length, 
						   FcObject data)
	{
        if(request) {
            getStMembers();
            FLibReadFile wthread = new FLibReadFile(m_bBusy,
                                                    m_file, 
                                                    m_bTextmode, 
                                                    m_iPosition.getInt(), 
                                                    length.getInt());
            execute(wthread, FLibMode.CFA_SYNC_READFILE);
            length.putInt(wthread.getData(data));
        } else {
            setError(false, 0, "");
        }
        return m_bError.getBool();
	} // readFile

	public void release()
	{
		super.release();
		if(m_file != null) {
            try {
                m_file.close();
            } catch(IOException e) {
                ;
            }
		}
	} // release
	
	public boolean renameFile(boolean request,
						   String strNewFilename)
	{
        if(request) {
            getStMembers();
            FLibRenameFile wthread = new FLibRenameFile(m_bBusy,
                                                        m_bIsOpen.getBool(),
                                                        m_strFilename.getString(),
                                                        strNewFilename);
            execute(wthread, FLibMode.CFA_SYNC_RENAMEFILE);
            wthread.getData((FcObject)m_strFilename);
        } else {
            setError(false, 0, "");
        }
        return m_bError.getBool();
	} // renameFile

    private void setError(boolean bError, int iErrcd, String strErrtxt)
    {
        m_bError.putBool(bError);
        m_iErrcd.putInt(iErrcd);
        m_strErrtxt.putString(strErrtxt);
    } // setError
    
	public boolean writeFile(boolean request,
							FcObject data)
	{
        if(request) {
            getStMembers();
            FLibWriteFile wthread = new FLibWriteFile(m_bBusy,
                                                      m_file,
                                                      m_bTextmode,
                                                      m_iPosition.getInt(),
                                                      data);
            execute(wthread, FLibMode.CFA_SYNC_WRITEFILE);
        } else {
            setError(false, 0, "");
        }
        return m_bError.getBool();
	} // writeFile
}
