/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrPStdBarGraf.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrPStdBarGraf.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrPStdBarGraf.java $
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
 * $History: CP_GrPStdBarGraf.java $
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
 * User: Scd          Date: 19.03.01   Time: 14:54
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:12
 * Created in $/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 4  *****************
 * User: Ff           Date: 29.04.98   Time: 12:42
 * Updated in $/fc/CP/lib/Java/Graphic/CPBarGraf
 * 
 * *****************  Version 3  *****************
 * User: Ff           Date: 23.04.98   Time: 10:15
 * Updated in $/fc/CP/lib/Java/Graphic/CPBarGraf
 *==
 ******************************************************************************
H<<*/

import java.awt.*;
import java.util.*;

/**
	Panel for analog monitoring
*/

public class CP_GrPStdBarGraf extends Panel
{

	/**
		contructor
		@param p_numBarGrafs number of bar grafs
		@param p_yMin miminal value
		@param p_yMax maximal value
		@param p_yOrigin origin of bargraf
	*/
	CP_GrPStdBarGraf( int p_numBarGrafs, double p_yMin, double p_yMax , double p_yOrigin)
	{
		//== limits
		m_BgLimits = new CP_GrLimitsV(CP_GrLimitsV.LEFT);
		m_BgLimits.setRange(p_yMin,p_yMax);

		//== bar graf
		m_BarGrafGroup = new CP_GrBarGrafGroupV(p_numBarGrafs);
		m_BarGrafGroup.setRange(p_yMin,p_yMax,p_yOrigin);
		m_BarGrafGroup.addBarGraf(1);

		//== scale
		// Font font = new Font ("Dialog",Font.PLAIN,12);
		m_Scale = new CP_GrScaleV( p_yMin, p_yMax, 0, true, 0.1 );
							
		pLayout();
	}

	/**
		set value of bargraf at specified position
	*/
	public void setValue(int p_position, double p_value)
	{
		//== update bargraf
		m_BarGrafGroup.setValue(p_position, p_value);

		//== update limits
		m_BgLimits.setControlledValue(p_value);
	}

	/**
		set value of limit mark at specified position
	*/
	public void setLimitValue(int p_position, double p_value)
	{
		m_BgLimits.setValue(p_position, p_value);
	}

	
	public CP_GrLimits getLimits()
	{
		return m_BgLimits;
	}

	public CP_GrBarGrafGroup getBarGrafGroup()
	{
		return m_BarGrafGroup;
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

	protected void pLayout()
	{
		GridBagLayout pLayout = new GridBagLayout();
		GridBagConstraints constr = new GridBagConstraints();

		//== limits panel
		CP_GrFramedPanel pl = new CP_GrFramedPanel(CP_GrFramedPanel.SUNKEN);
		pl.setXBorderType(0);
		pl.setYBorderType(0.1);
		pl.setFrameWidth(2);
		pl.setLayout(new GridLayout(1,1,1,0));
		pl.add(m_BgLimits);

		//== bar graf panel
		CP_GrFramedPanel pb = new CP_GrFramedPanel(CP_GrFramedPanel.SUNKEN);
		pb.setXBorderType(0);
		pb.setYBorderType(0.1);
		pb.setFrameWidth(2);
		pb.setLayout(new GridLayout(1,1));
		pb.add(m_BarGrafGroup);

		//== scale
		CP_GrFramedPanel ps = new CP_GrFramedPanel(CP_GrFramedPanel.NONE);
		ps.setXBorderType(0);
		ps.setYBorderType(0);
		ps.setFrameWidth(2);
		ps.setLayout(new GridLayout(1,1));
		ps.add(m_Scale);

		//== layout
		setLayout(pLayout);
		constr.fill = GridBagConstraints.BOTH;
		constr.weighty = 1;

		//== layout limits
		constr.weightx = 0.15;
		constr.insets = new Insets(0,5,0,0);
		pLayout.setConstraints(pl,constr);
		add(pl);

		//== layout bargraf
		constr.weightx = 0.2;
		pLayout.setConstraints(pb,constr);
		add(pb);

		//== layout scale
		constr.weightx = 1.0;
		constr.gridwidth = GridBagConstraints.REMAINDER;
		pLayout.setConstraints(ps,constr);
		add(ps);
	}


	//== limits bargraf and scale
	private CP_GrLimits m_BgLimits;
	private CP_GrBarGrafGroup m_BarGrafGroup;
	private CP_GrScale m_Scale;

	//== image and image dimension for double buffering
	Image m_offScreen = null;
	Dimension m_offScreenDimension = new Dimension(0,0);
}