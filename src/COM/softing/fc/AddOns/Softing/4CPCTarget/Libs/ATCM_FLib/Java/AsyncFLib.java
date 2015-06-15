package COM.softing.fclib.flib;

import COM.softing.fc.CC.system.StructVar;
import COM.softing.fc.CC.util.*;
import COM.softing.fc.CC.utility.ByteString;
import java.io.*;

/**
 * class AsyncFLib
 */
public class AsyncFLib extends StructVar
{
    private static boolean ENABLE_DEBUG_PRINT = false;	// set false for release compilation

    private static final int ASYNC_OP_EXECUTE = 1;
    private static final int ASYNC_OP_GETRESULT = 2;
        
    private RandomAccessFile    m_file = null;
    private Thread              m_monitorThread = null;

    // ST-Members:
    private FcString            st_filename;
    private int                 st_mode;
    private boolean             st_textmode;
    private long                st_timeout;
    private FcInt               st_position;
    private FcBool              st_iserror;
    private FcInt               st_lsterrcd;
    private FcString            st_lsterrtxt;
    
    public  FcBool              m_bResBusy  = new TempVarBool();    // resource busy
    private boolean             m_bToken    = true;

    private FcBool              m_bError    = new TempVarBool();
    private FcInt               m_iErrcd    = new TempVarInt();
    private FcString            m_strErrtxt = new TempVarString();
    private FcBool              m_bIsOpen   = new TempVarBool();

    public AsyncFLib()
    {
        m_strErrtxt.putString("");
    }

    private int checkopstat(boolean bExec, boolean bTrig, FcBool bToken)
    {
        // init error
        if(!bExec) {
            setError(false, 0, "");
        }
        
        // is resource busy ?
        if( bExec && !m_bResBusy.getBool() ) {
            // resource not busy
            if(!bToken.getBool() && bTrig) {
                // try to catch the Token
                bToken.putBool( getToken() );
                if(bToken.getBool()) {
                    // got Token
                    // start operation with positive edege
                    return ASYNC_OP_EXECUTE;
                }
            } else if(bToken.getBool()) {
                // get result
                putToken(true);
                bToken.putBool(false);
                return ASYNC_OP_GETRESULT;
            }
        } else if(bToken.getBool() && !m_bResBusy.getBool() ) {
            // get result
            putToken(true);
            bToken.putBool(false);
            return ASYNC_OP_GETRESULT;
        }
        return 0;
    } // checkopstat
    
	public boolean closeFile(boolean bExec,
                           boolean bTrig,
                           FcBool bToken,
                           FunctionBlock fb,
                           FcBool bError,
                           FcInt iErrcd,
                           FcString strErrtxt) // returns done
	{
        boolean bDone = false;
        init(bExec, bError, iErrcd, strErrtxt);
        int iState = checkopstat(bExec, bTrig, bToken);
        if(iState == ASYNC_OP_EXECUTE) {
            // start operation
            if( bDone = execute(new FLibCloseFile(m_bResBusy, m_bIsOpen, m_file)) ) {
                setStMembers(FLibMode.CFA_ASYNC_CLOSEFILE, fb);
                getError(bError, iErrcd, strErrtxt);
                putToken(bDone);        // if error, give token away
                bToken.putBool(!bDone);                
            }
        } else if(iState == ASYNC_OP_GETRESULT) {
            // get result
            getFile();
            getResult();
            setStMembers(FLibMode.CFA_ASYNC_CLOSEFILE, fb);
            getError(bError, iErrcd, strErrtxt);
            bDone = true;
        } else if(m_bResBusy.getBool()) {
            // still working
            bDone = false;
        } else {
            // nothing to do
            bDone = bExec || false;
        }
        
        return bDone;
	} // closeFile
	
