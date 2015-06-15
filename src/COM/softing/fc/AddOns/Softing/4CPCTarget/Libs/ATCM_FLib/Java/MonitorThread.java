package COM.softing.fclib.flib;

import COM.softing.fc.CC.system.FcTask;
import COM.softing.fc.CC.util.*;

public class MonitorThread
extends Thread
{
	public Thread tobj;
	private long timeout;
	
	public MonitorThread(Thread tobj, long timeout)
	{
		this.tobj = tobj;
		this.timeout = timeout;
	}

	public IFileLib getWorkerThread()
	{
		return (IFileLib)tobj;	
	}
	
	public void run()
	{
		// start file operation
		try {
			System.out.println("\t\tMonitorThreadPriority: " + this.getPriority());
			tobj.setPriority(MIN_PRIORITY);
			System.out.println("\t\tWrokerThreadPriority: " + tobj.getPriority());
			long start;
			start = System.currentTimeMillis();
			tobj.start();
			tobj.join(timeout);
			tobj.stop();
			//System.out.println("\t\tdiff-all: " + (System.currentTimeMillis() - start) );
		} catch(Throwable e) {
			// NOP
		} finally {
			// debug
			System.out.println("\t\tMonitorThread.run().finally");
		}
	}
}
