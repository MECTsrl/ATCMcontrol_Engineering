//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOCmn                              |
//                                                                            |
//  Filename    : SOCmnTrace.h                                                |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Tracing classes                                             |
//                - SOCmnTrace: basic trancing class                          |
//                - SOCmnTraceHandler: external trace handler                 |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOCMNTRACE_H_
#define _SOCMNTRACE_H_

#include "SOCmnObject.h"
#include <mmsystem.h>
#ifdef _DEBUG
#if !defined(SO_WINCE) && !defined(__BORLANDC__)
#include <crtdbg.h>
#endif
#endif

#ifdef __BORLANDC__
#include <assert.h>
#endif __BORLANDC__

#pragma pack(push,4)

//-----------------------------------------------------------------------------
// Trace Groups                                                               |
//-----------------------------------------------------------------------------

#define SOCMNTRACE_G_ALL			0xFFFFFFFF
#define SOCMNTRACE_G_NOTHING		0x00000000
#define SOCMNTRACE_G_USER			0xF0000000
#define SOCMNTRACE_G_OPC			0x00000003

#define SOCMNTRACE_G_REF_COUNTS		0x04000008
#define SOCMNTRACE_G_IO				0x00000203
#define SOCMNTRACE_G_LOG_OBJECTS	0x0C000000

#define SOCMNTRACE_G_OPC_CALL	0x00000001
#define SOCMNTRACE_G_OPC_ADVISE	0x00000002
#define SOCMNTRACE_G_OBJ_REF	0x00000008
#define SOCMNTRACE_G_ENTRY		0x00000010
#define SOCMNTRACE_G_CLT_THREAD	0x00000100
#define SOCMNTRACE_G_SRV_THREAD	0x00000200
#define SOCMNTRACE_G_SUS_FILTER	0x00000400
#define SOCMNTRACE_G_OBJ_LOG	0x04000000
#define SOCMNTRACE_G_DISCREPANCY 0x08000000
#define SOCMNTRACE_G_USER1		0x10000000
#define SOCMNTRACE_G_USER2		0x20000000
#define SOCMNTRACE_G_USER3		0x40000000
#define SOCMNTRACE_G_USER4		0x80000000

// compatibility don't use this in new projects!
#define TG_USER1	    0x10000000		
#define TG_USER2	    0x20000000		
#define TG_USER3	    0x40000000		
#define TG_USER4	    0x80000000		
#define TG_USER		    0xF0000000


//-----------------------------------------------------------------------------
// Trace Levels                                                               |
//-----------------------------------------------------------------------------

#define SOCMNTRACE_L_ERR 0
#define SOCMNTRACE_L_WRN 1
#define SOCMNTRACE_L_INF 2
#define SOCMNTRACE_L_DEB 3

// compatibility don't use this in new projects!
#define TL_ERR	0
#define TL_WRN	1
#define TL_INF	2
#define TL_DEB	3


//-----------------------------------------------------------------------------
// SOCmnTraceHandler                                                          |
//-----------------------------------------------------------------------------

class SOCMN_EXPORT SOCmnTraceHandler : public SOCmnObject
{
public:
	SOCmnTraceHandler();
	virtual void onTrace(IN LPCTSTR traceString, IN USHORT level, IN DWORD mask, IN LPCTSTR objId, IN LPCTSTR text);
	virtual void getUserMaskString(IN DWORD mask, OUT LPTSTR string);
}; // SOCmnTraceHandler

#define GenericTraceHandler SOCmnTraceHandler



//-----------------------------------------------------------------------------
// SOCmnTrace                                                                 |
//-----------------------------------------------------------------------------

class SOCMN_EXPORT SOCmnTrace
{
public:	
	SOCmnTrace();
	~SOCmnTrace();

	DWORD m_traceLevel[4];		// trace levels
	SOCmnSync m_lock;				// trace output lock
	MMTIME m_timeStart, m_timeStop;
	BYTE m_startedPeriod;

