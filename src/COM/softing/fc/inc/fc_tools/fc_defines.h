/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_defines.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_defines.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_defines.h $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           fc_tools
 *
 * =CURRENT             $Date: 28.02.07 19:01 $
 *                      $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *== Basic defines to be used for fc_tools headers and sources
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  03.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/
#ifndef FC_DEFINES_H_
#define FC_DEFINES_H_

#define FC_TOAPI
#define FC_TODECL _stdcall

//Free BSTR and NULL the variable
//!!!DEPRECATED was miss spelled !!!! better use FC_FREE_BSTR
#define FC_FREESYSTRING(var) if((var)) { \
	::SysFreeString((var)); \
	(var) = NULL;}





//------------------------------------------------------------------*
/**
 * @def - Macro calculates the number of array elements. Can only be
 *        used for fixed size arrays.
*/
#define FC_ARRAY_LEN(var) (sizeof(var)/sizeof((var)[0]))


//------------------------------------------------------------------*
/**
 * @def - Macro frees BSTR and NULL the variable
*/
#define FC_FREE_BSTR(var) if((var)) { \
	::SysFreeString((var)); \
	(var) = NULL;}


//------------------------------------------------------------------*
/**
 * @def - Macro allocates a struct type with C++ new (should not be 
 *        a class!) and memset it to zero. The returned pointer has 
 *        already the struct_type type, don't need an extra type cast.
 * @see   FC_NEW_ZASTRUCT
*/
#define FC_NEW_ZSTRUCT(p, struct_type) ((p = new struct_type)!=NULL?\
    memset(p, 0, sizeof(struct_type)) : NULL)

//------------------------------------------------------------------*
/**
 * @def - Macro allocates a one dimensional array of struct with 
 *        C++ new (should not be a class!) and memset the  array 
 *        to zero. The returned pointer has already the struct_type* 
 *        type, don't need an extra type cast.
 * @see   FC_NEW_ZSTRUCT
*/
#define FC_NEW_ZASTRUCT(p, struct_type, count) ((p = new struct_type[count])!=NULL?\
    memset(p, 0, sizeof(struct_type)*(count)) : NULL)

//------------------------------------------------------------------*
/**
 * @def - Macro for Win32 path seperators
*/
#define FC_PATH_SEP _T("\\/")

//------------------------------------------------------------------*
/**
 * @def - Macro Checks a pointer for NULL and returns hr if the 
 * reference is NULL. Useful for parameter checks on entry points
 * of functions. Use it with an HRESULT value as second argument.
 *
 * @see FC_PCHECK_THROW            
*/
#define FC_PCHECK_HR(p,hr)  if((p) == NULL) { return (hr); } 

//------------------------------------------------------------------*
/**
 * @def - Macro Checks a pointer for NULL and throws exception if the 
 * reference is NULL. Useful for parameter checks on entry points
 * of functions. Use it for example with an HRESULT value as second
 * argument.
 *
 * @see FC_PCHECK_HR            
*/
#define FC_PCHECK_THROW(p,except) if((p) == NULL) { throw (except); } 

//------------------------------------------------------------------*
/**
 * @def - Macro Checks the pointer for NULL and calls Release() 
 * for the pointer if the reference is not NULL. Useful for Release
 * of COM pointers.
 *
*/
#define FC_RELEASE_PTR(p) if((p)) {\
    (p)->Release(); \
    (p) = NULL;}

#endif //FC_DEFINES_H_

/*
 *----------------------------------------------------------------------------*
 *  $History: fc_defines.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 11  *****************
 * User: Ef           Date: 11.03.02   Time: 15:09
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * removed dead code
 * 
 * *****************  Version 10  *****************
 * User: Ef           Date: 26.03.01   Time: 11:54
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * added comments and renamed FC_FREESYSTRING to FC_FREE_BSTR
 * 
 * *****************  Version 9  *****************
 * User: Ef           Date: 7.03.01    Time: 18:23
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * macro arg needs ()
 * 
 * *****************  Version 8  *****************
 * User: Ef           Date: 26.02.01   Time: 10:48
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * corrected SWKE name
 * 
 * *****************  Version 7  *****************
 * User: Su           Date: 12.02.01   Time: 18:43
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 6  *****************
 * User: Su           Date: 8.02.01    Time: 18:59
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 6.02.01    Time: 16:23
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 31.01.01   Time: 16:31
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new function fc_cgarray::findLong
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 23.01.01   Time: 17:08
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 5.01.01    Time: 17:08
 * Created in $/4Control/COM/softing/fc/inc/fc_tools
 * from V1.3 tools.dll, and some new features
 *==
 *----------------------------------------------------------------------------*
*/
