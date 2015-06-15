/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpScale/CP_GrScalePainter.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrScalePainter.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpScale/CP_GrScalePainter.java $
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
 * $History: CP_GrScalePainter.java $
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
 * User: Scd          Date: 19.03.01   Time: 15:57
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpScale
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:13
 * Created in $/fc/CP/Java/Graphic/CpScale
 * 
 * *****************  Version 7  *****************
 * User: Ff           Date: 24.06.98   Time: 9:24
 * Updated in $/fc/CP/lib/Java/Graphic/CPScale
 * 
 * *****************  Version 6  *****************
 * User: Ff           Date: 17.06.98   Time: 16:57
 * Updated in $/fc/CP/lib/Java/Graphic/CPScale
 * 
 * *****************  Version 5  *****************
 * User: Ff           Date: 9.06.98    Time: 16:58
 * Updated in $/fc/CP/lib/Java/Graphic/CPScale
 * 
 * *****************  Version 4  *****************
 * User: Ff           Date: 29.04.98   Time: 12:42
 * Updated in $/fc/CP/lib/Java/Graphic/CPScale
 * 
 * *****************  Version 3  *****************
 * User: Ff           Date: 23.04.98   Time: 10:16
 * Updated in $/fc/CP/lib/Java/Graphic/CPScale
 *==
 ******************************************************************************
H<<*/

import java.awt.*;
import java.text.*;
import java.util.*;

//================================================================================
/** Paints a scale in a graphics context.*/
//================================================================================
abstract class CP_GrScalePainter 
{
	//============================================================
	/** The debug interface.*/
	//============================================================
	/*_CPDEBUG_BEGIN
	static public CP_Debug m_debug;
_CPDEBUG_END*/

	//============================================================
	/** The font that shall used for the lettering.*/
	//============================================================
	protected FontMetrics m_fontMetrics;

	//============================================================
	/** The positions of the big dashes.*/
	//============================================================
	protected int[] m_posBigDashes;

	//============================================================
	/** The position of the axis.*/
	//============================================================
	protected int m_posAxis;

	//============================================================
	/** The first point of the axis.*/
	//============================================================
	protected int m_posAxisFrom;

	//============================================================
	/** The last point of the axis.*/
	//============================================================
	protected int m_posAxisTo;

	//============================================================
	/** The letterings.*/
	//============================================================
	protected String[] m_letterings;

	//============================================================
	/** The step between two small dashes measured in pixels.*/
	//============================================================
	protected double m_smallStepInPixels;

	//============================================================
	/** The number of small dashes between two big dashes.*/
	//============================================================
	protected int m_numberOfSmallDashes;

	//============================================================
	/** The length of a big dash.*/
	//============================================================
	protected int m_lenBigDash;

	//============================================================
	/** The length of a small dash.*/
	//============================================================
	protected int m_lenSmallDash;

	//============================================================
	/** The width of a big dash.*/
	//============================================================
	protected int m_widthBigDash;

	//============================================================
	/** The width of a small dash.*/
	//============================================================
	protected int m_widthSmallDash;

	//============================================================
	/** The width the axis.*/
	//============================================================
	protected int m_widthAxis;

	//============================================================
	/** The padding between the big dashes and the lettering.*/
	//============================================================
	protected int m_padLettering;

	//============================================================
	/** The scale model.*/
	//============================================================
	protected CP_GrScaleModel m_scaleModel;

	//============================================================
	/** The alignment of the scale.
	* True means positive, that is to the right for vertical scales
	* and down for horizontal scales.
	* False means negative, that to left for vertical scales
	* and up for horizontal scales.
	*/
	//============================================================
	protected boolean m_alignmentPos;

	//============================================================
	/** Color for axis.*/
	//============================================================
	protected Color m_colorAxis;

	//============================================================
	/** Color for big dashes.*/
	//============================================================
	protected Color m_colorBigDash;

	//============================================================
	/** Color for small dashes.*/
	//============================================================
	protected Color m_colorSmallDash;