	public boolean copyFile(boolean bExec,
                           boolean bTrig,
                           FcBool bToken,
                           FunctionBlock fb,
                           String strDestFile,
                           boolean bOverwrite,
                           FcBool bError,
                           FcInt iErrcd,
                           FcString strErrtxt) // returns done
	{
        boolean bDone = false;
        init(bExec, bError, iErrcd, strErrtxt);
        int iState = checkopstat(bExec, bTrig, bToken);
        if(iState == ASYNC_OP_EXECUTE) {
            // start operation
            if( bDone = execute(new FLibCopyFile(m_bResBusy, m_bIsOpen.getBool(), st_filename.getString(), strDestFile, bOverwrite)) ) {
                setStMembers(FLibMode.CFA_ASYNC_COPYFILE, fb);
                getError(bError, iErrcd, strErrtxt);
                putToken(bDone);        // if error, give token away
                bToken.putBool(!bDone);                
            }
        } else if(iState == ASYNC_OP_GETRESULT) {
            // get result
            getResult();
            setStMembers(FLibMode.CFA_ASYNC_COPYFILE, fb);
            getError(bError, iErrcd, strErrtxt);
            bDone = true;
        } else if(m_bResBusy.getBool()) {
            // still working
            bDone = false;
        } else {
            // nothing to do
            bDone = bExec || false;
        }
        
        return bDone;
	} // copyFile
    
	public boolean createDir(boolean bExec,
                           boolean bTrig,
                           FcBool bToken,
                           FunctionBlock fb,
                           FcBool bError,
                           FcInt iErrcd,
                           FcString strErrtxt) // returns done
	{
        boolean bDone = false;
        init(bExec, bError, iErrcd, strErrtxt);
        int iState = checkopstat(bExec, bTrig, bToken);
        if(iState == ASYNC_OP_EXECUTE) {
            // start operation
            if( bDone = execute(new FLibCreateDir(m_bResBusy, st_filename.getString())) ) {
                setStMembers(FLibMode.CFA_ASYNC_CREATEDIRECTORY, fb);
                getError(bError, iErrcd, strErrtxt);
                putToken(bDone);        // if error, give token away
                bToken.putBool(!bDone);                
            }
        } else if(iState == ASYNC_OP_GETRESULT) {
            // get result
            getResult();
            setStMembers(FLibMode.CFA_ASYNC_CREATEDIRECTORY, fb);
            getError(bError, iErrcd, strErrtxt);
            bDone = true;
        } else if(m_bResBusy.getBool()) {
            // still working
            bDone = false;
        } else {
            // nothing to do
            bDone = bExec || false;
        }
        
        return bDone;
	} // createDir

	public boolean createFile(boolean bExec,
                           boolean bTrig,
                           FcBool bToken,
                           FunctionBlock fb,
                           FcBool bError,
                           FcInt iErrcd,
                           FcString strErrtxt) // returns done
	{
        boolean bDone = false;
        init(bExec, bError, iErrcd, strErrtxt);
        int iState = checkopstat(bExec, bTrig, bToken);
        if(iState == ASYNC_OP_EXECUTE) {
            // start operation
            if( bDone = execute(new FLibCreateFile(m_bResBusy, m_bIsOpen, m_file, st_filename.getString(), st_mode)) ) {
                setStMembers(FLibMode.CFA_ASYNC_OPENFILE, fb);
                getError(bError, iErrcd, strErrtxt);
                putToken(bDone);        // if error, give token away
                bToken.putBool(!bDone);                
            }
        } else if(iState == ASYNC_OP_GETRESULT) {
            // get result
            getFile();
            getResult();
            setStMembers(FLibMode.CFA_ASYNC_OPENFILE, fb);
            getError(bError, iErrcd, strErrtxt);
            bDone = true;
        } else if(m_bResBusy.getBool()) {
            // still working
            bDone = false;
        } else {
            // nothing to do
            bDone = bExec || false;
        }
        
        return bDone;
	} // createFile

	public boolean deleteFile(boolean bExec,
                           boolean bTrig,
                           FcBool bToken,
                           FunctionBlock fb,
                           FcBool bError,
                           FcInt iErrcd,
                           FcString strErrtxt) // returns done
	{
        boolean bDone = false;
        init(bExec, bError, iErrcd, strErrtxt);
        int iState = checkopstat(bExec, bTrig, bToken);
        if(iState == ASYNC_OP_EXECUTE) {
            // start operation
            if( bDone = execute(new FLibDeleteFile(m_bResBusy, st_filename.getString())) ) {
                setStMembers(FLibMode.CFA_ASYNC_DELETEFILE, fb);
                getError(bError, iErrcd, strErrtxt);
                putToken(bDone);        // if error, give token away
                bToken.putBool(!bDone);                
            }
        } else if(iState == ASYNC_OP_GETRESULT) {
            // get result
            getResult();
            setStMembers(FLibMode.CFA_ASYNC_DELETEFILE, fb);
            getError(bError, iErrcd, strErrtxt);
            bDone = true;
        } else if(m_bResBusy.getBool()) {
            // still working
            bDone = false;
        } else {
            // nothing to do
            bDone = bExec || false;
        }
        
        return bDone;
	} // deleteFile
	
