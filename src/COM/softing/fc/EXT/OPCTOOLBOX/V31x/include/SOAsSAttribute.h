//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOAsS                              |
//                                                                            |
//  Filename    : SOAsSAttribute.h                                            |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Event attribute related classes                             |
//                - SOAsSAttributeValue: Attribute value class with           |
//                                       connection to SODaSItemTag           |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOASSATTRIBUTE_H_
#define _SOASSATTRIBUTE_H_

#pragma pack(push,4)


//-----------------------------------------------------------------------------
// SOAsSAttributeValue                                                        |
//-----------------------------------------------------------------------------

class SOAES_EXPORT SOAsSAttributeValue : 
	public SOAeSAttributeValue
{
public:
	SOAsSAttributeValue(void);
	SOAsSAttributeValue(IN SOAeSAttribute *attribute);
	SOAsSAttributeValue(IN SOAeSAttribute *attribute, IN SODaSItemTag *itemTag);

	// establish connection for attribute value update
	virtual BOOL connectValueUpdate(IN SOAeSConditionGroup *condGrp);

	// item tag object
	virtual BOOL setItemTag(IN SODaSItemTag *tag);
	SODaSItemTag *getItemTag(void);

	BOOL updateValueFromCache(OPTIONAL OUT LPVARIANT newValue = NULL);

protected:
	SOCmnPointer<SODaSItemTag> m_itemTag;

	~SOAsSAttributeValue(void);
}; // SOAsSAttributeValue

inline SODaSItemTag *SOAsSAttributeValue::getItemTag(void)
{ SOCmnSingleLock<SOCmnSync> objectLock(SOCmnObjectLock::getObjectLock(this)); m_itemTag.addRef(); return m_itemTag; }

#pragma pack(pop)
#endif