	// do trace string
	void trace(IN USHORT level, IN DWORD mask, IN LPCTSTR objId, IN LPCTSTR text);

	// expand trance data
	LPCTSTR expand(IN LPCTSTR format, ...);

	// time measurement
	LPCTSTR elapsedTime(SOCmnString &elapsedTime, __int64 startTime, __int64 endTime);

	// set trace levels
	void setLevels(IN DWORD error, IN DWORD warning, IN DWORD info, 
			IN DWORD debug);

	// set trace file
	BOOL setFile(IN LPCTSTR file, OPTIONAL IN LPCTSTR secondFile = NULL, 
			OPTIONAL IN DWORD maxFileSize  = 1000000, 
			OPTIONAL IN DWORD creationDisposition = CREATE_ALWAYS);

	void setTraceHandler(IN SOCmnTraceHandler *traceHandler);
	SOCmnTraceHandler *getTraceHandler(void);

	SOCmnList<SOCmnObject> m_genObj;	// created generic object list
	SOCmnList<void> m_comObj;			// created COM object list

	SOCmnString m_variantBuffer;	// variant conversion buffer
	SOCmnString m_clipformatBuffer;	// clipformat conversion buffer
	SOCmnString m_vartypeBuffer;	// vartype conversion buffer
	SOCmnString m_qualityBuffer;	// quality conversion buffer
	SOCmnString m_filetimeBuffer;	// filetime conversion buffer

protected:
	HANDLE m_handleTrace;		// trace file handle
	HANDLE m_handleTrace2nd;	// second trace file handle
	HANDLE m_actHandle;			// current trace file handle
	DWORD m_fileSize;			// current trace file size
	TCHAR m_levelChar[4];		// trace level char
	TCHAR m_buffer[500];		// trace buffer
	TCHAR m_expand[410];		// expand buffer
	USHORT m_len;				// trace string lenght
	DWORD m_lines;				// lines in trace file
	TCHAR m_userMaskStr[4];		// user mask string
	LARGE_INTEGER m_pcFrequency;
	double m_pcPer1ms;
	
	SOCmnTraceHandler *m_trcHandler;	
								// trace handler

	// get string for trace mask
	LPCTSTR getMaskString(DWORD mask);

	void closeTraceFiles(void);
}; // SOCmnTrace

inline SOCmnTraceHandler *SOCmnTrace::getTraceHandler(void)
{ if (m_trcHandler) m_trcHandler->addRef(); return m_trcHandler; }

#define OPCTrace SOCmnTrace



//-----------------------------------------------------------------------------
// Global Functions                                                           |
//-----------------------------------------------------------------------------

// get pointer of the OPC trace object
SOCMN_EXPORT SOCmnTrace *getOPCTrace(void);
SOCMN_EXPORT SOCmnTrace *getSOCmnTrace(void);
void createTraceObject(void);
void destroyTraceObject(void);


// convert variant to string
SOCMN_EXPORT LPCTSTR variant2string(IN SOCmnString *buffer, IN LPCVARIANT var, OPTIONAL IN LCID localeID = LOCALE_SYSTEM_DEFAULT);

// convert variant to string and add vartype
SOCMN_EXPORT LPCTSTR variantWithType2string(IN SOCmnString *buffer, IN LPCVARIANT var, OPTIONAL IN LCID localeID = LOCALE_SYSTEM_DEFAULT);

// convert vartype to string
SOCMN_EXPORT LPCTSTR vartype2string(IN SOCmnString *buffer, IN VARTYPE vt);

// convert clipformat to string
SOCMN_EXPORT LPCTSTR clipformat2string(IN SOCmnString *buffer, IN CLIPFORMAT format);

// convert access rights to string
SOCMN_EXPORT LPCTSTR accessRights2string(IN SOCmnString *buffer, IN DWORD accessRight);

// convert quality to string
SOCMN_EXPORT LPCTSTR quality2string(IN SOCmnString *buffer, IN WORD quality);

