/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpUtil/CP_Debug.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_Debug.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpUtil/CP_Debug.java $
 *
 * =PROJECT             7VH102  ATCMControl
 *
 * =SWKE                CP
 *
 * =COMPONENT           graphic
 *
 * =CURRENT             $Date: 28.02.07 19:00 $
 *                      $Revision: 1 $
 *
 * =ENVIRONMENT         Win32:
 *                      MS Windows NT 4.0
 *                      MSVC++ V5.0 / MFC 4.2 / ATL 2.0 / DAO 3.5
 *                      MS Java 2.0
 *                      MS Visual SourceSafe 5.0
 *
 * =REFERENCES
 *==
 *----------------------------------------------------------------------------*
 *

 

 

 *==
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATIONS
 * $History: CP_Debug.java $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 1  *****************
 * User: Scd          Date: 20.03.01   Time: 9:27
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:13
 * Created in $/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 3  *****************
 * User: Ff           Date: 29.04.98   Time: 12:42
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 * 
 * *****************  Version 2  *****************
 * User: Ff           Date: 23.04.98   Time: 10:16
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 *==
 ******************************************************************************
H<<*/

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.lang.reflect.*;

//================================================================================
/** An ínterface for debug ouput. */
//================================================================================
public class CP_Debug 
{	
	protected TextArea m_debugOut;

	//============================================================
	/** Constructs a new debug object with the specified title.
	* @param title 
	*/
	//============================================================
	public CP_Debug(String title) {
		//=== create a new frame
		final Frame debugFrame = new Frame(title);

		//=== create a new textarea
		m_debugOut = new TextArea();

		//=== add textarea to frame
		debugFrame.setLayout(new GridLayout(1,1));
		debugFrame.add(m_debugOut);

		//=== add an window listener to frame
		debugFrame.addWindowListener(new WindowListener() 
		{
			public void windowDeactivated(WindowEvent e) {}
			public void windowClosed(WindowEvent e) {}
			public void windowOpened(WindowEvent e) {}
			public void windowClosing(WindowEvent e) 
			{
				debugFrame.setVisible(false);
				debugFrame.dispose();
			}
			public void windowDeiconified(WindowEvent e) {}
			public void windowActivated(WindowEvent e) {}
			public void windowIconified(WindowEvent e) {}
		});
	
		//=== pack and display
		debugFrame.pack();
		debugFrame.setVisible(true);
	}

	//============================================================
	/** write a text to the debug output.
	* @param text
	*/
	//============================================================
	public void write(String text)
	{
		//=== append to text to the textarea
		m_debugOut.append(text);
	}

	//============================================================
	/** write a line to the debug output.
	*/
	//============================================================
	public void writeln(String text)
	{
		write(text + "\n");
	}

	//============================================================
	/** check an assertion and halt program if assertion fails.
	* @param condition
	* @param errMsg
	*/
	//============================================================
	public void assert(boolean condition, String errMsg)
	{
		if ( condition )
		{
			//=== do nothing
		}
		else 
		{
			writeln("Assertion failed: " + errMsg);
			try { throw new Exception(); }
			catch(Exception e) { writeStackTrace(e); }
			assertionFailed();
		}
	}

	//============================================================
	/** assert a precondition and halt program if assertion fails.
	* @param precondition
	* @param msg
	*/
	//============================================================
	public void assertPre(boolean precondition, String msg)
	{
		assert(precondition, "invalid precondition " + msg);
	}

	//============================================================
	/** assert an invariant and halt program if assertion fails.
	* @param invariant
	* @param msg
	*/
	//============================================================
	public void assertInv(boolean invariant, String msg)
	{
		assert(invariant, "invalid invariant "+ msg);
	}

	//============================================================
	/** assert a postcondition and halt program if assertion fails.
	* @param postcondition
	* @param msg
	*/
	//============================================================
	public void assertPost(boolean postcondition, String  msg)
	{
		assert(postcondition, "invalid postcondition " + msg);
	}

	//============================================================
	/** test a condition and write a line with a given error message to the debug output.
	* @param condition
	* @param errType
	* @param errPlace
	*/
	//============================================================
	public boolean check(boolean condition, String errMsg)
	{
		//=== if condition is true
		if ( condition ) {
			//=== do nothing
		}
		//=== if condition is false
		else {
			//=== write a line with error message to debug output
			writeln("Check failed: " + errMsg);
			checkFailed();
		}

		return condition;
	}

	//============================================================
	/** test an invariant.
	* @param invariant
	* @param msg
	*/
	//============================================================
	public void checkInv(boolean invariant, String msg)
	{
		check(invariant, "invalid invariant " + msg);
	}

	//============================================================
	/** check a precondition.
	* @param precondition
	* @param msg
	*/
	//============================================================
	public void checkPre(boolean precondition, String msg)
	{
		check(precondition, "invalid precondition " + msg);
	}

	//============================================================
	/** check a postcondition.
	* @param postcondition
	* @param msg
	*/
	//============================================================
	public void checkPost(boolean postcondition, String msg)
	{
		check(postcondition, "invalid postcondition " + msg);
	}

	//============================================================
	/** writes an stack trace to the debug output.
	* @param t
	*/
	//============================================================
	public void writeStackTrace(Throwable t)
	{
		final StringWriter stackTrace = new StringWriter();
		t.printStackTrace(new PrintWriter(stackTrace));
		writeln(stackTrace.toString());
	}

	//============================================================
	/** This method is called when an assertion has failed.
	*/
	//============================================================
	public void assertionFailed()
	{
		try
		{
			//=== sleep for a very long time
			Thread.sleep(1000000);
			//=== exit
			System.exit(1);
		}
		catch(InterruptedException e) {}
	}

	//============================================================
	/** This method is called when a check has failed.
	*/
	//============================================================
	public void checkFailed()
	{
	}



}

