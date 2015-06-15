//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : GENCACHE.H                                                  |   
//  Version     : 2.01.0.00.release                                           |
//  Date        : 7-May-1999                                                  |
//                                                                            |
//  Description : Cache classes                                               |
//                - GenericCache: cache object for item tag values            |
//                                                                            |
//  CHANGE_NOTES                                                              |
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//  10/29/98  RT        added updateCache method                              |
//  01/03/99  RT        changed relation between cache and tag from 1:1 to 1:n|
//  04/22/99  RT        added report driven protocol support                  |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _GENCACHE_H_
#define _GENCACHE_H_

#include <opcda.h>
#include "GenObj.h"
#include "GenNS.h"

//-----------------------------------------------------------------------------
// GenericCache                                                               |
//-----------------------------------------------------------------------------

class GenericTag;
class GenericRequest;

class OPCENGINE_EXPORT GenericCache : public GenericObject
{
friend GenericItemTag;
friend GenericTag;

public:
	GenericCache(void); 
	
	void init(void);

	// cached value
	virtual BOOL setValue(IN LPCVARIANT value);
	virtual BOOL updateValue(IN LPCVARIANT value);
	virtual BOOL getValue(OUT LPVARIANT value);
	virtual BOOL getValue(IN VARTYPE typeWanted, OUT LPVARIANT value);
	virtual BOOL getValue(IN GenericRequest *req, IN VARTYPE typeWanted, OUT LPVARIANT value);

	ULONG getValueSize(IN OPTIONAL LPVARIANT val = NULL);

	// quality of cached value
	virtual BOOL setQuality(IN WORD quality);
	WORD getQuality(void) const;
	virtual WORD getQuality(IN GenericRequest *req);

	// time of cached value
	virtual BOOL setTimeStamp(IN LPFILETIME timeStamp);
	BOOL getTimeStamp(OUT LPFILETIME timeStamp) const;
	virtual BOOL getTimeStamp(IN GenericRequest *req, OUT LPFILETIME timeStamp);

	// native datatype
	virtual BOOL setDatatype(IN VARTYPE datatype);
	VARTYPE getDatatype(void) const;

	// datatype conversion
	virtual BOOL supportDatatype(IN VARTYPE dataType);
	virtual BOOL changeDatatype(IN VARIANT *value, IN VARTYPE type, 
			OUT VARIANT *changedValue);

	// get all item tags which are connected to the cache
	virtual void getAllItemTags(OUT ItemTagList &itemTagList);

	// object trace id
	virtual LPCTSTR getObjTraceId(void);

protected:
	~GenericCache(); 

	// callbacks
	virtual void onSetValue(IN LPCVARIANT value);
	virtual void onSetQuality(IN WORD quality);
	virtual void onSetTimeStamp(IN LPFILETIME timeStamp);
	virtual void onSetDatatype(IN VARTYPE datatype);

	COleVariant m_value;	// cached value
	FILETIME m_timeStamp;	// time of cached value
	WORD m_quality;			// quality of cached value
	VARTYPE m_datatype;		// native datatype of cached value

	// helper for getAllItemTags
	void addItemTagsOfNode(IN GenericBranch *node, OUT ItemTagList &itemTagList);
}; // GenericCache


inline VARTYPE GenericCache::getDatatype(void) const
{ return m_datatype; }

inline WORD GenericCache::getQuality(void) const
{ return m_quality; }

inline BOOL GenericCache::getTimeStamp(OUT LPFILETIME timeStamp) const
{ if (!timeStamp) return FALSE;
  timeStamp->dwLowDateTime = m_timeStamp.dwLowDateTime; 
  timeStamp->dwHighDateTime = m_timeStamp.dwHighDateTime; return TRUE; }

#endif