// convert filetime to string
SOCMN_EXPORT LPCTSTR filetime2string(IN SOCmnString *buffer, IN LPFILETIME filetime);

// convert interface id into interface description string
SOCMN_EXPORT LPCTSTR IID2string(IN SOCmnString *buffer,	IN REFIID riid);



//-----------------------------------------------------------------------------
// Tracing Macros                                                             |
//-----------------------------------------------------------------------------


#define BOOL2STR(a) ((a) ? _T("TRUE") : _T("FALSE")) 
#define VARIANT2STR(a) (variantWithType2string(&::getSOCmnTrace()->m_variantBuffer, a)) 
#define CLIPFORMAT2STR(a) (clipformat2string(&::getSOCmnTrace()->m_clipformatBuffer, a))
#define QUALITY2STR(a) (quality2string(&::getSOCmnTrace()->m_qualityBuffer, a)) 
#define VARTYPE2STR(a) (vartype2string(&::getSOCmnTrace()->m_vartypeBuffer, a)) 
#define FILETIME2STR(a) (filetime2string(&::getSOCmnTrace()->m_filetimeBuffer, a)) 


#ifndef SO_FLAG_NO_TRACE

#define _TRACE_CREATE_GENOBJ()							\
{														\
	SOCmnTrace *trace = ::getSOCmnTrace();					\
	if (trace->m_traceLevel[TL_DEB] & SOCMNTRACE_G_OBJ_LOG)	\
	{													\
		trace->m_genObj.lock();							\
		trace->m_genObj.AddTail(this);		\
		trace->m_genObj.unlock();							\
	}													\
}

#define _TRACE_CREATE_COMOBJ()							\
{														\
	SOCmnTrace *trace = ::getSOCmnTrace();					\
	if (trace->m_traceLevel[TL_DEB] & SOCMNTRACE_G_OBJ_LOG)		\
	{													\
		trace->m_comObj.lock();							\
		trace->m_comObj.add((void *)this, this->getObjTraceId());		\
		trace->m_comObj.unlock();							\
	}													\
}

#define _TRACEC_CREATE_COMOBJ(id)							\
{														\
	SOCmnTrace *trace = ::getSOCmnTrace();					\
	if (trace->m_traceLevel[TL_DEB] & SOCMNTRACE_G_OBJ_LOG)		\
	{													\
		trace->m_comObj.lock();							\
		trace->m_comObj.add((void *)this, (LPCTSTR)id);		\
		trace->m_comObj.unlock();							\
	}													\
}

#define _TRACE_DELETE_GENOBJ()							\
{														\
	SOCmnTrace *trace = ::getSOCmnTrace();					\
	if (trace->m_traceLevel[TL_DEB] & SOCMNTRACE_G_OBJ_LOG)	\
	{													\
		trace->m_genObj.lock();							\
		trace->m_genObj.removeObject((SOCmnObject *)this);			\
		trace->m_genObj.unlock();							\
	}													\
}

#define _TRACE_DELETE_COMOBJ()							\
{														\
	SOCmnTrace *trace = ::getSOCmnTrace();					\
	if (trace->m_traceLevel[TL_DEB] & SOCMNTRACE_G_OBJ_LOG)		\
	{													\
		trace->m_comObj.lock();							\
		trace->m_comObj.removeObject((void *)this);			\
		trace->m_comObj.unlock();							\
	}													\
}

#define _TRACE_IF(level, mask) if (::getSOCmnTrace()->m_traceLevel[level] & mask)	

// trace with object id
#define _TRACE(level, mask, y)							\
{														\
	if (::getSOCmnTrace()->m_traceLevel[level] & mask)	\
		::getSOCmnTrace()->trace(level, mask, this->getObjTraceId(), ::getSOCmnTrace()->expand y); \
}														