    private boolean execute(Thread wthread) // returns 'error success'
    {
        boolean bErrorSuccess;
		try {
			// create new thread
			if(ENABLE_DEBUG_PRINT) {
				System.out.println("\tAsyncFLib.execute() " + wthread.getName() );
			}
            
            if(st_timeout < 0) {
                throw new IllegalArgumentException();
            }
			// set flags
			m_bResBusy.putBool(true);
			m_monitorThread = new FLibMonitorThread(wthread, st_timeout);
			// start thread
			m_monitorThread.start();
            bErrorSuccess = false;
        } catch(IllegalArgumentException e) {
            setError(true, FLibError.CFA_Timeout, FLibError.CFA_TimeoutStr);
            if(ENABLE_DEBUG_PRINT) {
                System.out.println("\tAsyncFLib.execute() - Error: " + FLibError.CFA_TimeoutStr);
            }
            bErrorSuccess = true;
		} catch(Exception e) {
            setError(true, FLibError.CFA_CommonError, e.getMessage());
			if(ENABLE_DEBUG_PRINT) {
                System.out.println("\tAsyncFLib.execute() - Error: " + e.getMessage());
            }
            bErrorSuccess = true;
		}
        return bErrorSuccess;
    } // execute
    
	public boolean existsFile(boolean bExec,
                           boolean bTrig,
                           FcBool bToken,
                           FunctionBlock fb,
                           FcBool data,
                           FcBool bError,
                           FcInt iErrcd,
                           FcString strErrtxt) // returns done
	{
        boolean bDone = false;
        init(bExec, bError, iErrcd, strErrtxt);
        int iState = checkopstat(bExec, bTrig, bToken);
        if(!bExec)
        {
            // delete output
            ((FcBool)data).putBool(false);
        }
        if(iState == ASYNC_OP_EXECUTE) {
            // start operation
            if( bDone = execute(new FLibExistsFile(m_bResBusy, st_filename.getString())) ) {
                setStMembers(FLibMode.CFA_ASYNC_EXISTSFILE, fb);
                getError(bError, iErrcd, strErrtxt);
                putToken(bDone);        // if error, give token away
                bToken.putBool(!bDone);                
            }
        } else if(iState == ASYNC_OP_GETRESULT) {
            // get result
            getData((FcObject)data);
            setStMembers(FLibMode.CFA_ASYNC_EXISTSFILE, fb);
            getError(bError, iErrcd, strErrtxt);
            bDone = true;
        } else if(m_bResBusy.getBool()) {
            // still working
            bDone = false;
        } else {
            // nothing to do
            bDone = bExec || false;
        }
        
        return bDone;
	} // getSize

	public int getData(FcObject data)
	{
        if(m_monitorThread != null && !m_bError.getBool()) {
       		return ((IFLibGetData)((FLibMonitorThread)m_monitorThread).m_tobj).getData(data);
        } else {
            return 0;
        }
	} // getData
	
    private void getError(FcBool bError, FcInt iErrcd, FcString strErrtxt)
    {
        bError.putBool(m_bError.getBool());
        iErrcd.putInt(m_iErrcd.getInt());
        strErrtxt.putString(m_strErrtxt.getString());
    } // getError
    
    public void getFile()
    {
        if(m_bIsOpen.getBool()) {
            m_file = ((FLibMonitorThread)m_monitorThread).getWorkerThread().getFile();
        }
    } // getFile
	
	public void getResult()
	{
        if(m_monitorThread != null && !m_bError.getBool()) {
		    ( ((FLibMonitorThread)m_monitorThread).getWorkerThread() ).getResult(m_bError, m_iErrcd, m_strErrtxt, st_position);
        }
	} // getResult
	
