/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpUtil/CP_GrRectAngle.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrRectAngle.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpUtil/CP_GrRectAngle.java $
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
 * $History: CP_GrRectAngle.java $
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
 * User: Scd          Date: 20.03.01   Time: 9:30
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

public class CP_GrRectAngle {
	public CP_GrRectAngle()
	{
		x=0;
		y=0;
		w=0;
		h=0;
	}

	public CP_GrRectAngle(CP_GrRectAngle p_Rect)
	{
		x=p_Rect.x;
		y=p_Rect.y;
		w=p_Rect.w;
		h=p_Rect.h;
	}

	public int x,y,w,h;
}