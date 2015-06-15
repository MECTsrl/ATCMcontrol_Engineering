/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_ApBarGrafH.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_ApBarGrafH.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_ApBarGrafH.java $
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
 * $History: CP_ApBarGrafH.java $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 1  *****************
 * User: Scd          Date: 19.03.01   Time: 14:22
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:12
 * Created in $/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 4  *****************
 * User: So           Date: 27.05.98   Time: 11:03
 * Updated in $/fc/CP/lib/Java/Graphic/CPBarGraf
 * parameter background
 * 
 * *****************  Version 3  *****************
 * User: Ff           Date: 29.04.98   Time: 12:42
 * Updated in $/fc/CP/lib/Java/Graphic/CPBarGraf
 * 
 * *****************  Version 2  *****************
 * User: Ff           Date: 23.04.98   Time: 10:15
 * Updated in $/fc/CP/lib/Java/Graphic/CPBarGraf
 *==
 ******************************************************************************
H<<*/


//******************************************************************************
// CP_ApBarGrafH.java:	Applet
//
//******************************************************************************
import java.applet.*;
import java.awt.*;

//==============================================================================
// Main Class for applet CP_ApBarGrafH
//
//==============================================================================
public class CP_ApBarGrafH extends CP_ApBarGraf
{

	// CP_AnalogMonitor Class Constructor
	//--------------------------------------------------------------------------
	public CP_ApBarGrafH()
	{
		super();
	}

	// APPLET INFO SUPPORT:
	//		The getAppletInfo() method returns a string describing the applet's
	// author, copyright date, or miscellaneous information.
    //--------------------------------------------------------------------------
	public String getAppletInfo()
	{
		return "Name: CP_ApBarGrafH\r\n" +
		       "Author: ATCM\r\n" +
		       "";
	}


	// The init() method is called by the AWT when an applet is first loaded or
	// reloaded.  Override this method to perform whatever initialization your
	// applet needs, such as initializing data structures, loading images or
	// fonts, creating frame windows, setting the layout manager, or adding UI
	// components.
    //--------------------------------------------------------------------------
	public void init()
	{
		readParameters();
		
        //----------------------------------------------------------------------
		resize(400, 150);

		//== add analog monitor panel
		m_GrPBarGraf = new CP_GrPBarGrafH(m_numBarGrafs,
											m_YMin,
											m_YMax,
											m_YOrigin,
											m_barGraf,
											m_setPoint,
											m_scale,
											m_limits);
		
		if ( m_limits )
		{
			//== add limit marks to panel
			m_GrPBarGraf.getLimits().addLimit(1,CP_GrLimits.LOWER);
			m_GrPBarGraf.getLimits().addLimit(2,CP_GrLimits.LOWER);
			m_GrPBarGraf.getLimits().addLimit(3,CP_GrLimits.UPPER);
			m_GrPBarGraf.getLimits().addLimit(4,CP_GrLimits.UPPER);
		}

		if ( m_bgColor != null)
			m_GrPBarGraf.setBackground(m_bgColor);

		m_barGrafComp=m_GrPBarGraf.getBarGrafGroup();
		m_limitsComp=m_GrPBarGraf.getLimits();
		m_scaleComp=m_GrPBarGraf.getScale();
		setLayout(new GridLayout(1,1));
		add(m_GrPBarGraf);
	}
}