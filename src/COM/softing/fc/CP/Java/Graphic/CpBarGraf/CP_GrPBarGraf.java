/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrPBarGraf.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrPBarGraf.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrPBarGraf.java $
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
 * $History: CP_GrPBarGraf.java $
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
 * User: Scd          Date: 19.03.01   Time: 14:52
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:12
 * Created in $/fc/CP/Java/Graphic/CpBarGraf
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

import java.awt.*;
import java.util.*;

/**
	Panel for viewing variable
*/

public abstract class CP_GrPBarGraf extends Panel
{
	/**
		contructor
		@param p_numBarGrafs number of bar grafs in group
		@param p_yMin miminal value
		@param p_yMax maximal value
		@param p_yOrigin origin of bargraf
		@param p_barGraf switch to set bar graf
		@param p_setPoint switch to set set point
		@param p_scale switch to set scale
		@param p_limits switch to set limits
	*/
	CP_GrPBarGraf( int p_numBarGrafs,double p_yMin, double p_yMax , double p_yOrigin,
					boolean p_barGrafSwitch, boolean p_setPointSwitch, boolean p_scaleSwitch,
					boolean p_limitSwitch)
	{
		m_numBarGrafs = p_numBarGrafs;
		m_barGrafSwitch = p_barGrafSwitch;
		m_scaleSwitch = p_scaleSwitch;
		m_limitSwitch = p_limitSwitch;
		m_setPointSwitch = p_setPointSwitch;
	}

	public CP_GrBarGrafGroup getBarGrafGroup()
	{
		return m_BarGrafGroup;
	}

	public CP_GrLimits getLimits()
	{
		return m_Limits;
	}

	public CP_GrScale getScale()
	{
		return m_Scale;
	}

	// Update Handler
	//--------------------------------------------------------------------------
	public void update(Graphics g)
	{
		paint(g);
	}
	
	public void paint(Graphics g)
	{
		/*
		Dimension d = size();
		if (m_offScreen == null || m_offScreenDimension != d)
		{
			m_offScreen = createImage(d.width,d.height);
			m_offScreenDimension.width = d.width;
			m_offScreenDimension.height = d.height;
		}
		
		//double buffering does not work if component in framed panel
		Graphics goff = m_offScreen.getGraphics();
		m_BarGrafGroup.paint(goff);
		m_BgLimits.paint(goff);
		m_Scale.paint(goff);
		goff.drawImage(m_offScreen,0,0,this);
		*/
		super.paint(g);
	}
	

	public void repaint()
	{
	}

	protected abstract void pLayout();


	//== limits bargraf and scale
	protected CP_GrBarGrafGroup m_BarGrafGroup;
	protected CP_GrScale m_Scale;
	protected CP_GrLimits m_Limits;

	protected int m_numBarGrafs;
	protected boolean m_barGrafSwitch;
	protected boolean m_scaleSwitch;
	protected boolean m_limitSwitch;
	protected boolean m_setPointSwitch;


	//== image and image dimension for double buffering
	protected Image m_offScreen = null;
	protected Dimension m_offScreenDimension = new Dimension(0,0);
}