	//============================================================
	/** Color for lettering.*/
	//============================================================
	protected Color m_colorLetter;

	//============================================================
	/** Constructs a new scale painter.*/
	//============================================================
	public CP_GrScalePainter(boolean alignmentPos, FontMetrics fm)
	{
		m_alignmentPos = alignmentPos;
		m_fontMetrics = fm;
		setColors(Color.black, Color.black, Color.black, Color.black);

		m_scaleModel = new CP_GrScaleModel();
 
		setDomain(0.0, 100.0);

		m_lenBigDash = 10;
		m_lenSmallDash = 5;
		m_widthBigDash = 2;
		m_widthSmallDash = 1;
		m_numberOfSmallDashes = 3;
		m_widthAxis= 1;
		m_padLettering = 4;

	}

	//============================================================
	/** Sets scale domain.*/
	//============================================================
	public void setDomain(double min, double max) 
	{
		m_scaleModel.setDomain(min, max);
	}

	//============================================================
	/** Translate axis. */
	//============================================================
	public void translateAxis(int t)
	{
		m_posAxis += t;
	}
	
	//============================================================
	/** Sets color.*/
	//============================================================
	public void setColors(Color colAxis, Color colSmallDash, Color colBigDash, Color colLetter) 
	{
		m_colorAxis = colAxis;
		m_colorSmallDash = colSmallDash;
		m_colorBigDash = colBigDash;
		m_colorLetter = colLetter;
	}

	//============================================================
	/** Get length of scale.*/
	//============================================================
	abstract public int getAxisLength();

	//============================================================
	/** Get the space the lettering exceeds the axis.*/
	//============================================================
	abstract public int getAxisExceed();

	//============================================================
	/** Get breadth of scale.*/
	//============================================================
	abstract public int getBreadth();

	//============================================================
	/** Get font metrics. */
	//============================================================
	public FontMetrics getFontMetrics()
	{
		return m_fontMetrics;
	}
	
	//============================================================
	/** Get positions of big dashes relative to m_posAxisFrom. */
	//============================================================
	public int[] getPosBigDashes()
	{
		return m_posBigDashes;
	}

	//============================================================
	/** Get screen position of scale axis end. */
	//============================================================
	public int getPosAxisTo()
	{
		return m_posAxisTo;
	}

	//============================================================
	/** Paints a scale into a rectangle area of a graphics context.*/
	//============================================================
	public void paint(Graphics g)
	{
		//=== paint scale axis
		paintAxis(
			g, m_colorAxis,
			m_alignmentPos,
			m_posAxis, m_posAxisFrom, m_posAxisTo, m_widthAxis
			);

		//=== paint big dashes
		paintDashes(
			g, m_colorBigDash, 
			m_alignmentPos,
			m_posAxis, m_posAxisFrom, m_posAxisTo, m_widthAxis,
			m_posBigDashes, 
			m_lenBigDash, m_widthBigDash
			);

		int firstDash = m_posBigDashes[0];
		int lastDash = m_posBigDashes[m_posBigDashes.length - 1];

		//=== paint minimum and maximum dashes
		paintMinMaxDashes(
			g, m_colorBigDash, 
			m_alignmentPos,
			m_posAxis, m_posAxisFrom, m_posAxisTo, m_widthAxis,
			m_lenBigDash, m_widthBigDash,
			firstDash, lastDash
			);

		//=== paint small dashes
		paintSmallDashes(
			g, m_colorSmallDash,
			m_alignmentPos,
			m_posAxis, m_posAxisFrom, m_posAxisTo, m_widthAxis,
			m_posBigDashes, 
			m_lenSmallDash, m_widthSmallDash,
			m_numberOfSmallDashes,
			m_widthBigDash
			);

		//=== paint lettering
		paintLettering(
			g, m_colorLetter, m_fontMetrics, 
			m_alignmentPos,
			m_posAxis, m_posAxisFrom, m_posAxisTo, m_widthAxis,
			m_lenBigDash,
			m_padLettering,
			m_posBigDashes, 
			m_letterings
			);

  }