	public void getResult(FcBool error, FcInt errcd, FcString errtxt)
	{
        if(m_monitorThread != null) {
    		( ((FLibMonitorThread)m_monitorThread).getWorkerThread() ).getResult(error, errcd, errtxt, st_position);
        }
	} // getResult
	
	public boolean getSize(boolean bExec,
                           boolean bTrig,
                           FcBool bToken,
                           FunctionBlock fb,
                           FcObject data,
                           FcBool bError,
                           FcInt iErrcd,
                           FcString strErrtxt) // returns done
	{
        boolean bDone = false;
        init(bExec, bError, iErrcd, strErrtxt);
        int iState = checkopstat(bExec, bTrig, bToken);
        if(!bExec)
        {
            // delete output
            ((FcLong)data).putLong(0);
        }
        if(iState == ASYNC_OP_EXECUTE) {
            // start operation
            if( bDone = execute(new FLibGetSize(m_bResBusy, st_filename.getString())) ) {
                setStMembers(FLibMode.CFA_ASYNC_GETSIZE, fb);
                getError(bError, iErrcd, strErrtxt);
                putToken(bDone);        // if error, give token away
                bToken.putBool(!bDone);                
            }
        } else if(iState == ASYNC_OP_GETRESULT) {
            // get result
            getData((FcObject)data);
            getResult();
            setStMembers(FLibMode.CFA_ASYNC_GETSIZE, fb);
            getError(bError, iErrcd, strErrtxt);
            bDone = true;
        } else if(m_bResBusy.getBool()) {
            // still working
            bDone = false;
        } else {
            // nothing to do
            bDone = bExec || false;
        }
        
        return bDone;
	} // getSize

	private void getStMembers()
	{
		st_filename		= ((FcString)locVars[0]);
		st_mode			= ((FcInt)locVars[1]).getInt();
		st_textmode		= ((FcBool)locVars[2]).getBool();
		st_timeout		= ((FcLong)locVars[3]).getLong();
		st_position		= (FcInt)locVars[4];
		st_iserror		= (FcBool)locVars[7];
		st_lsterrcd		= (FcInt)locVars[8];
		st_lsterrtxt	= (FcString)locVars[9];
	} // getStMembers
	
	private boolean getToken()
	{
		boolean result;
		if(m_bToken) {
			// give token away
			m_bToken = false;
			result = true;
		} else {
			// sorry, there is no token
			result = false;
		}
		return result;
	} // getToken
	
    private void init(boolean bExec,
                           FcBool error,
						   FcInt errcd,
						   FcString errtxt)
    {
        if(!bExec) {
            // delete output
		    error.putBool(false);
		    errcd.putInt(0);
		    errtxt.putString("");
        } else {
            getStMembers();
        }
    } // init
    
	private void putToken(boolean token)
	{
		if(token && !this.m_bToken) {
			this.m_bToken = true;
		} else {
			if(ENABLE_DEBUG_PRINT)
				System.out.println("\tAsyncFlib: token error!");
		}
	} // putToken
	
	public boolean readFile(boolean bExec,
                           boolean bTrig,
                           FcBool bToken,
                           FunctionBlock fb,
                           FcObject data,
                           FcInt lLength,
                           FcBool bError,
                           FcInt iErrcd,
                           FcString strErrtxt) // returns done
	{
        boolean bDone = false;
        init(bExec, bError, iErrcd, strErrtxt);
        int iState = checkopstat(bExec, bTrig, bToken);
        if(!bExec)
        {
            // delete output
            ((ByteString)data).initFromString("");
        }
        if(iState == ASYNC_OP_EXECUTE) {
            // start operation
            if( bDone = execute(new FLibReadFile(m_bResBusy, m_file, st_textmode, st_position.getInt(), lLength.getInt())) ) {
                setStMembers(FLibMode.CFA_ASYNC_READFILE, fb);
                getError(bError, iErrcd, strErrtxt);
                putToken(bDone);        // if error, give token away
                bToken.putBool(!bDone);                
            }
        } else if(iState == ASYNC_OP_GETRESULT) {
            // get result
            lLength.putInt(getData((FcObject)data));
            getResult();
            setStMembers(FLibMode.CFA_ASYNC_READFILE, fb);
            getError(bError, iErrcd, strErrtxt);
            bDone = true;
        } else if(m_bResBusy.getBool()) {
            // still working
            bDone = false;
        } else {
            // nothing to do
            bDone = bExec || false;
        }
        
        return bDone;
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
        if(m_monitorThread != null) {
            ((FLibMonitorThread)m_monitorThread).release();
            m_monitorThread.stop();
		}
	} // release
	
