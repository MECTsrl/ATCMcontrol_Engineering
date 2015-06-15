#ifndef _POUPROTECTDATA_H_
#define _POUPROTECTDATA_H_

//------------------------------------------------------------------*
/**
 *  class CPouProtectData
 *
 *  @see    CGHFile
*/
class CPouProtectData
{
public:
    CPouProtectData() {};
    virtual ~CPouProtectData() {};

public:
    CString m_strFileName;
    CString m_strFingerprint;
};

#endif

