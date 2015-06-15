
#ifndef __IMPORTDIRECTIVE__
#define __IMPORTDIRECTIVE__

// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include <string>

#include "Global.h"

using namespace std;

#include "IECSRCVisitor.h"
#include "StringPosRange.h"

//================================================================================
class ImportDirective
//================================================================================
{
public:
    ImportDirective(const string& fileName, 
                    StringPosRange range,
                    const string& type);
    
    void setEndPos(StringPos end);

    virtual void invite(const IECSRCVisitor& visitor) const;

    BACKPARSERDLLEXPORT const string getFileName() const;
    BACKPARSERDLLEXPORT const StringPosRange& getImportDirectiveRange() const;

    void setType(const string& type);
    const string& getType() const;

protected:
    string m_fileName;
    StringPosRange m_range;
    string m_type;

private:
    void delChar(char c);
};

#endif
