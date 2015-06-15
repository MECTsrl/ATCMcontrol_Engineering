import java.applet.*;
import java.io.*;  
import java.net.*;
import java.util.Vector;

public class CP_ASPStarter extends Applet
{
	private Vector m_params = new Vector();
	private Vector m_values = new Vector();
	private String m_aspOutput = null;

	public String getAppletInfo()
	{
		return "ATCM. All Rights Reserved\n";
	}

	public void startASP(String asp,
						 String param1, String value1
						 ) throws MalformedURLException, IOException
	{
		m_params.addElement(param1);
		m_values.addElement(value1);
		startASP(asp);
	}

	public void startASP(String asp,
						 String param1, String value1,
						 String param2, String value2
						 ) throws MalformedURLException, IOException
	{
		m_params.addElement(param2);
		m_values.addElement(value2);
		startASP(asp, param1, value1);
	}

	public void startASP(String asp,
						 String param1, String value1,
						 String param2, String value2,
						 String param3, String value3
						 ) throws MalformedURLException, IOException
	{
		m_params.addElement(param3);
		m_values.addElement(value3);
		startASP(asp, param1, value1, param2, value2);
	}

	public void startASP(String asp,
						 String param1, String value1,
						 String param2, String value2,
						 String param3, String value3,
						 String param4, String value4
						 ) throws MalformedURLException, IOException
	{
		m_params.addElement(param4);
		m_values.addElement(value4);
		startASP(asp, param1, value1, param2, value2, param3, value3);
	}

	public void startASP(String asp,
						 String param1, String value1,
						 String param2, String value2,
						 String param3, String value3,
						 String param4, String value4,
						 String param5, String value5
						 ) throws MalformedURLException, IOException
	{
		m_params.addElement(param5);
		m_values.addElement(value5);
		startASP(asp, param1, value1, param2, value2, param3, value3, param4, value4);
	}

	public void startASP(String asp) throws MalformedURLException, IOException
	{
		// Delete old output of ASP.
		m_aspOutput = null;

		// Write to ASP.
	    URL serverFile = new URL(asp);
		URLConnection serverConnection = serverFile.openConnection();
		serverConnection.setDoOutput(true);
		PrintWriter out = new PrintWriter(serverConnection.getOutputStream());

		int numberOfParams = m_params.size();
		int maxIndex = numberOfParams - 1;
		for (int i = 0; i <= maxIndex; i++)
		{
			String param = (String) m_params.elementAt(i);
			String value = (String) m_values.elementAt(i);

			// All param and value parameters of all startASP functions are checked here.
			if (param == null)
			{
				throw new IllegalArgumentException("parameter name == null");
			}
			if (value == null)
			{
				throw new IllegalArgumentException("parameter value == null");
			}
			if (param.length() == 0)
			{
				throw new IllegalArgumentException("length of parameter name == 0");
			}

			// Start ASP.
			out.print(URLEncoder.encode(param) + "=" + URLEncoder.encode(value) + "&");
		}

		out.close();

		// Delete all old params and values.
		m_params.removeAllElements();
		m_values.removeAllElements();



		// Read result from ASP.
		InputStream in = serverConnection.getInputStream();
		int inputBufferSize = 2048;
		byte inputBuffer[] = new byte[inputBufferSize];
		StringBuffer aggregationBuffer = new StringBuffer();
		int numberOfReadBytes;
		while ((numberOfReadBytes = in.read(inputBuffer)) != -1)
		{
			aggregationBuffer.append(new String (inputBuffer, 0, numberOfReadBytes));
		}

		in.close();

		// Store output of ASP.
		m_aspOutput = aggregationBuffer.toString();
	}

	public String getASPOutput()
	{
		return m_aspOutput;
	}

	// This method is called by java script to check whether called applet methods are executed.
	// This method does not belong to this class really.
	public String checkAppletExecution()
	{
		return "ok";
	}
}