	public boolean renameFile(boolean bExec,
                           boolean bTrig,
                           FcBool bToken,
                           FunctionBlock fb,
                           String strNewFilename,
                           FcBool bError,
                           FcInt iErrcd,
                           FcString strErrtxt) // returns done
	{
        boolean bDone = false;
        init(bExec, bError, iErrcd, strErrtxt);
        int iState = checkopstat(bExec, bTrig, bToken);
        if(iState == ASYNC_OP_EXECUTE) {
            // start operation
            if( bDone = execute(new FLibRenameFile(m_bResBusy, m_bIsOpen.getBool(), st_filename.getString(), strNewFilename)) ) {
                setStMembers(FLibMode.CFA_ASYNC_RENAMEFILE, fb);
                getError(bError, iErrcd, strErrtxt);
                putToken(bDone);        // if error, give token away
                bToken.putBool(!bDone);                
            }
        } else if(iState == ASYNC_OP_GETRESULT) {
            // get result
            getData((FcObject)st_filename);
            getResult();
            setStMembers(FLibMode.CFA_ASYNC_RENAMEFILE, fb);
            getError(bError, iErrcd, strErrtxt);
            bDone = true;
        } else if(m_bResBusy.getBool()) {
            // still working
            bDone = false;
        } else {
            // nothing to do
            bDone = bExec || false;
        }
        
        return bDone;
	} // renameFile

    private void setError(boolean bError, int iErrcd, String strErrtxt)
    {
        m_bError.putBool(bError);
        m_iErrcd.putInt(iErrcd);
        m_strErrtxt.putString(strErrtxt);
    } // setError
    
    private void setStMembers(String strLstOp, FunctionBlock fb)
    {
		((FcInt)locVars[4]).putInt(st_position.getInt()); // position
		((FcString)locVars[5]).putString(strLstOp); // last operation
		((FcString)locVars[6]).putString(fb.getOD().getName()); // last instance
		((FcBool)locVars[7]).putBool(m_bError.getBool()); // is error
		((FcInt)locVars[8]).putInt(m_iErrcd.getInt()); // last errorcode
		((FcString)locVars[9]).putString(m_strErrtxt.getString()); // last errortext
		((FcBool)locVars[10]).putBool(m_bIsOpen.getBool()); // is open
    } // setStMembers
    
	public boolean writeFile(boolean bExec,
                           boolean bTrig,
                           FcBool bToken,
                           FunctionBlock fb,
                           FcObject data,
                           FcBool bError,
                           FcInt iErrcd,
                           FcString strErrtxt) // returns done
	{
        boolean bDone = false;
        init(bExec, bError, iErrcd, strErrtxt);
        int iState = checkopstat(bExec, bTrig, bToken);
        if(iState == ASYNC_OP_EXECUTE) {
            // start operation
            if( bDone = execute(new FLibWriteFile(m_bResBusy, m_file, st_textmode, st_position.getInt(), data)) ) {
                setStMembers(FLibMode.CFA_ASYNC_WRITEFILE, fb);
                getError(bError, iErrcd, strErrtxt);
                putToken(bDone);        // if error, give token away
                bToken.putBool(!bDone);                
            }
        } else if(iState == ASYNC_OP_GETRESULT) {
            // get result
            getResult();
            setStMembers(FLibMode.CFA_ASYNC_WRITEFILE, fb);
            getError(bError, iErrcd, strErrtxt);
            bDone = true;
        } else if(m_bResBusy.getBool()) {
            // still working
            bDone = false;
        } else {
            // nothing to do
            bDone = bExec || false;
        }
        
        return bDone;
	} // writeFile
}