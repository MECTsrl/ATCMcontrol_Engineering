/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrLimitMark.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrLimitMark.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrLimitMark.java $
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
 * $History: CP_GrLimitMark.java $
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
 * User: Scd          Date: 19.03.01   Time: 14:30
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

/**
	class for drawing limit markers in face plates
*/
class CP_GrLimitMark extends CP_GrPolygon
{
	/**
		constructor
		CP_GrLimitMark supports following styles

	|\
	|  \				CP_GrLimitMark.UPPERLEFT
	|    \
	|______\ (0,0) origin of local coordinate system 
	
	 _______
	|      / (0,0)
    |    /
	|  /				CP_GrLimitMark.LOWERLEFT
	|/

		  / |
	    /   |			CP_GrLimitMark.UPPERRIGHT
	  /	    |
	/_______|
	(0,0)
	
	(0,0)
	________
	\       |
	  \     |			CP_GrLimitMark.LOWERRIGHT
	    \   |
		  \ |

	| width |

			
		@param p_width width of mark , height is width/2
		@p_style style of mark (see above), default is lower right 
  
	*/

	//== styles
	public final static int UPPERLEFT  = 1;
	public final static int LOWERLEFT  = 2;
	public final static int UPPERRIGHT = 3;
	public final static int LOWERRIGHT = 4;
	public final static int UPPERTOP   = 5;
	public final static int LOWERTOP   = 6;
	public final static int UPPERBOTTOM= 7;
	public final static int LOWERBOTTOM= 8;


	CP_GrLimitMark(int p_wh, int p_style)
	{
		super();
		int w1,h1,w2,h2;

		if ( p_style <= LOWERRIGHT )
		{
			w1=p_wh;
			h1=w1/2;
		}
		else
		{
			h1 = p_wh;
			w1 = h1/2;
		}
		w2 = w1;
		h2 = h1;
	
		switch (p_style)
		{
			case 1: w1 *= -1; h1 *= -1; w2 *= -1; h2 = 0;
					break;
			case 2: w1 *= -1; h1 = 0; w2 *=-1; 
					break;
			case 3: h1 = 0; h2 *= -1; 
					break;
			case 4: h2 = 0;
					break;
			case 5: h1 *= -1; w2 = 0; h2 *= -1;
					break;
			case 6: h1 *= -1; w1 = 0; w2 *= -1; h2 *= -1;
					break;
			case 7: w1 = 0;
					break;
			case 8: w1 *= -1; w2 = 0;
					break;
			default://nothing to do
		}

		// init polygon
		int n=4;
		int x[] = {0, w1, w2, 0};
		int y[] = {0, h1, h2, 0};
		
		init(x,y,n,x[0],y[0]);
	}

}