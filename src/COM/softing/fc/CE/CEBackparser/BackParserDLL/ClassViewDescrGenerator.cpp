
#include "stdafx.h"

#include "ClassViewDescrGenerator.h"
#include "keyrc.h"
#include "Function.h"
#include  "DllInterface.h"

//------------------------------------------------------------------*
/**
 * Function name    ClassViewDescrGenerator::ClassViewDescrGenerator
 * Description      Constructor for class ClassViewDescrGenerator
 * @param           const string& completeFileName
 * @param           const string& fileName
 * @param           const string& fileSuffix
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
ClassViewDescrGenerator::ClassViewDescrGenerator(const string& language,
                                                 const string& completeFileName,
                                                 const string& fileName,
                                                 const string& fileSuffix)
: m_complete_file_name(completeFileName),
  m_file_name(fileName),
  m_file_suffix(fileSuffix),
  m_language(language)
//================================================================================
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    CString strDummy;
    strDummy.LoadString(IDS_KEY_ALIAS1);
    m_aliasAttributeName = strDummy;
    strDummy.MakeUpper();
    m_filesyscmnt_attr_filter.insert((const char*)strDummy);
    strDummy.LoadString(IDS_KEY_CREATED);
    strDummy.MakeUpper();
    m_filesyscmnt_attr_filter.insert((const char*)strDummy);
    strDummy.LoadString(IDS_KEY_GUID1);
    strDummy.MakeUpper();
    m_filesyscmnt_attr_filter.insert((const char*)strDummy);
    strDummy.LoadString(IDS_KEY_MODEFIED);
    strDummy.MakeUpper();
    m_filesyscmnt_attr_filter.insert((const char*)strDummy);
    strDummy.LoadString(IDS_KEY_USRCREATED);
    strDummy.MakeUpper();
    m_filesyscmnt_attr_filter.insert((const char*)strDummy);
    strDummy.LoadString(IDS_KEY_USRMODIFIED);
    strDummy.MakeUpper();
    m_filesyscmnt_attr_filter.insert((const char*)strDummy);
    strDummy.LoadString(IDS_KEY_URL);
    strDummy.MakeUpper();
    m_filesyscmnt_attr_filter.insert((const char*)strDummy);
    strDummy.LoadString(IDS_KEY_HELPURL);
    strDummy.MakeUpper();
    m_filesyscmnt_attr_filter.insert((const char*)strDummy);
    strDummy.LoadString(IDS_KEY_USERTEXT1);
    strDummy.MakeUpper();
    m_filesyscmnt_attr_filter.insert((const char*)strDummy);
    strDummy.LoadString(IDS_KEY_USERTEXT2);
    strDummy.MakeUpper();
    m_filesyscmnt_attr_filter.insert((const char*)strDummy);
    strDummy.LoadString(IDS_KEY_USERTEXT3);
    strDummy.MakeUpper();
    m_filesyscmnt_attr_filter.insert((const char*)strDummy);

    m_visitingConfiguration = FALSE;

    HRESULT hr = GetDates(m_complete_file_name,
                          m_create_date, 
                          m_modification_date);

    if (hr != S_OK)
    {
        m_create_date = "";
        m_modification_date = "";
    }
}

//------------------------------------------------------------------*
/**
 * Function name    ClassViewDescrGenerator::~ClassViewDescrGenerator
 * Description      Destructor for class ClassViewDescrGenerator
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
ClassViewDescrGenerator::~ClassViewDescrGenerator()
//================================================================================
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
}

//------------------------------------------------------------------*
/**
 * Function name    ClassViewDescrGenerator::gen
 * Description      generates the XML description for a pou
 * @param           const POU& pou
 * @param           string& res
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ClassViewDescrGenerator::gen(const POU& pou, string& res)
//================================================================================
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    m_classview_description = &res;

    pou.invite(*this);

    m_classview_description = 0;
}

//------------------------------------------------------------------*
/**
 * Function name    ClassViewDescrGenerator::gen
 * Description      appends t1 and t2 to the xml description
 * @param           const string& t1
 * @param           const string& t2 = ""
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ClassViewDescrGenerator::gen(const string& t1, const string& t2) const
//================================================================================
{
    *m_classview_description += t1;
    *m_classview_description += t2;
}

//------------------------------------------------------------------*
/**
 * Function name    ClassViewDescrGenerator::visit
 * Description      Generates the XML description for a configuration
 * @param           Configuration& con
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ClassViewDescrGenerator::visit(const Configuration& con) const
//================================================================================
{
    m_visitingConfiguration = TRUE;
    visit((POU&)con);
    m_visitingConfiguration = FALSE;
}

//------------------------------------------------------------------*
/**
 * Function name    ClassViewDescrGenerator::visit
 * Description      Generates the XML description for a pou
 * @param           POU& pou
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ClassViewDescrGenerator::visit(const POU& pou) const
//================================================================================
{
//    DebugOutput << "--> VISIT POU: " << endl;DebugOutput.flush();
    gen("<NODE");

    const string& type        = pou.getPOUType();
    string errorText          = pou.getErrorText();

    string concat_file_name = m_file_name + 
                              ( m_file_suffix.empty() ? "" : "." + m_file_suffix );

    gen(" ", createAttr("SOURCE",   concat_file_name));

    if (CString(type.c_str()).Compare("") == 0)
    {
        // write error node
        gen(" ", createAttr("ID",       concat_file_name));
        gen(" ", createAttr("TEXT",     concat_file_name));
        gen(" ", createAttr("TYPE",     _ERROR_NODE));
        gen("/>\n");
    }
    else
    {
        // write pou description
        string name;
        if(m_visitingConfiguration)
        {
            name = "Configuration";
        }
        else
        {
            name = pou.getName();
        }
        string text_suffix = "";
        if ((type.compare(_FUNCTION_BLOCK) == 0) ||
            (type.compare(_FUNCTION) == 0) ||
            (type.compare(_PROGRAM) == 0))
        {
            text_suffix = ( m_language.empty() ? "" : " [" + m_language + "]" );
        }

        gen(" ", createAttr("ID",       name));
        gen(" ", createAttr("TEXT",     name + text_suffix));
        gen(" ", createAttr("TYPE",     type));
        if (type.compare(_FUNCTION) == 0)
        {
            gen(" ", createAttr("DATA_TYPE", ((const Function&)pou).getFunctionType()));
        }

        gen(" ", createAttr("CREATED",     m_create_date));
        gen(" ", createAttr("MODIFIED",     m_modification_date));
        //gen(" ", createAttr("POSITION", pou.getPosition()));
        
        gen(" ", m_syscmnt_buf);

        gen(">\n");

        if (m_visitingConfiguration == TRUE)
        {
            gen(m_importdirective_configuration_buf);
        }
        else
        {
            gen(m_importdirective_buf);
        }

        gen(m_vardecl_buf);

        gen(m_taskconfiguration_buf);

        gen(m_programconfiguration_buf);

        if(CString(errorText.c_str()).Compare("") != 0)
        {
            addChildErrorNode(errorText);
        }

        gen("</NODE>");
    }


    m_importdirective_buf.empty();
    m_importdirective_configuration_buf.empty();
    m_vardecl_buf.empty();
    m_taskconfiguration_buf.empty();
    m_programconfiguration_buf.empty();
}

//------------------------------------------------------------------*
/**
 * Function name    ClassViewDescrGenerator::addChildErrorNode
 * Description      Generates the XML description for a error node as child
 * @param           string errorText
*/
//================================================================================
void ClassViewDescrGenerator::addChildErrorNode(const string& errorText) const
//================================================================================
{
    gen("<NODE");

    string concat_file_name = m_file_name + 
                              ( m_file_suffix.empty() ? "" : "." + m_file_suffix );

    gen(" ", createAttr("SOURCE",   concat_file_name));

    gen(" ", createAttr("ID",       concat_file_name));
    gen(" ", createAttr("TEXT",     errorText));
    gen(" ", createAttr("TYPE",     _ERROR_NODE));
    gen("/>\n");
}


