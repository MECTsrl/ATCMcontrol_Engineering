/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_ApBarGraf.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_ApBarGraf.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_ApBarGraf.java $
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
 * $History: CP_ApBarGraf.java $
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
 * User: Scd          Date: 19.03.01   Time: 14:21
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 3  *****************
 * User: Jd           Date: 10.07.00   Time: 11:06
 * Updated in $/4Control/COM/softing/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:12
 * Created in $/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 13  *****************
 * User: So           Date: 17.07.98   Time: 11:48
 * Updated in $/fc/CP/lib/Java/Graphic/CPBarGraf
 * 
 * *****************  Version 12  *****************
 * User: So           Date: 16.07.98   Time: 14:19
 * Updated in $/fc/CP/lib/Java/Graphic/CPBarGraf
 * andere Darstellung bzgl quality
 * 
 * *****************  Version 11  *****************
 * User: Ct           Date: 5/29/98    Time: 4:30p
 * Updated in $/fc/CP/lib/Java/Graphic/CPBarGraf
 * 
 * *****************  Version 10  *****************
 * User: Ct           Date: 5/29/98    Time: 4:17p
 * Updated in $/fc/CP/lib/Java/Graphic/CPBarGraf
 * 
 * *****************  Version 9  *****************
 * User: Ct           Date: 5/29/98    Time: 3:42p
 * Updated in $/fc/CP/lib/Java/Graphic/CPBarGraf
 * 
 * *****************  Version 8  *****************
 * User: Ff           Date: 29.05.98   Time: 11:11
 * Updated in $/fc/CP/lib/Java/Graphic/CPBarGraf
 * 
 * *****************  Version 7  *****************
 * User: So           Date: 27.05.98   Time: 11:03
 * Updated in $/fc/CP/lib/Java/Graphic/CPBarGraf
 * parameter background
 * 
 * *****************  Version 6  *****************
 * User: So           Date: 27.05.98   Time: 10:42
 * Updated in $/fc/CP/lib/Java/Graphic/CPBarGraf
 * parameter 'Background' eingefuegt
 * 
 * *****************  Version 5  *****************
 * User: Ff           Date: 26.05.98   Time: 11:10
 * Updated in $/fc/CP/lib/Java/Graphic/CPBarGraf
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

//******************************************************************************
// CP_ApBarGraf.java:	Applet
//
//******************************************************************************
import java.applet.*;
import java.awt.*;

//==============================================================================
// Main Class for applet CP_ApBarGraf
//
//==============================================================================
public abstract class CP_ApBarGraf extends Applet implements Runnable
{
	// THREAD SUPPORT:
	//		m_CP_GrBarGraf	is the Thread object for the applet
	//--------------------------------------------------------------------------
	Thread	 m_CP_GrBarGraf = null;

    // Members for applet parameters
    // <type>       <MemberVar>    = <Default Value>
    //--------------------------------------------------------------------------
	protected double m_YMin = 0.0;
	protected double m_YMax = 1.0;
	protected double m_YOrigin = 0.0;
	protected int m_numBarGrafs = 1;
	protected boolean m_barGraf = true;
	protected boolean m_scale = true;
	protected boolean m_limits = false;
	protected boolean m_setPoint = false;
	protected Color m_bgColor=null;
	protected CP_GrBarGrafGroup m_barGrafComp=null;
	protected CP_GrLimits m_limitsComp=null;
	protected CP_GrScale m_scaleComp=null;
	


    // Parameter names.  To change a name of a parameter, you need only make
	// a single change.  Simply modify the value of the parameter string below.
    //--------------------------------------------------------------------------
	private final String PARAM_YMin = "YMin";
	private final String PARAM_YMax = "YMax";
	private final String PARAM_YOrigin = "YOrigin";
	private final String PARAM_NumBarGrafs = "NumBarGrafs";
	private final String PARAM_BarGraf = "BarGraf";
	private final String PARAM_Scale = "Scale";
	private final String PARAM_Limits = "Limits";
	private final String PARAM_SetPoint = "SetPoint";
	private final String PARAM_Background = "Background";
	
	//== the analog monitor panel
	CP_GrPBarGraf m_GrPBarGraf;


	// CP_AnalogMonitor Class Constructor
	//--------------------------------------------------------------------------
	public CP_ApBarGraf()
	{
	}

