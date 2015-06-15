
#ifndef PARSERCONTEXT_H
#define PARSERCONTEXT_H

#include "Global.h"

extern int end_of_last_comment;

class stream_scan;

class LexVal;
class POU;
class SysComment;
class EmptyFile;
class GlobVarConstList;
class Configuration;
class Resource;
class TaskConfiguration;
class ProgramConfiguration;
class Program;
class FunctionBlock;
class Function;
class StStruct;
class ImportDirective;
class VarDeclSection;
class VarDecl;

//================================================================================
class ParserContext
//================================================================================
{
public:
    ParserContext();
    virtual ~ParserContext();

    BACKPARSERDLLEXPORT void clear();

    BACKPARSERDLLEXPORT POU* GetPou() const;

    void cleanup(SysComment*& syscmnt);

    void assert(int line, const char* file);

    void        syscmnt_begin (LexVal* lexval);
    SysComment* syscmnt_end   (LexVal* lexval);
    void        syscmnt_add_av(LexVal* a, LexVal* v);
    SysComment* syscmnt_clear();

    void set_next_impdir_insert_pos(LexVal* lv);

    void emptyfile();

    void initializer_begin(LexVal* initializer);
    void initializer_clear();

    void globvarconstlist_begin();
    void globvarconstlist_end();
    void globvarconstlist_clear();
    void globvarconstlist_set_poutype(const string& POUType);

    void configuration_begin(LexVal* configuration, LexVal* name);
    void configuration_clear(LexVal* end);

    void resource_begin(LexVal* resource, LexVal* name);
    void resource_set_target(LexVal* target);
    void resoure_set_next_taskconf_insert_pos(LexVal* lv);
    void resoure_set_next_progconf_insert_pos(LexVal* lv);
    void resource_clear(/*LexVal* lv*/);

    void task_configuration_begin(LexVal* task_configuration, LexVal* name);
    void task_configuration_initialisation(LexVal* identifier, LexVal* becomes);
    void task_configuration_initialisation_list_range(LexVal* open, LexVal* close);
    void task_configuration_set_sys_comment(SysComment* comment);
    void task_configuration_clear(LexVal* end);

    void program_configuration_begin(LexVal* program_configuration, LexVal* name);
    void program_configuration_set_task(LexVal* task);
    void program_configuration_set_program_class(LexVal* progClass);
    void program_configuration_clear(LexVal* semicolon);
    void program_configuration_set_sys_comment(SysComment* comment);

    void program_begin(LexVal* program, LexVal* name);
    void program_clear();

    void fb_begin(LexVal* fb, LexVal* name);
    void fb_clear();

    void function_begin(LexVal* function, LexVal* name);
    void function_clear();
    void function_set_function_type(LexVal* type);

    void struct_begin(LexVal* st, LexVal* name);
    void struct_clear();

    void import_directive(LexVal* imp_dir);

    void vardeclsect_begin(LexVal* var, LexVal* modifier);
    void vardeclsect_end(LexVal* end);
    void vardecl_type(LexVal* type);
    void vardecl_edge(LexVal* edge);
    void vardecl_var_comment(LexVal* var_comment);
    void vardeclsect_clear();

    void vardecl_begin(LexVal* firstelem);
    void vardecl_end(LexVal* end);
    void vardecl_end_without_varcomment(LexVal* end);
    void vardecl_clear();

    void varlistelem_begin(LexVal* name);
    void varlistelem_end(LexVal* end);
    void varlistelem_address(LexVal* address);
    void varlistelem_syscomment(SysComment* comment);

    void SetFileSysComment();
    void SetFileSysComment(SysComment* p);

    void pou_clear();
    int  pou_error();

    void SuppressError();

    void extract_array_type(LexVal* start, LexVal* end);

    virtual stream_scan* getStreamScanner() = 0;

private:
    POU *m_pou;

    void init();

    SysComment           *m_syscmnt;
    GlobVarConstList     *m_globvarconstlist;
    Configuration        *m_configuration;
    Resource             *m_resource;
    TaskConfiguration    *m_taskconfiguration;
    ProgramConfiguration *m_programconfiguration;
    Program              *m_program;
    FunctionBlock        *m_fb;
    Function             *m_function;
    StStruct             *m_struct;
    ImportDirective      *m_importdirective;
    VarDeclSection       *m_vardeclsect;
    VarDecl              *m_vardecl;
    bool                 m_varlistopened;
    bool                 m_vardeclsect_found;
};  

#endif

