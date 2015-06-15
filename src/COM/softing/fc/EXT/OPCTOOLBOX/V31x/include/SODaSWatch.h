//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SODaS                              |
//                                                                            |
//  Filename    : SODaSWatch.h                                                |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : OPC Server monitoring classes                               |
//                - SODaSWatch: watch object                                  |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SODASWATCH_H_
#define _SODASWATCH_H_

#include "SOSrvWatch.h"

#pragma pack(push,4)

//-----------------------------------------------------------------------------
// SODaSWatchHandler                                                          |
//-----------------------------------------------------------------------------

class SODAS_EXPORT SODaSWatchHandler : public SOSrvWatchHandler
{
public:
	SODaSWatchHandler(void);  

	static void doRequestStatistics(IN SODaSRequest *req, IN DWORD completionSpan);

protected:
	struct SODaSWatchHandlerStatisticsData
	{
		// successful I/O requests
		ULONG asyncCacheRead;
		ULONG asyncDeviceRead;
		ULONG asyncWrite;
		ULONG syncCacheRead;
		ULONG syncDeviceRead;
		ULONG syncWrite;
		ULONG cyclicRead;
		ULONG reportRead;

		// failed I/O requests
		ULONG failedReadOrBadQuality;
		ULONG failedWrite;
		
		// times in milliseconds:
		// complete time in device operation 
		unsigned __int64 timeRead;
		unsigned __int64 timeWrite;
		// average time in device operation
		ULONG averageTimeRead;
		ULONG averageTimeWrite;
		// maximum time in device operation
		ULONG maxTimeRead;
		ULONG maxTimeWrite;
	} m_statistics; 


	virtual SOCmnObject *getObject(IN LPCTSTR objSpace, IN LPCTSTR objType);
	virtual void getRootObjects(OUT SOCmnList<SOCmnObject> objList);

	virtual BOOL hasCacheObject(IN SOCmnObject *tag, IN SOCmnObject *cache, OUT BOOL &uniqueCache);
	virtual BOOL setCollectCacheChanges(IN SOCmnObject *tag, IN BOOL doCollect);

	virtual SOCmnObject *getNameSpaceObjectByItemID(IN LPCTSTR itemID);

	virtual HRESULT getStatistics(IN MSXML::IXMLDOMDocumentPtr pDoc, IN MSXML::IXMLDOMNodePtr pRootNode);
};

#pragma pack(pop)
#endif