	// PARAMETER SUPPORT
	//		The getParameterInfo() method returns an array of strings describing
	// the parameters understood by this applet.
	//
    // CP_AnalogMonitor Parameter Information:
    //  { "Name", "Type", "Description" },
    //--------------------------------------------------------------------------
	public String[][] getParameterInfo()
	{
		String[][] info =
		{
			{ PARAM_YMin, "double", "minimal value of scale/bargraf" },
			{ PARAM_YMax, "double", "maximal value of display" },
			{ PARAM_YOrigin, "double", "origin of bargraf" },
			{ PARAM_NumBarGrafs, "int", "number of bargrafs" },
			{ PARAM_BarGraf, "boolean", "show bar graf" },
			{ PARAM_Scale, "boolean", "show scale" },
			{ PARAM_Limits, "boolean", "show limits" },
			{ PARAM_SetPoint, "boolean", "show setpoint in bar graf" },
			{ PARAM_Background, "string", "background color" }
		};
		return info;		
	}

	// The init() method is called by the AWT when an applet is first loaded or
	// reloaded.  Override this method to perform whatever initialization your
	// applet needs, such as initializing data structures, loading images or
	// fonts, creating frame windows, setting the layout manager, or adding UI
	// components.
    //--------------------------------------------------------------------------
	public abstract void init();


	// Place additional applet clean up code here.  destroy() is called when
	// when you applet is terminating and being unloaded.
	//-------------------------------------------------------------------------
	public void destroy()
	{
		// TODO: Place applet cleanup code here
	}

	// CP_AnalogMonitor Paint Handler
	//--------------------------------------------------------------------------
	public void paint(Graphics g)
	{
		Dimension d = getSize();
		if (m_offScreen == null || m_offScreenDimension != d)
		{
			m_offScreen = createImage(d.width,d.height);
			m_offScreenDimension.width = d.width;
			m_offScreenDimension.height = d.height;
		}
		
		//double buffering does not work if component in framed panel
		Graphics goff = m_offScreen.getGraphics();

//		Color oldColor = goff.getColor();
//		goff.setColor(getBackground());
//		goff.setColor(Color.blue);
//		goff.fillRect(0, 0, d.width, d.height);
//		goff.setColor(oldColor);

		m_GrPBarGraf.paint(goff);
		goff.drawImage(m_offScreen,0,0,this);
	}

	// CP_AnalogMonitor Update Handler
	//--------------------------------------------------------------------------
	public void update(Graphics g)
	{
		paint(g);
	}

	//		The start() method is called when the page containing the applet
	// first appears on the screen. The AppletWizard's initial implementation
	// of this method starts execution of the applet's thread.
	//--------------------------------------------------------------------------
	public void start()
	{
		if (m_CP_GrBarGraf == null)
		{
			m_CP_GrBarGraf = new Thread(this);
			m_CP_GrBarGraf.start();
		}
		// TODO: Place additional applet start code here
	}
	
	//		The stop() method is called when the page containing the applet is
	// no longer on the screen. The AppletWizard's initial implementation of
	// this method stops execution of the applet's thread.
	//--------------------------------------------------------------------------
	public void stop()
	{
		if (m_CP_GrBarGraf != null)
		{
			m_CP_GrBarGraf.stop();
			m_CP_GrBarGraf = null;
		}

		// TODO: Place additional applet stop code here
	}

	// THREAD SUPPORT
	//		The run() method is called when the applet's thread is started. If
	// your applet performs any ongoing activities without waiting for user
	// input, the code for implementing that behavior typically goes here. For
	// example, for an applet that performs animation, the run() method controls
	// the display of images.
	//--------------------------------------------------------------------------
	public void run()
	{
		while (true)
		{
			try
			{
				//repaint();
				// TODO:  Add additional thread-specific code here
				Thread.sleep(1000000);
			}
			catch (InterruptedException e)
			{
				// TODO: Place exception-handling code here in case an
				//       InterruptedException is thrown by Thread.sleep(),
				//		 meaning that another thread has interrupted this one
				stop();
			}
		}
	}