// trace with object id and conversion
#define _TRACEW(level, mask, y)							\
{														\
	if (::getSOCmnTrace()->m_traceLevel[level] & mask)	\
	{	\
		USES_CONVERSION;								\
		::getSOCmnTrace()->trace(level, mask, this->getObjTraceId(), ::getSOCmnTrace()->expand y); \
	}	\
}														

// trace in interface with object id
#define _TRACEI(level, mask, y)							\
{														\
	if (::getSOCmnTrace()->m_traceLevel[level] & mask)	\
		::getSOCmnTrace()->trace(level, mask, this->getIntTraceId(), ::getSOCmnTrace()->expand y); \
}														

// trace with external object id
#define _TRACEO(level, mask, obj, y)							\
{														\
	if (::getSOCmnTrace()->m_traceLevel[level] & mask)	\
		::getSOCmnTrace()->trace(level, mask, obj->getObjTraceId(), ::getSOCmnTrace()->expand y);\
}														

// trace without object id
#define _TRACEC(level, mask, id, y)						\
{														\
	if (::getSOCmnTrace()->m_traceLevel[level] & mask)	\
		::getSOCmnTrace()->trace(level, mask, id, ::getSOCmnTrace()->expand y); \
}														

#define _TRACE_START_TIME(level, mask) QueryPerformanceCounter(&pcStart);
#define _TRACE_STOP_TIME(level, mask) QueryPerformanceCounter(&pcEnd);
#define _TRACE_ELAPSED_TIME (::getSOCmnTrace()->elapsedTime(szElapsedTime, pcStart.QuadPart, pcEnd.QuadPart))

#else

#define _TRACE_CREATE_GENOBJ()							
#define _TRACE_CREATE_COMOBJ()							
#define _TRACEC_CREATE_COMOBJ(id)							
#define _TRACE_DELETE_GENOBJ()							
#define _TRACE_DELETE_COMOBJ()							
#define _TRACE(level, mask, y) {}						
#define _TRACEW(level, mask, y)	{}					
#define _TRACEI(level, mask, y)	{}							
#define _TRACEO(level, mask, obj, y) {}							
#define _TRACEC(level, mask, id, y)	{}						
#define _TRACE_START_TIME(level, mask) pcStart.QuadPart = 0;
#define _TRACE_STOP_TIME(level, mask) pcEnd.QuadPart = 0;
#define _TRACE_ELAPSED_TIME _T("?");

#endif // SO_FLAG_NO_TRACE


#ifdef __BORLANDC__
#define _ASSERTION(expr, message) assert(expr)
#else

#ifndef SO_WINCE
#ifdef  _DEBUG
#define _ASSERTION(expr, message) \
        do { if (!(expr) && \
                (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, message))) \
             _CrtDbgBreak(); } while (0)
#else
#define _ASSERTION(expr, message)
#endif
#else
#define _ASSERTION(expr, message)
#endif

#endif

//-----------------------------------------------------------------------------
// Memory validation helpers                                                  |
//-----------------------------------------------------------------------------

inline BOOL isValidAddress(IN const void* lp, IN UINT nBytes, IN BOOL bReadWrite = FALSE)
{
	return (lp != NULL && !::IsBadReadPtr(lp, nBytes) &&
		(!bReadWrite || !::IsBadWritePtr((LPVOID)lp, nBytes)));
}

#define _IS_VALID_POINTER(p)		( isValidAddress(p, sizeof(*p)) )
#define _IS_VALID_WRITE_POINTER(p)	( isValidAddress(p, sizeof(*p), TRUE) )

#define _ASSERT_PTR(p)				_ASSERTION(_IS_VALID_POINTER(p), "Invalid pointer");
#define _ASSERT_WRITE_PTR(p)		_ASSERTION(_IS_VALID_WRITE_POINTER(p), "Invalid write Pointer");

#define _ASSERT_HANDLE(h)     _ASSERTION( h != INVALID_HANDLE_VALUE && h != NULL, "Invalid handle");
#define _COUNTOF(array) (sizeof(array)/sizeof(array[0]))

#pragma pack(pop)
#endif