	//============================================================
	/** Paints the scale axis.*/
	//============================================================
	abstract protected void paintAxis(
		Graphics g,
		Color color,
		boolean alignPos,
		int posAxis,
		int posAxisFrom,
		int posAxisTo,
		int widthAxis
		);

	//============================================================
	/** Paints dashes.*/
	//============================================================
	protected void paintDashes(
		Graphics g,
		Color color,
		boolean alignPos,
		int posAxis,
		int posAxisFrom,
		int posAxisTo,
		int axisWidth,
		int[] pos,
		int length,
		int width
		)
	{
		if ( pos == null || pos.length < 1 )
		{
			return;
		}

		//=== remember old color
		Color oldColor = g.getColor();

		//=== set new color
		g.setColor(color);

		//=== draw dashes
		for(int i = 0; i < pos.length; i++)
		{		
			paintDash(
				g, alignPos, 
				posAxis, posAxisFrom, posAxisTo, axisWidth,
				pos[i], length, width
				);
		}

		//=== reset old color
		g.setColor(oldColor);
	}

	//============================================================
	/** Paints small dashes.*/
	//============================================================
	protected void paintSmallDashes(
		Graphics g,	Color color,
		boolean alignPos,
		int posAxis, int posAxisFrom, int posAxisTo, int axisWidth,
		int[] pos, int length, int width,
		int n,
		int wBD
		)
	{
		//=== if no small dashes shall be painted return immediately
		if ( n <= 0 || pos == null )
		{
			return;
		}

		//=== set color and remember old one
		Color oldColor = g.getColor();
		g.setColor(color);

		double smallStep = m_smallStepInPixels;

		//=== draw small dashes between big dashes
		for(int i = 0; i < pos.length - 1; i++)
		{
			for( 
				double posSD = pos[i] + wBD + smallStep, j = 0; 
				j < n && posSD < pos[i+1]; 
				posSD += width + smallStep, j++
					)
			{
				paintDash(
					g, alignPos,
					posAxis, posAxisFrom, posAxisTo, axisWidth,
					(int)Math.round(posSD), length, width
					);
			}
		}

		//=== if no big dashes were painted ...
		if ( pos.length == 0 )
		{
			//=== draw small dashes between minimum and maximum
			for(
				double posSD = posAxisFrom + wBD + smallStep, j = 0;
				j < n && posSD < posAxisTo;
				posSD += width + smallStep, j++
					)
				{
					paintDash(
						g, alignPos,
						posAxis, posAxisFrom, posAxisTo, axisWidth,
						(int)Math.round(posSD), length, width
						);
				}
		}
		//=== otherwise ...
		else 
		{
			//=== ... draw small dashes between minimum and first dash ...
			for(
				double posSD = pos[0] - smallStep - width, j = 0;
				j < n && posSD > (int)Math.round(wBD + smallStep);
				posSD -= width + smallStep, j++
					)
				{
					paintDash(
						g, alignPos,
						posAxis, posAxisFrom, posAxisTo, axisWidth,
						(int)Math.round(posSD), length, width
						);
				}

			//=== ... draw small dashes between maximum and last dash
			for(
				double posSD = pos[pos.length - 1] + wBD + smallStep, j = 0;
				j < n && posSD <= posAxisTo - posAxisFrom - wBD - width;
				posSD += width + smallStep, j++
					)
				{
					paintDash(
						g, alignPos,
						posAxis, posAxisFrom, posAxisTo, axisWidth,
						(int)Math.round(posSD), length, width
						);
				}

		}
		//=== reset previous color
		g.setColor(oldColor);
	}

