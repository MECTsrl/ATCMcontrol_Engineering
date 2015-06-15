
#ifndef CLASSVIEWDESCRGENERATOR_H
#define CLASSVIEWDESCRGENERATOR_H

// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include <string>

#include "Global.h"

using namespace std;

#include "POU.h"
#include "IECSRCVisitor.h"

//================================================================================
class ClassViewDescrGenerator : public IECSRCVisitor
//================================================================================
{
public:
	BACKPARSERDLLEXPORT ClassViewDescrGenerator(const string& language,
                                                const string& completeFileName, 
                                                const string& fileName, 
                                                const string& fileSuffix="");
    BACKPARSERDLLEXPORT virtual ~ClassViewDescrGenerator();

    BACKPARSERDLLEXPORT void gen(const POU& pou, string& res);
    virtual void visit(const POU& pou)                const;
    virtual void visit(const Configuration& con)      const;
    virtual void visit(const SysComment& sc)          const;
    virtual void visit(const VarDecl& vd)             const;
    virtual void visit(const VarDeclSection& vds)     const;
    virtual void visit(const ImportDirective& impdir) const;
    virtual void visit(const TaskConfiguration& taskconf) const;
    virtual void visit(const ProgramConfiguration& taskconf,
                       int withTask = 0) const;

protected:

    set<string> m_filesyscmnt_attr_filter;
    
    string m_language;
    string m_complete_file_name;
    string m_file_name;
    string m_file_suffix;
    string m_create_date;
    string m_modification_date;

    mutable string* m_classview_description;
    mutable string m_syscmnt_buf;
    mutable string m_vardecl_buf;
    mutable string m_importdirective_buf;
    mutable string m_importdirective_configuration_buf;
    mutable string m_taskconfiguration_buf;
    mutable string m_programconfiguration_buf;

    mutable string m_var_type;
    mutable string m_var_modifier;

    void gen(const string& t1, const string& t2 = "") const;

    string createAttr(const string& key, const string& val) const;

    void addChildErrorNode(const string& errorText) const;

    mutable BOOL m_visitingConfiguration;

    string m_aliasAttributeName;

    string ValToXml(const string& val) const;
};

#endif