//------------------------------------------------------------------*
/**
 * Function name    ClassViewDescrGenerator::visit
 * Description      Generates the XML description for a system comment
 * @param           SysComment& sc
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ClassViewDescrGenerator::visit(const SysComment& sc) const
//================================================================================
{
//    DebugOutput << "--> VISIT SYCMNT: " << endl;DebugOutput.flush();
    string separator = " ";
    m_syscmnt_buf = sc.toString(separator, m_filesyscmnt_attr_filter);
}

//------------------------------------------------------------------*
/**
 * Function name    ClassViewDescrGenerator::visit
 * Description      Generates the XML description for a variable declaration
 * @param           VarDecl& vd
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ClassViewDescrGenerator::visit(const VarDecl& vd) const
//================================================================================
{
//    DebugOutput << "--> VISIT VARDECL: " << endl;DebugOutput.flush();

    if ( m_var_type.empty() )
        return;

    string::size_type   capacity = m_vardecl_buf.capacity();
    string::size_type   size = m_vardecl_buf.size();

    if(capacity - size < 100)
    {
        m_vardecl_buf.reserve(100 + capacity * 2);
    }

    string type = vd.getType();
    string edge = vd.getEdge();
    string initializer = vd.getInitializer();
    string variablecomment = vd.getVarComment();

    vars_type vars = vd.getVariables();

    for( vars_type::const_iterator ci_v = vars.begin();
         ci_v != vars.end();
         ++ci_v )
    {
        string alias = "";
        if ((*ci_v)->syscomment != 0)
        {
            alias = (*ci_v)->syscomment->avm_get(m_aliasAttributeName);
        }

        m_vardecl_buf += "<NODE " ;
        m_vardecl_buf += createAttr("TEXT", (*ci_v)->name + " (" + type + ")");
        m_vardecl_buf += " ";
        m_vardecl_buf += createAttr("ID", (*ci_v)->name);
        m_vardecl_buf += " ";
        if(!alias.empty())
        {
            m_vardecl_buf += createAttr("ALIAS", alias);
            m_vardecl_buf += " ";
        }
        m_vardecl_buf += createAttr("TYPE", m_var_type) + " ";
        if(!m_var_modifier.empty())
        {
            m_vardecl_buf += createAttr("MODIFIER", m_var_modifier);
            m_vardecl_buf += " ";
        }
        if(!edge.empty())
        {
            m_vardecl_buf += createAttr("EDGE", edge);
            m_vardecl_buf += " ";
        }
        if(!initializer.empty())
        {
            m_vardecl_buf += createAttr("INIT_VALUE", initializer);
            m_vardecl_buf += " ";
        }
        if(!variablecomment.empty())
        {
            m_vardecl_buf += createAttr("COMMENT", variablecomment);
            m_vardecl_buf += " ";
        }
        if(!(*ci_v)->address.empty())
        {
            m_vardecl_buf += createAttr("ADDRESS", (*ci_v)->address);
            m_vardecl_buf += " ";
        }
        m_vardecl_buf += createAttr("DATA_TYPE", type);

        m_vardecl_buf += ">\n";

        m_vardecl_buf += "</NODE>\n";        
    }   
}

//------------------------------------------------------------------*
/**
 * Function name    ClassViewDescrGenerator::visit
 * Description      Generates the XML description for a variable declaration section
 * @param           VarDeclSection& vds
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ClassViewDescrGenerator::visit(const VarDeclSection& vds) const
//================================================================================
{
//    DebugOutput << "--> VISIT VARDECLSECT: " << endl;DebugOutput.flush();

    CString t;
    t = vds.getType().data();
    t.MakeUpper();
    if (t.Compare(_T(_STRUCT)) == 0)
    {
        t = _T(_STRUCT_VAR);
    }
    m_var_type = t;

    CString m;
    m = vds.getModifier().data();
    m.MakeUpper();
    m_var_modifier = m;
}

//------------------------------------------------------------------*
/**
 * Function name    ClassViewDescrGenerator::visit
 * Description      Generates the XML description for an import directive
 * @param           Importdirective& impdir
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ClassViewDescrGenerator::visit(const ImportDirective& impdir) const
//================================================================================
{
//    DebugOutput << "--> VISIT IMPORTDIRECTIVE: " << endl;DebugOutput.flush();

    string FileName = impdir.getFileName();

    m_importdirective_configuration_buf += "<LINK SOURCE=\"";
    m_importdirective_configuration_buf += FileName;    
    m_importdirective_configuration_buf += "\"/>\n";        

    m_importdirective_buf += "<NODE ";
    m_importdirective_buf += createAttr("OPENSOURCE", FileName) + " ";
    m_importdirective_buf += createAttr("TEXT", FileName) + " ";
    m_importdirective_buf += createAttr("ID", FileName) + " ";
    m_importdirective_buf += createAttr("TYPE", impdir.getType());
    m_importdirective_buf += ">";
    m_importdirective_buf += "</NODE>\n";        
}

//------------------------------------------------------------------*
/**
 * Function name    ClassViewDescrGenerator::visit
 * Description      Generates the XML description for a task configuration
 * @param           TaskConfiguration& taskconf
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ClassViewDescrGenerator::visit(const TaskConfiguration& taskconf) const
//================================================================================
{
//    DebugOutput << "--> VISIT TASKCONFIGURATION: " << endl;DebugOutput.flush();

    string TaskName = taskconf.getName();

    m_taskconfiguration_buf += "<NODE ";
    m_taskconfiguration_buf += createAttr("TEXT", TaskName) + " ";
    m_taskconfiguration_buf += createAttr("ID", TaskName) + " ";
    m_taskconfiguration_buf += createAttr("TYPE", "TASK");

    const SysComment* pSysComm = taskconf.GetFileSystemComment();
    if (pSysComm)
    {
        visit(*pSysComm);
        m_taskconfiguration_buf += " ";
        m_taskconfiguration_buf += m_syscmnt_buf;
    }

    m_taskconfiguration_buf += ">\n";

    m_taskconfiguration_buf += m_programconfiguration_buf;
    m_programconfiguration_buf = "";

    int num = taskconf.getNumTaskAttributes();
    for (int i=0; i<num; i++)
    {
        string strName;
        string strValue;
        taskconf.getTaskAttibute(i, strName, strValue);
        m_taskconfiguration_buf += "<ATTRIB ";
        m_taskconfiguration_buf += createAttr("NAME", strName) + " ";
        m_taskconfiguration_buf += createAttr("VALUE", strValue);
        m_taskconfiguration_buf += "/>\n";
    }

    m_taskconfiguration_buf += "</NODE>\n";        
}

//------------------------------------------------------------------*
/**
 * Function name    ClassViewDescrGenerator::visit
 * Description      Generates the XML description for a program configuration
 * @param           ProgramConfiguration& progconf
 * @param           int withTask
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ClassViewDescrGenerator::visit(const ProgramConfiguration& progconf,
                                    int withTask) const
//================================================================================
{
//    DebugOutput << "--> VISIT PROGRAMCONFIGURATION: " << endl;DebugOutput.flush();

    if ((withTask == 0) && (progconf.getTaskName().length() != 0)) 
    {
        // method is called for program configurations without tasks,
        // but progconf runs within a task
        return;
    }
    string Text;
    string ProgramName = progconf.getName();
    string ProgramClass = progconf.getProgramClass();
   
    Text = ProgramName + " [" + ProgramClass +"]";

    m_programconfiguration_buf += "<NODE ";
    m_programconfiguration_buf += createAttr("TEXT", Text) + " ";
    m_programconfiguration_buf += createAttr("ID", ProgramName) + " ";
    m_programconfiguration_buf += createAttr("TYPE", "PROGRAM_REF") + " ";
    m_programconfiguration_buf += createAttr("DATA_TYPE", ProgramClass);
        
    m_programconfiguration_buf += ">";
    m_programconfiguration_buf += "</NODE>\n";        
}

//------------------------------------------------------------------*
/**
 * Function name    ClassViewDescrGenerator::createAttr
 * Description      Generates the XML description for a key value tuple of the system comment
 * @param           const string& key
 * @param           const string& val
 * @return          string
 * @exception       -
 * @see             -
*/
//================================================================================
string ClassViewDescrGenerator::createAttr(const string& key,
                                           const string& val) const
