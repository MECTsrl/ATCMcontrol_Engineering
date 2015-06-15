/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrPBarGrafV.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrPBarGrafV.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrPBarGrafV.java $
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
 * $History: CP_GrPBarGrafV.java $
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
	Panel for viewing variable
*/

public class CP_GrPBarGrafV extends CP_GrPBarGraf
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
	CP_GrPBarGrafV( int p_numBarGrafs,double p_yMin, double p_yMax , double p_yOrigin,
					boolean p_barGrafSwitch, boolean p_setPointSwitch, boolean p_scaleSwitch,
					boolean p_limitSwitch)
	{
		super(p_numBarGrafs, p_yMin, p_yMax, p_yOrigin, p_barGrafSwitch, p_setPointSwitch,
			p_scaleSwitch, p_limitSwitch);

		//== bar graf
		if ( m_barGrafSwitch ) 
		{
			m_BarGrafGroup = new CP_GrBarGrafGroupV(p_numBarGrafs);
			m_BarGrafGroup.setRange(p_yMin,p_yMax,p_yOrigin);
			for ( int ii=1;ii<=p_numBarGrafs;ii++)
				m_BarGrafGroup.addBarGraf(ii);
			if ( p_setPointSwitch ) m_BarGrafGroup.addSetPoint();
		}
		
		//== scale
		if ( m_scaleSwitch )
//			m_Scale = new CP_GrScaleV( p_yMin, p_yMax );
			m_Scale = new CP_GrScaleV( p_yMin, p_yMax, 0, true, 0.1);

		//== limits
		if ( m_limitSwitch )
		{
			m_Limits = new CP_GrLimitsV(CP_GrLimitsV.LEFT);
			m_Limits.setRange(p_yMin, p_yMax );
		}
							
		pLayout();	
	}

	protected void pLayout()
	{
		GridBagLayout pLayout = new GridBagLayout();
		GridBagConstraints constr = new GridBagConstraints();
		
		//== get weights for different layouts
		double weightx[] = new double[3];
		int insetl[] = new int[3];
		int insetr[] = new int[3];

		getWeightX(weightx,insetl,insetr);

		//== layout
		setLayout(pLayout);
		constr.fill = GridBagConstraints.BOTH;
		constr.weighty = 1;
		
		//== limits panel
		if ( m_limitSwitch )
		{
			CP_GrFramedPanel pl = new CP_GrFramedPanel(CP_GrFramedPanel.SUNKEN);
			pl.setXBorderType(0);
			pl.setYBorderType(0.1);
			pl.setFrameWidth(2);
			pl.setLayout(new GridLayout(1,1,1,0));
			pl.add(m_Limits);
			//== layout limits
			constr.weightx = weightx[0];
			constr.insets = new Insets(0,insetl[0],0,insetr[0]);
			pLayout.setConstraints(pl,constr);
			add(pl);
		}

		//== bar graf panel
		if ( m_barGrafSwitch )
		{
			CP_GrFramedPanel pb = new CP_GrFramedPanel(CP_GrFramedPanel.SUNKEN);
			pb.setXBorderType(0);
			pb.setYBorderType(0.1);
			pb.setFrameWidth(2);
			pb.setLayout(new GridLayout(1,1));
			pb.add(m_BarGrafGroup);
			//== layout bargraf
			constr.weightx = weightx[1];
			constr.insets = new Insets(0,insetl[1],0,insetr[1]);
			pLayout.setConstraints(pb,constr);
			add(pb);
		}

		//== scale
		if ( m_scaleSwitch )
		{
			CP_GrFramedPanel ps = new CP_GrFramedPanel(CP_GrFramedPanel.NONE);
			ps.setXBorderType(0);
			ps.setYBorderType(0);
			ps.setFrameWidth(2);
			ps.setLayout(new GridLayout(1,1));
			ps.add(m_Scale);
			//== layout scale
			constr.weightx = weightx[2];
			constr.insets = new Insets(0,insetl[2],0,insetr[2]);
			constr.gridwidth = GridBagConstraints.REMAINDER;
			pLayout.setConstraints(ps,constr);
			add(ps);
		}
	}

	/**
		get horizontal weights, dependant on switches
	*/
	protected void getWeightX(double weight[],int insetl[], int insetr[])
	{
		int setPoint;
		final int padBorder=5;
		final int padInner=5;

		if (m_setPointSwitch)
			setPoint = 1;
		else
			setPoint = 0;

		if (m_limitSwitch && m_barGrafSwitch && m_scaleSwitch)
		{
			weight[0] = 0.15;
			weight[1] = 0.2*(m_numBarGrafs+setPoint);
			weight[2] = 1.0;
			insetl[0] = padBorder;
			insetr[0] = 0;
			insetl[1] = padInner;
			insetr[1] = 0;
			insetl[2] = padInner;
			insetr[2] = padBorder;
		}
		else if (!m_limitSwitch && m_barGrafSwitch && m_scaleSwitch)
		{
			weight[0] = 0.0;
			weight[1] = 0.15*(m_numBarGrafs+setPoint);
			weight[2] = 1.0;
			insetl[0] = 0;
			insetr[0] = 0;
			insetl[1] = padBorder;
			insetr[1] = 0;
			insetl[2] = padInner;
			insetr[2] = padBorder;
		}
		else if (m_limitSwitch && !m_barGrafSwitch && m_scaleSwitch)
		{
			weight[0] = 0.15;
			weight[1] = 0.0;
			weight[2] = 1.0;
			insetl[0] = padBorder;
			insetr[0] = 0;
			insetl[1] = 0;
			insetr[1] = 0;
			insetl[2] = padInner;
			insetr[2] = padBorder;
		}
		else if (!m_limitSwitch && !m_barGrafSwitch && m_scaleSwitch)
		{
			weight[0] = 0.0;
			weight[1] = 0.0;
			weight[2] = 1.0;
			insetl[0] = 0;
			insetr[0] = 0;
			insetl[1] = 0;
			insetr[1] = 0;
			insetl[2] = padBorder;
			insetr[2] = padBorder;

		}
		else if (m_limitSwitch && m_barGrafSwitch && !m_scaleSwitch)
		{
			weight[0] = 0.15;
			weight[1] = 0.15*(m_numBarGrafs+setPoint);;
			weight[2] = 0.0;
			insetl[0] = padBorder;
			insetr[0] = 0;
			insetl[1] = padInner;
			insetr[1] = padBorder;
			insetl[2] = 0;
			insetr[2] = 0;
		}
		else if (!m_limitSwitch && m_barGrafSwitch && !m_scaleSwitch)
		{
			weight[0] = 0.0;
			weight[1] = 1.0;
			weight[2] = 0.0;
			insetl[0] = 0;
			insetr[0] = 0;
			insetl[1] = padBorder;
			insetr[1] = padBorder;
			insetl[2] = 0;
			insetr[2] = 0;
		}
		else if (m_limitSwitch && !m_barGrafSwitch && !m_scaleSwitch)
		{
			weight[0] = 1.0;
			weight[1] = 0.0;
			weight[2] = 0.0;
			insetl[0] = padBorder;
			insetr[0] = padBorder;
			insetl[1] = 0;
			insetr[1] = 0;
			insetl[2] = 0;
			insetr[2] = 0;
		}
		else
		{
			weight[0] = 0.0;
			weight[1] = 0.0;
			weight[2] = 0.0;
			insetl[0] = 0;
			insetr[0] = 0;
			insetl[1] = 0;
			insetr[1] = 0;
			insetl[2] = 0;
			insetr[2] = 0;
		}
	}
}