	//============================================================
	/** Draws a minimum dash.*/
	//============================================================
	protected void paintMinMaxDashes(
		Graphics g,
		Color color,
		boolean alignPos, 
		int posAxis, int posAxisFrom, int posAxisTo, int axisWidth, 
		int length, int width,
		int firstDash, int lastDash
		)
	{
	
		Color oldColor = g.getColor();
		g.setColor(color);

		if ( firstDash - width >= width )
		{
			paintDash(
				g, alignPos,
				posAxis, posAxisFrom, posAxisTo, axisWidth,
				0, length, width
				);
		}

		if ( lastDash + 2*width <= posAxisTo - ( width + 1 ) )
		{
			paintDash(
				g, alignPos,
				posAxis, posAxisFrom, posAxisTo, axisWidth,
				posAxisTo - posAxisFrom - (width - 1), length, width
				);
		}

		g.setColor(oldColor);
	}

	//============================================================
	/** Draws a dash.*/
	//============================================================
	abstract protected void paintDash(
		Graphics g,
		boolean alignPos, 
		int posAxis, int posAxisFrom, int posAxisTo, int axisWidth, 
		int pos, int length, int width
		);

	//============================================================
	/** Paints lettering.*/
	//============================================================
	abstract protected void paintLettering(
		Graphics g,
		Color color,
		FontMetrics fm,
		boolean alignPos,
		int posAxis,
		int posAxisFrom,
		int posAxisTo,
		int widthAxis,
		int lenBigDashes,
		int padLettering,
		int[] pos,
		String[] letterings
		);

	//============================================================
	/** Computes the positions for dashes and letterings.*/
	//============================================================
	public void compute(Rectangle r)
	{
		//=== compute axis
		computeAxis(r, m_alignmentPos);

		//=== compute big dashes and lettering
		computeBigDashesAndLettering(
			m_scaleModel,
			m_widthBigDash, m_widthSmallDash,
			m_numberOfSmallDashes,
			(m_posAxisTo - m_posAxisFrom + 1) - m_widthBigDash,
			m_fontMetrics
			);

	}

	//============================================================
	/** Computes the positions for the axis.*/
	//============================================================
	abstract protected void computeAxis(Rectangle r, boolean alignPos);

	//============================================================
	/** Computes the positions for big dashes and texts for lettering.*/
	//
	//============================================================
	protected void computeBigDashesAndLettering(
		CP_GrScaleModel scaleModel,
		int widthBD,
		int widthSD,
		int noSD,
		int lenAxis,
		FontMetrics fm
	)
	{
		//=== compute the available space in pixels for the domain
		//=== since each big dash has itself a width and the last
		//=== big dash may reach over the end of the axis
		//=== the available space is:
		//=== length of axis - ( width of a big dash - 1)
		int availSpace = lenAxis - (widthBD - 1);

		if ( availSpace < 1 )
		{
			m_posBigDashes = null;
			m_letterings = null;
			return;
		}
		//=== compute a first guess for the minimum padding between big dashes
		int padMin = computePadMin(scaleModel, widthBD, widthSD, noSD, fm);
		
		//=== compute the scaling factor to convert screen distances 
		//=== to domain distances:
		double screenToDomainFactor = 
			(scaleModel.getMax() - scaleModel.getMin()) / availSpace;

		//=== array holding the positions for big dashes
		int[] posBigDashes = null;;

		//=== array holding the letterings
		String[] letterings = null;

		//=== map the minimum pixel space to minimum stepwidth in scale domain
		double stepWidthMin = padMin * screenToDomainFactor;
		
		//== set stepwidth constraints
		scaleModel.setStepWidthConstraints(stepWidthMin);
		
		//=== optimize scale model
		scaleModel.optimize();
		
		//=== compute big dash positions
		posBigDashes = computePosBigDashes(
			scaleModel, lenAxis, widthBD);
		
		//=== compute letterings
		letterings = computeLetterings(scaleModel);

		//=== compute maximal overlapping of texts
		int maxOverlap = computeMaxOverlap(
			letterings, posBigDashes, m_padLettering, m_fontMetrics);
		/*_CPDEBUG_BEGIN
		m_debug.writeln("MAXOVERLAP: " + String.valueOf(maxOverlap));
_CPDEBUG_END*/

		if ( maxOverlap > 0 )
		{
			padMin += maxOverlap;
			stepWidthMin = padMin * screenToDomainFactor;
			scaleModel.setStepWidthConstraints(stepWidthMin);
			scaleModel.optimize();
			posBigDashes = computePosBigDashes(scaleModel, lenAxis, widthBD);
			letterings = computeLetterings(scaleModel);
			maxOverlap = computeMaxOverlap(
				letterings, posBigDashes, m_padLettering, m_fontMetrics);
			/*_CPDEBUG_BEGIN
			m_debug.writeln("MAXOVERLAP: " + String.valueOf(maxOverlap));	
_CPDEBUG_END*/
		}


		//=== double compute length of scale steps measured in pixels
		double stepInPixels = 0;
		//=== if there are any big dashes
		if ( posBigDashes.length > 0 )
		{
			//=== compute domain to screen factor
			double domainToScreenFactor = availSpace / (scaleModel.getMax() - scaleModel.getMin());
		
			//=== take scale step as base
			stepInPixels = scaleModel.getStepWidth() * domainToScreenFactor;
		}
		//=== otherwise...
		else
		{
			//===  take the whole available space as base
			stepInPixels = availSpace;
		}

		//=== compute step width for small dashes measured in pixels
		double smallStep = (stepInPixels - widthBD - noSD * widthSD)/(noSD+1);

		//=== set members
		m_posBigDashes = posBigDashes;
		m_letterings = letterings;
		m_smallStepInPixels = smallStep;

}

