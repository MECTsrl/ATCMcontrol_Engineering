//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOCmn                              |
//                                                                            |
//  Filename    : SOCmnData.h                                                 |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Basic data object class                                     |
//                - SOCmnData: root class for toolbox data objects            |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOCMNDATA_H_
#define _SOCMNDATA_H_

#pragma pack(push,4)

//-----------------------------------------------------------------------------
// SOCmnData                                                                  |
//-----------------------------------------------------------------------------

class SOCMN_EXPORT SOCmnData  
{
public:
	SOCmnData(void);
	virtual ~SOCmnData(void);
}; // SOCmnData

#pragma pack(pop)
#endif
