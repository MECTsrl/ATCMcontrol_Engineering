//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOAeS                              |
//                                                                            |
//  Filename    : SOAeSWatch.h                                                |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : OPC Server monitoring classes                               |
//                - SOAeSWatch: watch object                                  |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOAESWATCH_H_
#define _SOAESWATCH_H_

#include "SOSrvWatch.h"

#pragma pack(push,4)

//-----------------------------------------------------------------------------
// SOAeSWatchHandler                                                          |
//-----------------------------------------------------------------------------

class SOAES_EXPORT SOAeSWatchHandler : public SOSrvWatchHandler
{
public:
	SOAeSWatchHandler(void);  

	static void doEventStatistics(IN SOAeSEvent *event);

protected:
	struct SOAeSWatchHandlerStatisticsData
	{
		// event notifications
		ULONG simpleEvent;
		ULONG trackingEvent;
		ULONG condEvent;
	} m_statistics; 

	virtual SOCmnObject *getObject(IN LPCTSTR objSpace, IN LPCTSTR objType);
	virtual void getRootObjects(OUT SOCmnList<SOCmnObject> objList);

	virtual HRESULT getStatistics(IN MSXML::IXMLDOMDocumentPtr pDoc, IN MSXML::IXMLDOMNodePtr pRootNode);
};

#pragma pack(pop)
#endif
