//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOAeS                              |
//                                                                            |
//  Filename    : SOAeSSubCondition.h                                         |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Subcondition related classes                                |
//                - SOAeSSubConditionClass: subcondition class -              |
//                                  The descrition of a subcondition          |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOAESSUBCONDITION_H_
#define _SOAESSUBCONDITION_H_

#pragma pack(push,4)


//-----------------------------------------------------------------------------
// SOAeSSubConditionClass                                                     |
//-----------------------------------------------------------------------------

class SOAES_EXPORT SOAeSSubConditionClass : 
	public SOCmnElement,
	virtual public SOCmnElementNameString
{
public:
	SOAeSSubConditionClass(void);
	SOAeSSubConditionClass(IN LPCTSTR name, IN DWORD severity, 
			IN LPCTSTR description, IN BOOL ackRequired, IN LPCTSTR definition);

	virtual BOOL isUniqueName(void) const;

	// severity
	virtual BOOL setSeverity(IN DWORD severity);
	DWORD getSeverity(void) const;

	// description
	virtual BOOL setDescription(IN LPCTSTR description);
	SOCmnString getDescription(void) const;

	// definition
	virtual BOOL setDefinition(IN LPCTSTR definition);
	SOCmnString getDefinition(void) const;

	// acknowledge requiered in activation event of this subcondition
	virtual BOOL setAckRequired(IN BOOL ackRequired);
	BOOL getAckRequired(void) const;

	// objects watch data
	virtual void getObjWatchData(IN void *pXMLDocumentInterface, IN void *pXMLParentNodeInterface, 
			IN BOOL withAttributes,	IN LPWSTR objNodeName, OPTIONAL OUT void **pXMLObjectNodeInterface = NULL);

protected:
	DWORD m_severity;
	SOCmnString m_description;
	SOCmnString m_definition;
	BYTE m_ackRequired;

	virtual ~SOAeSSubConditionClass(void);

	// callbacks
	virtual void onSetSeverity(IN DWORD severity);
	virtual void onSetDescription(IN LPCTSTR description);
	virtual void onSetDefinition(IN LPCTSTR definition);
	virtual void onSetAckRequired(IN BOOL ackRequired);

	virtual SOCmnObject *getObjectCmnElementIName(void);
}; // SOAeSSubConditionClass

inline DWORD SOAeSSubConditionClass::getSeverity(void) const
{ return m_severity; }

inline SOCmnString SOAeSSubConditionClass::getDescription(void) const
{ return m_description; }

inline SOCmnString SOAeSSubConditionClass::getDefinition(void) const
{ return m_definition; }

inline BOOL SOAeSSubConditionClass::getAckRequired(void) const
{ return (BOOL)m_ackRequired; }


#pragma pack(pop)
#endif
