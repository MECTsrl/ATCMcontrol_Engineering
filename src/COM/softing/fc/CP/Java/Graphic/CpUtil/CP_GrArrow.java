/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpUtil/CP_GrArrow.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrArrow.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpUtil/CP_GrArrow.java $
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
 * $History: CP_GrArrow.java $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 1  *****************
 * User: Scd          Date: 20.03.01   Time: 9:28
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:13
 * Created in $/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 3  *****************
 * User: Ff           Date: 29.04.98   Time: 12:42
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 * 
 * *****************  Version 2  *****************
 * User: Ff           Date: 23.04.98   Time: 10:16
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 *==
 ******************************************************************************
H<<*/

import java.awt.*;

/**
	class for drawing arrows
*/
public class CP_GrArrow extends CP_GrPolygon
{

	/**
		constructor

			   /\        -   -
			  /  \
			 /    \	     ha 	
			/_    _\     -
			  |  |
			  |  |           hb
			  |  |           _
			   --
			|wa|
			 |wb|
					  
		
		@param p_wb half width of body
		@param p_hb total length
		@param p_wa half width arrow head
		@param p_ha height arrow head
	*/
	public CP_GrArrow(int p_wb, int p_hb, int p_wa, int p_ha)
	{
		super();
		int n=8;
		int x[] = {0, -p_wa, -p_wb, -p_wb, p_wb, p_wb, p_wa, 0};
		int y[] = {0,  p_ha,  p_ha,  p_hb, p_hb, p_ha, p_ha, 0};
		init(x,y,n,x[0],y[0]);
	}

}