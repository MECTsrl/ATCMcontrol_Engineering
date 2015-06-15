//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SODaS                              |
//                                                                            |
//  Filename    : SODaSCache.h                                                |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Cache classes                                               |
//                - SODaSCache: base class for cache objects                  |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SODASCACHE_H_
#define _SODASCACHE_H_

#include <opcda.h>
#include "SOCmnObject.h"
#include "SODaSItemTag.h"

#pragma pack(push,4)


//-----------------------------------------------------------------------------
// SODaSCache                                                                 |
//-----------------------------------------------------------------------------

#define SODASCACHE_NOTIFYWATCH_VALUE      0x01
#define SODASCACHE_NOTIFYWATCH_QUALITY    0x02
#define SODASCACHE_NOTIFYWATCH_TIMESTAMP  0x04

class SODaSTag;
class SODaSRequest;

class SODAS_EXPORT SODaSCache : public SOCmnObject
{
friend SODaSItemTag;
friend SODaSTag;

public:
	SODaSCache(void); 
	
	void init(void);	// initialize

	// combined setting of the cache values
	virtual BOOL get(OUT LPVARIANT value, OUT WORD *quality, OUT LPFILETIME timeStamp, 
			OPTIONAL IN SODaSRequest *req = NULL, OPTIONAL IN VARTYPE typeWanted = VT_EMPTY, OPTIONAL IN LCID localeID = LOCALE_SYSTEM_DEFAULT);
	virtual BOOL set(IN LPCVARIANT value, IN WORD quality, OPTIONAL IN LPFILETIME timeStamp = NULL, OPTIONAL IN LCID localeID = LOCALE_SYSTEM_DEFAULT);
	virtual BOOL update(IN LPCVARIANT value, IN WORD quality, OPTIONAL IN LPFILETIME timeStamp = NULL, OPTIONAL IN LCID localeID = LOCALE_SYSTEM_DEFAULT);

	// cached value
	virtual BOOL setValue(IN LPCVARIANT value, OPTIONAL IN LCID localeID = LOCALE_SYSTEM_DEFAULT);
	virtual BOOL updateValue(IN LPCVARIANT value, OPTIONAL IN LCID localeID = LOCALE_SYSTEM_DEFAULT);
	virtual BOOL getValue(OUT LPVARIANT value);

	// size of cache value
	ULONG getValueSize(void);
	static ULONG getValueSize(IN LPVARIANT val);

	// quality of cached value
	virtual BOOL setQuality(IN WORD quality);
	WORD getQuality(void) const;
	virtual WORD getQuality(IN SODaSRequest *req);

	// time of cached value
	virtual BOOL setTimeStamp(IN LPFILETIME timeStamp);
	BOOL getTimeStamp(OUT LPFILETIME timeStamp) const;
	virtual BOOL getTimeStamp(IN SODaSRequest *req, OUT LPFILETIME timeStamp);

	// native datatype
	virtual BOOL setDatatype(IN VARTYPE datatype);
	VARTYPE getDatatype(void) const;

	// datatype conversion
	static BOOL defaultSupportDatatype(IN VARTYPE dataType);
	virtual BOOL supportDatatype(IN VARTYPE dataType);
	virtual BOOL changeDatatype(IN VARIANT *value, IN VARTYPE type, 
			OUT VARIANT *changedValue, OPTIONAL IN LCID localeID = LOCALE_SYSTEM_DEFAULT,
			OPTIONAL IN SODaSItemTag *itemTag = NULL);

	// get all item tags which are connected to the cache
	virtual void getAllItemTags(OUT SOCmnList<SODaSItemTag> &itemTagList);

	// object trace id
	virtual LPCTSTR getObjTraceId(void);

	// check if cache set at least once
	BOOL isSet(void) { return ((m_timeStamp.dwLowDateTime != 0) && (m_timeStamp.dwHighDateTime != 0)); }

	// value sting
	LPCTSTR getValueString(IN SOCmnString &buffer, OPTIONAL IN LCID localeID = SOSRVCONVERTER_LCID_DEFAULT);

protected:
	virtual ~SODaSCache(); 

	// callbacks
	virtual void onSetValue(IN LPCVARIANT value);
	virtual void onSetQuality(IN WORD quality);
	virtual void onSetTimeStamp(IN LPFILETIME timeStamp);
	virtual void onSetDatatype(IN VARTYPE datatype);

	SOCmnVariant m_value;	// cached value
	FILETIME m_timeStamp;	// time of cached value
	WORD m_quality;			// quality of cached value
	VARTYPE m_datatype;		// native datatype of cached value

	virtual BOOL getValue(IN SODaSRequest *req, IN VARTYPE typeWanted, IN LCID localeID, OUT LPVARIANT value);
	virtual BOOL getValue(IN VARTYPE typeWanted, OUT LPVARIANT value, OPTIONAL IN LCID localeID = LOCALE_SYSTEM_DEFAULT);

	// set helpers
	BOOL doSetValue(IN LPCVARIANT value, IN LCID localeID);
	BOOL doUpdateValue(IN LPCVARIANT value, IN LCID localeID);

	// helper for getAllItemTags
	void addItemTagsOfNode(IN SOCmnElement *node, OUT SOCmnList<SODaSItemTag> &itemTagList);

	// watch helpers
	void notifyWatch(IN BYTE what);
}; // SODaSCache

inline VARTYPE SODaSCache::getDatatype(void) const
{ return m_datatype; }

inline WORD SODaSCache::getQuality(void) const
{ return m_quality; }

inline BOOL SODaSCache::getTimeStamp(OUT LPFILETIME timeStamp) const
{ if (!timeStamp) return FALSE;
  timeStamp->dwLowDateTime = m_timeStamp.dwLowDateTime; 
  timeStamp->dwHighDateTime = m_timeStamp.dwHighDateTime; return TRUE; }

#define GenericCache SODaSCache

#pragma pack(pop)
#endif
