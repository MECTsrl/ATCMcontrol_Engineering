
#ifndef IECSRCVISITOR_H
#define IECSRCVISITOR_H

class POU;
class Configuration;
class SysComment;
class VarDecl;
class VarDeclSection;
class ImportDirective;
class TaskConfiguration;
class ProgramConfiguration;

//================================================================================
class IECSRCVisitor  
//================================================================================
{
public:
	IECSRCVisitor();
	virtual ~IECSRCVisitor();

    virtual void visit(const POU& pou)                const = 0;
    virtual void visit(const Configuration& con)      const = 0;
    virtual void visit(const SysComment& sc)          const = 0;
    virtual void visit(const VarDecl& vd)             const = 0;
    virtual void visit(const VarDeclSection& vds)     const = 0;
    virtual void visit(const ImportDirective& impdir) const = 0;
    virtual void visit(const TaskConfiguration& taskconf) const = 0;
    virtual void visit(const ProgramConfiguration& taskconf,
                       int withTask = 0) const = 0;

};

#endif