	//============================================================
	/** Computes a first guess for the minimum padding between big dashes.*/
	//============================================================
	protected int computePadMin(
		CP_GrScaleModel scaleModel,
		int widthBD,
		int widthSD,
		int noSD,
		FontMetrics fm
		)
	{
		//=== compute the stepwidth constraint:
		//=== consider two neighboured steps s1 <s2
		//=== every dash with width w is painted from the 
		//=== pixel covering s1 to the point laying w-1 pixels
		//=== away in maximum direction.
		//=== the variables called SpaceBD below measure the space
		//=== between s1 and s2 in pixels. When you assume, that s1 and
		//=== s2 both half a pixel, all these values are integers.

		//=== under s1 will a big dash be painted. 
		//=== * since s1 halfs the first pixel of this big dash,
		//===   there remains a space of widthBD - 1/2 for it, laying
		//===   between s1 and s2. 
		//=== * after a big dash must be some free place, that has to be at least
		//===   as long as the big dash itself.
		//=== * at the end there comes a half pixel laying before s2.
		//=== so all together this implies that between two big dashes there
		//=== has to be a minimum amount of:
		//=== widthBD - 1/2 + widthBD + 1/2 = 2 * widthBD
		//=== This value shall not be smaller than 2!
		int minSpaceBD_1 = Math.min(2*widthBD, 2);

		//=== between s1 and s2 must fit all small dashes.
		//=== between two small dashes must fit at least a gap of width widthSD
		//=== but at least of width 2
		int minGapSD = Math.min(widthSD, 2);
		
		//=== the small dashes need a minimum amount of space
		//=== between two big dashes must fit noSD small dashes
		//=== and between two small dashes must fit a gap of length minGapSD
		//=== but only if any small dashes shall be painted
		int minSpaceSD = (
			noSD > 0 ? 
			noSD * (widthSD + minGapSD) + minGapSD : 0
			);
			
		//=== this all results in a minimum space required between s1 and s2 of
		//=== widthBD - 1/2 + minSpaceSD + 1/2 = widthBD + minSpaceSD
		int minSpaceBD_2 = widthBD + minSpaceSD;

		//=== format letterings for scale minimum and scale maximum
		String minLettering = format(scaleModel.getMin());
		String maxLettering = format(scaleModel.getMax());
		
		//=== compute a first guess for lettering size
		int letteringSize = Math.max(getStringSize(fm, minLettering), getStringSize(fm, maxLettering));

		//=== between two letterings must fit some empty space
		int padLettering = getPadLettering(fm);

		//=== the lettering needs also a minimum place
		//=== if you assume that every lettering hash the same length
		//=== then there is the following space required:
		//=== letteringSize/2 + padLettering + letteringSize/2 = letteringSize + padLettering
		int minSpaceBD_3 = letteringSize + padLettering; 
		
		//=== compute the maximum of all minimum space requirements
		int minSpaceBD = Math.max(minSpaceBD_1, minSpaceBD_2);
		minSpaceBD = Math.max(minSpaceBD_2, minSpaceBD_3);

		return minSpaceBD;
	}