//================================================================================
{    
    string xml = ValToXml(val);

    return key + "=\"" + xml + "\"";
}

//------------------------------------------------------------------*
/**
 * Function name    ClassViewDescrGenerator::ValToXml
 * Description      Generates the XML description for a value of the system comment
 * @param           const string& val
 * @return          string
 * @exception       -
 * @see             -
*/
//================================================================================
string ClassViewDescrGenerator::ValToXml(const string& val) const
//================================================================================
{
    string::size_type   len = val.length();

    string xml;
    xml.reserve(len + 20);

    for (int i = 0; i < len; i++)
    {
        char c = val[i];
        switch (c)
        {
            case '"': xml += "&quot;"; break;
            case '\'': xml += "&apos;"; break;
            case '&': xml += "&amp;"; break;
            case '<': xml += "&lt;"; break;
            case '>': xml += "&gt;"; break;
            case 'ß': xml += "&#223;"; break;
            case 'ä': xml += "&#228;"; break;
            case 'Ä': xml += "&#196;"; break;
            case 'ö': xml += "&#246;"; break;
            case 'Ö': xml += "&#214;"; break;
            case 'ü': xml += "&#252;"; break;
            case 'Ü': xml += "&#220;"; break;
            default : xml += c;
        }
    }
    return xml;
}


