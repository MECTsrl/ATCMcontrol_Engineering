/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdPinList.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: FbdPinList.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdPinList.java $
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
 * $History: FbdPinList.java $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 1  *****************
 * User: Scd          Date: 19.03.01   Time: 15:38
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 2  *****************
 * User: Mkr          Date: 14.07.99   Time: 18:22
 * Updated in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 23.06.99   Time: 17:49
 * Created in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 * Initial version
 *==
 ******************************************************************************
H<<*/

import java.util.Vector;
  
public class FbdPinList
{
	private Vector m_pinVector = new Vector();

	public FbdPinList()
	{
		m_pinVector = new Vector();			
	}

	public void addPin(FbdPin pin)
	{
		m_pinVector.addElement(pin);
	}

	public void addPin(int x, int y, int iType, String strName)
	{
		m_pinVector.addElement(new FbdPin(x, y, iType, strName));
	}

	public int getLength()	      { return m_pinVector.size();	}
	public FbdPin getPin(int pos) { return (FbdPin)(m_pinVector.elementAt(pos)); }
	public boolean isEmpty()      { return m_pinVector.isEmpty(); }
}