	protected void readParameters()
	{
		// PARAMETER SUPPORT
		//		The following code retrieves the value of each parameter
		// specified with the <PARAM> tag and stores it in a member
		// variable.
		//----------------------------------------------------------------------
		String param;

		// YMin: minimal value of scale/bargraf
		//----------------------------------------------------------------------
		param = getParameter(PARAM_YMin);
		if (param != null) {
			m_YMin = Double.valueOf(param).doubleValue();
			m_YOrigin = m_YMin;
		}

		// YMAX: maximal value of display
		//----------------------------------------------------------------------
		param = getParameter(PARAM_YMax);
		if (param != null)
			m_YMax = Double.valueOf(param).doubleValue();

		// YOrigin
		//------------------------------------------------------------------------
		param = getParameter(PARAM_YOrigin);
		if (param != null) 
			m_YOrigin = Double.valueOf(param).doubleValue();

		// NumBarGraf
		//------------------------------------------------------------------------
		param = getParameter(PARAM_NumBarGrafs);
		if (param != null)
			m_numBarGrafs = Integer.valueOf(param).intValue();

		// barGraf
		//------------------------------------------------------------------------
		param = getParameter(PARAM_BarGraf);
		if (param != null)
			m_barGraf = Boolean.valueOf(param).booleanValue();

		// scale
		//------------------------------------------------------------------------
		param = getParameter(PARAM_Scale);
		if (param != null)
			m_scale = Boolean.valueOf(param).booleanValue();

		// limits
		//------------------------------------------------------------------------
		param = getParameter(PARAM_Limits);
		if (param != null)
			m_limits = Boolean.valueOf(param).booleanValue();

		// set points
		//------------------------------------------------------------------------
		param = getParameter(PARAM_SetPoint);
		if (param != null)
			m_setPoint = Boolean.valueOf(param).booleanValue();
	
		// Background
		//------------------------------------------------------------------------
		param = getParameter(PARAM_Background);
		if (param != null)
			m_bgColor = CP_GrColor.getColor(param);
	}

	/**
		set value of bargraf at postion
		@p_position position of bar graf in group, 1 is leftmost bar graf
		@p_value value to display
	*/
	public void setValue(int p_position, double p_value)
	{
		if (m_barGrafComp != null)
			m_barGrafComp.setValue(p_position, p_value);
	}

	/**
		set quality of bar graf (min, max, origin)
		@p_quality quality of bargraf
	*/
	public void setQuality(int p_quality)
	{
		if (m_barGrafComp != null)
			m_barGrafComp.setQuality(p_quality);
		if (m_limitsComp != null)
			m_limitsComp.setQuality(p_quality);
	}	

	/**
		set quality of bar graf at position
		@p_position position of bar graf in group, 1 is leftmost bar graf
		@p_quality quality of bargraf
	*/
	public void setQuality(int p_position, int p_quality)
	{
		if (m_barGrafComp != null)
			m_barGrafComp.setQuality(p_position, p_quality);
	}

	/**
		set value of set point in bar graf
		@p_value value to display
	*/
	public void setValueSetPoint(double p_value)
	{
		if (m_barGrafComp != null)
			m_barGrafComp.setSetPointValue(p_value);
	}

	/**
		set quality of set point in bar graf at postion
		@p_position position of bar graf in group, 1 is leftmost bar graf
		@p_quality quality of bargraf
	*/
	public void setQualitySetPoint(int p_quality)
	{
		if (m_barGrafComp != null)
			m_barGrafComp.setSetPointQuality(p_quality);
	}

	/**
		set minimal value of bargraf
		@p_value minimal value in bar graf
	*/
	public void setMin(double p_value)
	{
		if (m_barGrafComp != null)
			m_barGrafComp.setMin(p_value);
		if (m_limitsComp != null)
			m_limitsComp.setMin(p_value);
		if (m_scaleComp != null)
			m_scaleComp.setDomainMin(p_value);
	}

	/**
		set maximal value of bargraf
		@p_value maximal value in bar graf
	*/
	public void setMax(double p_value)
	{
		if (m_barGrafComp != null)
			m_barGrafComp.setMax(p_value);
		if (m_limitsComp != null)
			m_limitsComp.setMax(p_value);
		if (m_scaleComp != null)
			m_scaleComp.setDomainMax(p_value);
	}

	/**
		set origin value of bargraf
		@p_value origin value in bar graf
	*/
	public void setOrigin(double p_value)
	{
		if (m_barGrafComp != null)
			m_barGrafComp.setOrigin(p_value);
	}

	/**
		set background color
		@param p_Color name of color (see JavaScript alinkColor) or
				RGB value "#<hexvalue>" 
				examle: "red", "#ff0000", ...
	*/
	public void setBgColor(String p_Color)
	{
		Color color = CP_GrColor.getColor(p_Color);
		m_GrPBarGraf.setBackground(color);
		super.repaint();
	}

