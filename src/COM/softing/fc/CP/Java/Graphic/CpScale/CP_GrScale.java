/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpScale/CP_GrScale.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrScale.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpScale/CP_GrScale.java $
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
 * $History: CP_GrScale.java $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpScale
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpScale
 * 
 * *****************  Version 1  *****************
 * User: Scd          Date: 19.03.01   Time: 15:56
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpScale
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:13
 * Created in $/fc/CP/Java/Graphic/CpScale
 * 
 * *****************  Version 9  *****************
 * User: Ff           Date: 24.06.98   Time: 9:24
 * Updated in $/fc/CP/lib/Java/Graphic/CPScale
 * 
 * *****************  Version 8  *****************
 * User: Ff           Date: 17.06.98   Time: 16:57
 * Updated in $/fc/CP/lib/Java/Graphic/CPScale
 * 
 * *****************  Version 7  *****************
 * User: Ff           Date: 9.06.98    Time: 16:58
 * Updated in $/fc/CP/lib/Java/Graphic/CPScale
 * 
 * *****************  Version 6  *****************
 * User: Ff           Date: 29.04.98   Time: 12:42
 * Updated in $/fc/CP/lib/Java/Graphic/CPScale
 * 
 * *****************  Version 5  *****************
 * User: Ff           Date: 23.04.98   Time: 10:16
 * Updated in $/fc/CP/lib/Java/Graphic/CPScale
 *==
 ******************************************************************************
H<<*/

import java.awt.*;

//================================================================================
/** A graphical component for scale. */
//================================================================================
public abstract class CP_GrScale extends Panel
{
	//============================================================
	/** The debug interface.*/
	//============================================================
	/*_CPDEBUG_BEGIN
	static public CP_Debug m_debug;
	_CPDEBUG_END*/

	//============================================================
	/** The minimum of the scale domain.*/
	//============================================================
	double m_min;

	//============================================================
	/** The maximum of the scale domain.*/
	//============================================================
	double m_max;

	//============================================================
	/** The quality of the scale component.*/
	//============================================================
	CP_GrQuality m_quality;

	//============================================================
	/** The relative inset of the scale at the ends of its axis.*/
	//============================================================
	double m_relPad;

	//============================================================
	/** The scale painter.*/
	//============================================================
	CP_GrScalePainter m_painter;

	//============================================================
	/** The default font.*/
	//============================================================
	static final protected Font m_defaultFont = new Font(
												"Sans", 
												Font.PLAIN, 
												10
												);

	//============================================================
	/** Set members.*/
	//============================================================
	protected void setMembers(
		double min, double max, int q, 
		double relPad, CP_GrScalePainter p
		)
	{
		m_painter = p;
		setDomain(min, max);
		m_quality = new CP_GrQuality(q);
		m_relPad  = relPad;
	}

	//============================================================
	/** Set domain.*/
	//============================================================
	public void setDomain(double min, double max)
	{
		if ( m_min != min || m_max != max )
		{
			m_min = min;
			m_max = max;

			m_painter.setDomain(min, max);
			repaint();
		}
		else 
		{
			m_min = min;
			m_max = max;
		}
	}

	//============================================================
	/** Set domain minimum.*/
	//============================================================
	public void setDomainMin(double min)
	{
		setDomain(min, m_max);
	}

	//============================================================
	/** Set domain maximum.*/
	//============================================================
	public void setDomainMax(double max)
	{
		setDomain(m_min, max);
	}

	//============================================================
	/** Set quality.*/
	//============================================================
	public void setQuality(int q)
	{
		if ( m_quality.get() != q )
		{
			m_quality.set(q);
			repaint();
		}
	}

	//============================================================
	/** Set padding factor.*/
	//============================================================
	public void setPadding(double p)
	{
		m_relPad = p;
		repaint();
	}

	//============================================================
	/** Paints this component.*/
	//============================================================
	abstract public void paint(Graphics g);	
}

