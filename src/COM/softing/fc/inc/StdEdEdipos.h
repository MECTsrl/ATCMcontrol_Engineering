/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/StdEdEdipos.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: StdEdEdipos.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/StdEdEdipos.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                CE
 *
 * =COMPONENT           INC
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *== Edipos enum (replaces former dbi.h)
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  05.09.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef STDEDEDIPOS_H_
#define STDEDEDIPOS_H_


/* 
 * Enum EDP_POS_QUALIFIER controls the interpretation of the x,y,cx and cy 
 * numbers of an standard editor position or BP-Info (part of DBI File)
 * x,y,cx,cy: default value is -1 if not set.
 */
typedef enum tagEDP_POS_QUALIFIER {        //    interpretation of coordinates
    EDP_STLINE,     //edp at ST line        :  x>=1: column  x<=0: no column info available
                    //                         y  line number always >=1 
    EDP_GRBOX,	     //edp at Box of 4GR     : (x;y)=Box origin, cx=cy=-1
    EDP_GRLDBOX,	  //edp at LD-Box of 4GR  : (x;y)=Box origin, cx=cy=-1
    EDP_GRSTBOX,    //edp at ST-Text Box    : (x;y)=Box origin, 
                    //                         cx>=1: column  cx<=0: no column info available
                    //                         cy   : line number always >=1
    EDP_GRREGION,   //edp is a 4GR region   : (x;y)=region origin (cx;cy)=region Size 
    EDP_GRBEGIN,    //edp at begin of 4GR   : x=y=cx=cy-1
    EDP_GREND,      //edp at end of 4GR     : x=y=cx=cy-1
    EDP_GRLINE,     //edipos is a line      : from (x;y) to (cx;cy) or vice versa
    EDP_GRSIGNAL    //edipos is a signal    : one of the signals line is ends (begins) at (x;y)
} EDP_POS_QUALIFIER;

#endif



/*
 *----------------------------------------------------------------------------*
 *  $History: StdEdEdipos.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 2  *****************
 * User: Rw           Date: 9.11.01    Time: 10:58
 * Updated in $/4Control/COM/softing/fc/inc
 * EDP_GRLDBOX for ladder animation inserted
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 22.10.01   Time: 11:32
 * Created in $/4Control/COM/softing/fc/inc
 * moved from CE\inc to global inc (is needed in DbgInfo.idl)
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 5.09.01    Time: 11:09
 * Created in $/4Control/COM/softing/fc/CE/inc
 *==
 *----------------------------------------------------------------------------*
*/