	/**
		set background color of bar graf
		@param p_Color name of color (see JavaScript alinkColor) or
				RGB value "#<hexvalue>" 
				examle: "red", "#ff0000", ...
	*/
	public void setBgColorBarGraf(String p_Color)
	{
		if (m_barGrafComp != null)
		{
			Color color = CP_GrColor.getColor(p_Color);
			m_barGrafComp.setBgColor(color);
		}
	}

	/**
		set background color of bar graf
		@param p_position position of bargraf in group, 1 is leftmost/upmost bargraf
		@param p_Color name of color (see JavaScript alinkColor) or
				RGB value "#<hexvalue>" 
				examle: "red", "#ff0000", ...
	*/
	public void setBgColorBarGraf(int p_position, String p_Color)
	{
		if (m_barGrafComp != null)
		{
			Color color = CP_GrColor.getColor(p_Color);
			m_barGrafComp.setBgColor(p_position, color);
		}
	}

	/**
		set forground color of bar graf
		@param p_position position of bargraf in group, 1 is leftmost/upmost bargraf
		@param p_Color name of color (see JavaScript alinkColor) or
				RGB value "#<hexvalue>" 
				examle: "red", "#ff0000", ...
	*/
	public void setFgColorBarGraf(int p_position, String p_Color)
	{
		if (m_barGrafComp != null)
		{
			Color color = CP_GrColor.getColor(p_Color);
			m_barGrafComp.setFgColorPos(p_position,color);
			m_barGrafComp.setFgColorNeg(p_position,color);
		}
	}

	/**
		set background color of set point bar graf
		@param p_Color name of color (see JavaScript alinkColor) or
				RGB value "#<hexvalue>" 
				examle: "red", "#ff0000", ...
	*/
	public void setBgColorSetPoint(String p_Color)
	{
		if (m_barGrafComp != null)
		{
			Color color = CP_GrColor.getColor(p_Color);
			m_barGrafComp.setBgColorSetPoint(color);
		}
	}

	/**
		set forground color of set point bar graf
		@param p_Color name of color (see JavaScript alinkColor) or
				RGB value "#<hexvalue>" 
				examle: "red", "#ff0000", ...
	*/
	public void setFgColorSetPoint(String p_Color)
	{
		if (m_barGrafComp != null)
		{
			Color color = CP_GrColor.getColor(p_Color);
			m_barGrafComp.setFgColorSetPoint(color);
		}
	}

	/**
		set value of limit marker at spec. position 
		@param p_position position of limit: 1,2 lower limits 3,4 upper limits
		@param p_value value of limit
	*/
	public void setValueLimit(int p_position, double p_value)
	{
		if (m_limitsComp != null)
			m_limitsComp.setValue(p_position,p_value);
	}
	/**
		set quality for limit mark at spec. position
		@param p_position position of limit mark
		@param p_quality 0 good, 1 uncertain, 2 bad
	*/
	public void setQualityLimit(int p_position, int p_quality) 
	{
		if (m_limitsComp != null)
			m_limitsComp.setQuality(p_position,p_quality);
	}

		/**
		set quality for limits as whole id scale min and max
		if quality is set to not good the quality of all limit marks
		changes to not good. if quality changes to good, the state is not 
		inherited to limit marks
		@param p_quality 0 good, 1 uncertain, 2 bad
	*/
	public void setQualityLimit(int p_quality) 
	{
		if (m_limitsComp != null)
			m_limitsComp.setQuality(p_quality);
	}

	/**
		set property BgColor of limits
		@param p_Color background color of limits
	*/
	public void setBgColorLimit(String p_Color) 
	{
		if (m_limitsComp != null)
		{
			Color color = CP_GrColor.getColor(p_Color);
			m_limitsComp.setBgColor(color);
		}
	}

	/**
		set property FgColor for limit mark at spec. position
		@param p_position position of limit mark
		@param p_Color forground color of limit mark
	*/
	public void setFgColorLimit(int p_position, String p_Color) 
	{
		if (m_limitsComp != null)
		{
			Color color = CP_GrColor.getColor(p_Color);
			m_limitsComp.setFgColor(p_position,color);
		}
	}


	//== image and image dimension for double buffering
	Image m_offScreen = null;
	Dimension m_offScreenDimension = new Dimension(0,0);
}