	//============================================================
	/** Computes the positions for big dashes.*/
	//============================================================
	protected int[] computePosBigDashes(
		CP_GrScaleModel sm, int lenAxis, int widthBigDash)
	{
		//=== get number of scale steps
		long scaleSteps = sm.getNumberOfSteps();

		int[] posBigDashes = null;
		double min = sm.getMin();

		if ( scaleSteps > 0 )
		{
			posBigDashes = new int[(int)scaleSteps];

			//=== compute scaling factor from domain to screen
			double domainToScreenFactor = lenAxis / ( sm.getMax() - min );

			for(long i = 0; i < scaleSteps; i++ )
			{
				//=== get the i-th scalestep
				double step = sm.getStep(i);

				//=== compute the pixel on the axis, that covers this step
				int pos = (int)Math.round((step-min)*domainToScreenFactor);

				//=== but no dash may be closer to last axis pixel
				//== than width of big dash - 1
				//=== NOTE: should be covered by right value of availSpace
				//=== in computeBigDashesAndLettering
//				if( pos + widthBigDash > lenAxis )
//				{
//					pos = lenAxis - 1 - ( widthBigDash - 1);
//				}

				//=== add that pixel position to the array
				posBigDashes[(int)i] = pos;
			}
		}
		//=== if there are no scale steps
		//=== set as default one big dash for maximum and one for minimum
		//=== NOTE: should be covered by strategy for minimum and maximum dashes
//		else
//		{
//			posBigDashes = new int[2] { 0, lenAxis - 1 };
//		}

		return posBigDashes;
	}

	//============================================================
	/** Computes letterings.*/
	//============================================================
	protected String[] computeLetterings(CP_GrScaleModel sm)
	{
		//=== get number of scalesteps
		long scaleSteps = sm.getNumberOfSteps();

		String[] letterings = null;

		if ( scaleSteps > 0 )
		{
			letterings = new String[(int)scaleSteps];

			for(long i = 0; i < scaleSteps; i++)
			{
				letterings[(int)i] = format(sm.getStep(i));
			}
		}
		//=== if there are no scale steps 
		//=== set as default one lettering for minimum and one lettering for maximum
		else
		{
			letterings = new String[2];
			letterings[0] = format(sm.getMin());
			letterings[1] = format(sm.getMax());
		}

		return letterings;
	}

	//============================================================
	/** Computes maximum overlapping of lettering texts.*/
	//============================================================
	abstract protected int computeMaxOverlap(
		String[] letterings, int[] pos, int pad, FontMetrics fm);

	//============================================================
	/** Formats a number to a scale lettering.*/
	//============================================================
	protected String format(double x)
	{
		NumberFormat form = NumberFormat.getNumberInstance(Locale.US);
		String formatted = form.format(x);
		return formatted;
//		return String.valueOf(x);		
	}

	//============================================================
	/** Returns padding between two neighboured lettering.*/
	//============================================================
	abstract protected int getPadLettering(FontMetrics fm);

	//============================================================
	/** Returns place for string in a given font in pixels.*/
	//============================================================
	abstract protected int getStringSize(FontMetrics fm, String s);

}

