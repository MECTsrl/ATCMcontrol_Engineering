package COM.softing.fclib.flib;

import COM.softing.fc.CC.system.FcTask;
import COM.softing.fc.CC.util.*;
import java.io.*;

/**
 * class FLibMonitorThread
 */
public class FLibMonitorThread
extends Thread
{
	private static boolean ENABLE_DEBUG_PRINT = false;	// set false for release compilation
    
	public Thread m_tobj;
	private long m_lnTimeout;
	
	public FLibMonitorThread(Thread tobj, long lnTimeout)
	{
		m_tobj = tobj;
		m_lnTimeout = lnTimeout;
        setName("FLibMonitorThread");
	}

    public void finalize()
    {
        release();
    }
    
    public IFLib getWorkerThread()
    {
        return (IFLib)m_tobj;	
    }

	public void release()
	{
        if(m_tobj != null)
        {
            RandomAccessFile    file = null;
            file = ((IFLib)m_tobj).getFile();
            if( file != null) 
            {
                try 
                {
                    file.close();
                } 
                catch(Exception e)
                {
                    ;
                }
            }
		    m_tobj.stop();
		}
	}
    
    public void run()
    {
    // start file operation
        try {
            m_tobj.setPriority(MIN_PRIORITY);
            m_tobj.start();
            m_tobj.join(m_lnTimeout);
            m_tobj.stop();
		} catch(ThreadDeath e) {
		    if(ENABLE_DEBUG_PRINT)
		    	System.out.println("\t\tFLibMonitorThread - ThreadDeath");
            release();
            throw e;
        } catch(IllegalArgumentException e) {
            m_tobj.stop();
            ((IFLib)m_tobj).setResult(true, FLibError.CFA_Timeout, FLibError.CFA_TimeoutStr);
            if(ENABLE_DEBUG_PRINT)
		        System.out.println("\t\tFLibMonitorThread - IllegalArgumentException " + e.getMessage());
        } catch(Exception e) {
            m_tobj.stop();
            ((IFLib)m_tobj).setResult(true, FLibError.CFA_CommonError, e.getMessage());
            if(ENABLE_DEBUG_PRINT)
		        System.out.println("\t\tFLibMonitorThread - IllegalArgumentException " + e.getMessage());
        }
    }
}
