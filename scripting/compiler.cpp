//
// Created by wirewhiz on 14/10/22.
//

#include "compiler.h"
#include "../antlr4/braneLexer.h"
#include "antlr4/ANTLRInputStream.h"
#include "nativeTypes.h"
#include "aotNode/aotOperationNodes.h"
#include "aotNode/aotValueNodes.h"
#include "aotNode/aotFlowNodes.h"
#include "linker.h"
#include "library.h"
#include "structDefinition.h"

namespace BraneScript
{
#define RETURN_NULL return (AotNode*)nullptr
#define PROPAGATE_NULL(node) \
    if(!node) return (AotNode*)nullptr

    class LexerErrorListener : public antlr4::BaseErrorListener
    {

        Compiler& _compiler;
    public:
        LexerErrorListener(Compiler& compiler) : _compiler(compiler)
        {};

        void syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line,
                         size_t charPositionInLine,
                         const std::string& msg, std::exception_ptr e) override
        {
            if (offendingSymbol)
                _compiler.throwError(offendingSymbol, msg);
            else
            {
                size_t charIndex = 0;
                size_t lineIndex = 0;
                while (lineIndex < line - 1)
                {
                    if ((*_compiler._currentFile)[charIndex] == '\n')
                        ++lineIndex;
                    charIndex++;
                }
                _compiler.throwError(
                        (std::string)"Unknown Token \"" + (*_compiler._currentFile)[charIndex + charPositionInLine] +
                        "\" on line " + std::to_string(line) + ":" + std::to_string(charPositionInLine));
            }
        }
    };

    class ParserErrorListener : public antlr4::BaseErrorListener
    {
        Compiler& _compiler;
    public:
        ParserErrorListener(Compiler& compiler) : _compiler(compiler)
        {

        }

        void syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line,
                         size_t charPositionInLine,
                         const std::string& msg, std::exception_ptr e) override
        {
            if (offendingSymbol)
                _compiler.throwError(offendingSymbol, msg);
        }
    };

    IRScript* Compiler::compile(const std::string& script)
    {
        _currentFile = &script;
        antlr4::ANTLRInputStream input(script);

        LexerErrorListener lexErrListener(*this);
        braneLexer lexer(&input);
        lexer.removeErrorListeners();
        lexer.addErrorListener(&lexErrListener);
        antlr4::CommonTokenStream tokens(&lexer);

        ParserErrorListener parserErrListener(*this);
        braneParser parser(&tokens);
        parser.removeErrorListeners();
        parser.addErrorListener(&parserErrListener);

        _ctx = std::make_unique<CompilerCtx>(*this, new IRScript());
        visit(parser.program());

        if (contextValid())
            return _ctx->script;
        return nullptr;
    }

    std::any Compiler::visitProgram(braneParser::ProgramContext* context)
    {
        return braneBaseVisitor::visitProgram(context);
    }

    std::any Compiler::visitInclude(braneParser::IncludeContext* context)
    {
        return braneBaseVisitor::visitInclude(context);
    }

    std::any Compiler::visitConstString(braneParser::ConstStringContext* context)
    {
        return braneBaseVisitor::visitConstString(context);
    }

    std::any Compiler::visitInlineScope(braneParser::InlineScopeContext* context)
    {
        return visit(context->expression());
    }

    std::any Compiler::visitAssignment(braneParser::AssignmentContext* context)
    {
        auto* rValue = std::any_cast<AotNode*>(visit(context->expr));
        PROPAGATE_NULL(rValue);
        auto* lValue = std::any_cast<AotNode*>(visit(context->dest));
        PROPAGATE_NULL(lValue);

        return (AotNode*)new AotAssignNode(lValue, rValue);
    }

    std::any Compiler::visitScope(braneParser::ScopeContext* context)
    {
        pushScope();
        std::vector<AotNode*> operations;
        for (auto stmt: context->statement())
        {
            auto* operation = std::any_cast<AotNode*>(visit(stmt));
            PROPAGATE_NULL(operation);
            operations.push_back(operation);
        }
        popScope();
        return (AotNode*)new AotScope(std::move(operations));
    }

    std::any Compiler::visitConstFloat(braneParser::ConstFloatContext* context)
    {
        return (AotNode*)new AotConst(std::stof(context->FLOAT()->getText()), getType("float"));
    }

    std::any Compiler::visitAddsub(braneParser::AddsubContext* context)
    {
        auto left = std::any_cast<AotNode*>(visit(context->left));
        PROPAGATE_NULL(left);
        auto right = std::any_cast<AotNode*>(visit(context->right));
        PROPAGATE_NULL(right);
        AotNode* node = nullptr;
        if (context->op->getText() == "+")
            node = new AotAddNode(left, right);
        else
            node = new AotSubNode(left, right);
        return node;
    }

    std::any Compiler::visitMuldiv(braneParser::MuldivContext* context)
    {
        auto left = std::any_cast<AotNode*>(visit(context->left));
        PROPAGATE_NULL(left);
        auto right = std::any_cast<AotNode*>(visit(context->right));
        PROPAGATE_NULL(right);
        AotNode* node = nullptr;
        if (context->op->getText() == "*")
            node = new AotMulNode(left, right);
        else
            node = new AotDivNode(left, right);
        return node;
    }

    std::any Compiler::visitConstInt(braneParser::ConstIntContext* context)
    {
        return (AotNode*)new AotConst((int32_t)std::stoi(context->getText()), getType("int"));
    }

    std::any Compiler::visitId(braneParser::IdContext* context)
    {
        AotNode* node = getValueNode(context->getText());
        if (!node)
        {
            throwError(context->start, "Undefined identifier");
            return (AotNode*)nullptr;
        }
        return node;
    }

    std::any Compiler::visitMemberAccess(braneParser::MemberAccessContext* context)
    {
        auto* baseStructValue = getValueNode(context->base->getText());
        if(!baseStructValue)
        {
            throwError(context->base, "Identifier not found");
            RETURN_NULL;
        }

        auto structDef = dynamic_cast<StructDef*>(baseStructValue->resType());
        if(!structDef)
        {
            throwError(context->base, "Cant get member of void type");
            RETURN_NULL;
        }

        auto structMemberDef = structDef->getMember(context->member->getText());
        if(!structMemberDef)
        {
            throwError(context->member, "Member for struct " + context->base->getText() + " not found");
            RETURN_NULL;
        }


        return (AotNode*)new AotDerefNode(baseStructValue, structMemberDef->type, structMemberDef->offset);
    }

    std::any Compiler::visitDecl(braneParser::DeclContext* context)
    {
        return visit(context->declaration());
    }

    std::any Compiler::visitDeclaration(braneParser::DeclarationContext* context)
    {
        auto name = context->id->getText();
        auto type = getType(context->type->getText());
        if(!type)
        {
            throwError(context->type, "Undefined type");
            return (AotNode*)nullptr;
        }
        if(localValueExists(name))
        {
            throwError(context->id, "Identifier is already in use");
            return (AotNode*)nullptr;
        }
        if(context->isRef && type->type() != ObjectRef)
        {
            throwError(context->isRef, "Only types of struct can be marked as references");
            return (AotNode*)nullptr;
        }
        registerLocalValue(name, context->type->getText(), context->isConst, context->isRef);
        return getValueNode(name);
    }

    const std::vector<std::string>& Compiler::errors() const
    {
        return _errors;
    }

    std::any Compiler::visitArgumentList(braneParser::ArgumentListContext* ctx)
    {
        return braneBaseVisitor::visitArgumentList(ctx);
    }

    std::any Compiler::visitFunction(braneParser::FunctionContext* ctx)
    {
        ScriptFunction* previousFunction = _ctx->function;

        _ctx->script->localFunctions.push_back({});
        _ctx->setFunction(&*--_ctx->script->localFunctions.end());

        //TODO: TypeAssert()
        _ctx->function->returnType = ctx->type->getText();
        if (!getType(_ctx->function->returnType) && _ctx->function->returnType != "void")
        {
            throwError(ctx->type, "Unknown return type");
            return {};
        }

        _ctx->function->name = ctx->id->getText();
        _ctx->function->name += "(";
        pushScope();
        for (auto argument: ctx->arguments->declaration())
        {
            if(*--_ctx->function->name.end() != '(')
                _ctx->function->name += ',';
            std::string type = argument->type->getText();
            if (!getType(type))
            {
                throwError(argument->type, "Unknown argument type");
                return {};
            }
            _ctx->function->name += type;
            _ctx->function->arguments.push_back(type);

            AotValue value = _ctx->newReg(type, false);
            _ctx->lValues.insert({_lValueIDCount, value});
            registerLocalValue(argument->id->getText(), type, false);
        }
        _ctx->function->name += ")";

        bool previousReturnVal = _ctx->returnCalled;
        _ctx->returnCalled = false;
        for (auto* stmtCtx: ctx->statement())
        {
            auto stmt = std::any_cast<AotNode*>(visit(stmtCtx));
            if (!stmt)
                continue;

            //TODO optimize toggle
            auto expr = std::unique_ptr<AotNode>(stmt);

            AotNode* optimizedTree = expr->optimize();
            if (expr.get() != optimizedTree)
                expr = std::unique_ptr<AotNode>(optimizedTree);

            expr->generateBytecode(*_ctx);
        }
        if (!_ctx->returnCalled && _ctx->function->returnType != "void")
        {
            size_t begin = ctx->start->getStartIndex();
            size_t end = ctx->stop->getStopIndex() + 1;
            throwError(ctx->getStart()->getLine(), ctx->getStart()->getCharPositionInLine(),
                       "\n" + _currentFile->substr(begin, end - begin), "Function missing call to return");
            return {};
        }
        _ctx->returnCalled = previousReturnVal;
        popScope();

        _ctx->function = previousFunction;
        return {};
    }

    std::any Compiler::visitCast(braneParser::CastContext* ctx)
    {
        return (AotNode*)new AotCastNode(std::any_cast<AotNode*>(visit(ctx->expression())),
                                         getType(ctx->ID()->getText()));
    }

    std::any Compiler::visitReturnVoid(braneParser::ReturnVoidContext* ctx)
    {
        assert(false);
        throwError("Void return statements not implemented");
        _ctx->returnCalled = true;
        return (AotNode*)new AotReturnNode();
    }

    std::any Compiler::visitReturnVal(braneParser::ReturnValContext* ctx)
    {
        auto retVal = std::any_cast<AotNode*>(visit(ctx->expression()));
        PROPAGATE_NULL(retVal);
        if(!retVal->resType())
        {
            throwError(ctx->expression()->start, "can't cast from void to " + _ctx->function->returnType);
            RETURN_NULL;
        }
        if (retVal->resType()->name() != _ctx->function->returnType) //TODO check if a cast here is actually possible
            retVal = new AotCastNode(retVal, getType(_ctx->function->returnType));
        _ctx->returnCalled = true;
        return (AotNode*)new AotReturnValueNode(retVal);
    }

    uint16_t Compiler::registerLocalValue(std::string name, const std::string& type, bool constant, bool ref)
    {
        TypeDef* typeDef = getType(type);
        assert(typeDef);
        uint16_t index = _lValueIDCount++;
        _scopes.back().localValues.emplace(std::move(name), AotValueNode(index, typeDef, constant, ref));
        return index;
    }

    AotNode* Compiler::getValueNode(const std::string& name)
    {
        for (auto i = _scopes.rbegin(); i != _scopes.rend(); ++i)
        {
            auto v = i->localValues.find(name);
            if (v != i->localValues.end())
                return new AotValueNode(v->second);
        }
        return nullptr;
    }

    void Compiler::pushScope()
    {
        _scopes.push_back({});
    }

    void Compiler::popScope()
    {
        _scopes.pop_back();
    }

    std::any Compiler::visitConstBool(braneParser::ConstBoolContext* ctx)
    {
        if (ctx->getText() == "true")
            return (AotNode*)new AotConst(true, getType("bool"));
        else
            return (AotNode*)new AotConst(false, getType("bool"));
    }

    std::any Compiler::visitIf(braneParser::IfContext* ctx)
    {
        auto condition = std::any_cast<AotNode*>(visit(ctx->cond));
        auto operation = std::any_cast<AotNode*>(visit(ctx->operation));
        return (AotNode*)new AotIf(condition, operation);
    }

    std::any Compiler::visitWhile(braneParser::WhileContext* ctx)
    {
        auto condition = std::any_cast<AotNode*>(visit(ctx->cond));
        auto operation = std::any_cast<AotNode*>(visit(ctx->operation));
        return (AotNode*)new AotWhile(condition, operation);
    }

    std::any Compiler::visitComparison(braneParser::ComparisonContext* context)
    {
        auto* a = std::any_cast<AotNode*>(visit(context->left));
        auto* b = std::any_cast<AotNode*>(visit(context->right));

        AotCompareNode::Mode mode = AotCompareNode::Mode::Equal;
        std::string symbol = context->op->getText();
        if (symbol == "==")
            mode = AotCompareNode::Equal;
        else if (symbol == "!=")
            mode = AotCompareNode::NotEqual;
        else if (symbol == ">")
            mode = AotCompareNode::Greater;
        else if (symbol == ">=")
            mode = AotCompareNode::GreaterEqual;
        else if (symbol == "<")
        {
            std::swap(a, b);
            mode = AotCompareNode::Greater;
        } else if (symbol == "<=")
        {
            std::swap(a, b);
            mode = AotCompareNode::GreaterEqual;
        }


        return (AotNode*)new AotCompareNode(mode, a, b);
    }

    std::any Compiler::visitExprStatement(braneParser::ExprStatementContext* context)
    {
        return visit(context->expression());
    }

    std::any Compiler::visitArgumentPack(braneParser::ArgumentPackContext* context)
    {
        std::vector<AotNode*> arguments;
        for(auto& arg : context->expression())
            arguments.push_back(std::any_cast<AotNode*>(visit(arg)));
        return std::move(arguments);
    }

    std::any Compiler::visitFunctionCall(braneParser::FunctionCallContext* context)
    {
        std::string functionName = context->name->getText();
        std::vector<AotNode*> arguments = std::any_cast<std::vector<AotNode*>>(visit(context->argumentPack()));
        functionName += "(";
        for(auto* arg : arguments)
        {
            PROPAGATE_NULL(arg);
            TypeDef* argType = arg->resType();
            if(!argType)
            {
                throwError(context->getStart()->getLine(), context->getStart()->getChannel(), "", "Tried to pass void argument into function");
                RETURN_NULL;
            }
            if(*--functionName.end() != '(')
                functionName += ',';
            functionName += argType->name();
        }
        functionName += ")";

        if(context->namespace_)
        {
            std::string space = context->namespace_->getText();
            if(!_ctx->libraryAliases.count(space))
            {
                throwError(context->namespace_, "Library not found!");
                RETURN_NULL;
            }

            auto* library = _linker->getLibrary(_ctx->script->linkedLibraries[_ctx->libraryAliases[space]]);

            auto retType = getType(library->getFunctionReturnT(functionName));
            uint32_t libIndex = _ctx->libraryAliases.at(space);
            return (AotNode*)new AotExternalFunctionCall(libIndex, functionName, retType, arguments);
        }

        uint32_t fIndex = 0;
        for(auto& func : _ctx->script->localFunctions)
        {
            if(functionName == func.name)
            {
                TypeDef* retType = getType(func.returnType);
                return (AotNode*)new AotFunctionCall(fIndex, retType, arguments);
            }
            ++fIndex;
        }

        throwError(context->getStart(), "Could not find function with signature " + functionName);
        RETURN_NULL;
    }

    std::any Compiler::visitLink(braneParser::LinkContext* context)
    {
        std::string libraryName = removePars(context->library->getText());
        if(!_linker)
        {
            throwError("You must set a linker to be able to link libraries");
            return {};
        }
        if(!_linker->getLibrary(libraryName))
        {
            throwError("Library \"" + libraryName + "\" not found");
            return {};
        }
        _ctx->script->linkedLibraries.push_back(libraryName);
        if(context->alias)
            _ctx->libraryAliases.insert({removePars(context->alias->getText()), (uint32_t)_ctx->libraryAliases.size()});
        else
            _ctx->libraryAliases.insert({libraryName, (uint32_t)_ctx->libraryAliases.size()});
        return {};
    }

    std::any Compiler::visitNew(braneParser::NewContext* context)
    {
        auto* type = getType(context->type->getText());
        if(!type)
        {
            throwError(context->type, "Unknown type");
            RETURN_NULL;
        }
        if(type->type() != ObjectRef)
        {
            throwError(context->type, "Type is not an object");
        }
        return (AotNode*)new AotNewNode(dynamic_cast<StructDef*>(type));
    }

    std::any Compiler::visitDelete(braneParser::DeleteContext* context)
    {
        auto ptr = std::any_cast<AotNode*>(visit(context->ptr));
        if(ptr->resType()->type() != ObjectRef)
        {
            throwError(context->start, "Can only delete objects");
            RETURN_NULL;
        }
        return (AotNode*)new AotDeleteNode(ptr);
    }

    std::any Compiler::visitStructMembers(braneParser::StructMembersContext* context)
    {
        std::vector<StructMember> members;
        for(auto& decl : context->declaration())
        {
            StructMember m;
            auto type = getType(decl->type->getText());
            if(!type)
            {
                throwError(decl->type, "Could not create struct member with undefined type");
                continue;
            }
            m.name = decl->id->getText();
            m.type = type;
            members.push_back(std::move(m));
        }
        return members;
    }

    std::any Compiler::visitStructDef(braneParser::StructDefContext* context)
    {
        auto members = std::any_cast<std::vector<StructMember>>(visit(context->members));
        if(!contextValid())
            return {};
        auto newDef = new StructDef(context->id->getText());

        for(auto& m : members)
            newDef->addMember(std::move(m.name), m.type);
        if(context->packed)
            newDef->packMembers();
        else
            newDef->padMembers();
        _privateTypes.insert({newDef->name(), newDef});
        _ctx->localStructIndices.emplace(newDef, (uint16_t)_ctx->localStructDefs.size());
        _ctx->localStructDefs.push_back(std::unique_ptr<StructDef>(newDef));

        //We need to store a copy of this info in the ir script for the runtime
        IRScript::IRStructDef irDef;
        irDef.name = newDef->name();
        irDef.packed = context->packed;

        for(auto& m : newDef->members())
            irDef.members.push_back({m.name, m.offset, m.type->name()});
        _ctx->script->localStructs.push_back(irDef);
        return {};
    }

    void Compiler::throwError(const std::string& message)
    {
        _errors.push_back(message);
    }

    void Compiler::throwError(antlr4::Token* token, const std::string& message)
    {
        assert(token);
        throwError(token->getLine(), token->getCharPositionInLine(), token->getText(), message);
    }

    void Compiler::throwError(size_t line, size_t position, const std::string& context, const std::string& message)
    {
        std::string error =
                "Compile Error at [" + std::to_string(line) + ":" + std::to_string(position) + "] " + message;
        if (!context.empty())
            error += ": " + context;
        _errors.push_back(std::move(error));
    }

    bool Compiler::contextValid()
    {
        return _errors.empty();
    }

    TypeDef* Compiler::getType(const std::string& typeName) const
    {
        if(_privateTypes.count(typeName))
            return _privateTypes.at(typeName);
        return _linker->getType(typeName);
    }

    bool Compiler::localValueExists(const std::string& name)
    {
        for (auto& s: _scopes)
        {
            if (s.localValues.count(name))
                return true;
        }
        return false;
    }

    std::string Compiler::removePars(const std::string& str)
    {
        assert(str.size() >= 2);
        return str.substr(1, str.size() - 2);
    }

    Compiler::Compiler(Linker* linker)
    {
        _linker = linker;
    }

    CompilerCtx::CompilerCtx(Compiler& c, IRScript* s) : compiler(c), script(s)
    {

    }

    AotValue CompilerCtx::newReg(const std::string& type, uint8_t flags)
    {
        TypeDef* t = compiler.getType(type);
        return newReg(t, flags);
    }

    AotValue CompilerCtx::newReg(TypeDef* type, uint8_t flags)
    {
        AotValue value;
        value.def = type;
        value.flags = flags;
        if (!value.isVoid())
        {
            value.valueIndex.index = regIndex++;
            value.valueIndex.valueType = value.def->type();
            value.valueIndex.storageType = (type->type() != ValueType::ObjectRef) ? ValueStorageType_Reg : ValueStorageType_Ptr;
        }
        return value;
    }

    AotValue CompilerCtx::castValue(const AotValue& value)
    {
        if (value.isCompare())
            return castReg(value);
        switch(value.valueIndex.storageType)
        {

            case ValueStorageType_Null:
                assert(false);
                break;
            case ValueStorageType_Ptr:
            case ValueStorageType_StackPtr:
            case ValueStorageType_DerefPtr:
            case ValueStorageType_Reg:
            case ValueStorageType_Const:
                return value;
        }
        return value;
    }

    AotValue CompilerCtx::newConst(ValueType type, uint8_t flags)
    {
        AotValue value;
        value.def = getNativeTypeDef(type);
        assert(value.def);
        value.flags = flags;
        value.valueIndex.index = memIndex++;
        value.valueIndex.valueType = type;
        value.valueIndex.storageType = ValueStorageType_Const;
        return std::move(value);
    }

    AotValue CompilerCtx::castTemp(const AotValue& value)
    {
        assert(function);
        if (value.flags & AotValue::Temp)
            return value;
        AotValue tempValue = newReg(value.def, AotValue::Temp | (AotValue::Constexpr & value.flags));
        function->appendCode(MOV, tempValue.valueIndex, value.valueIndex);
        return tempValue;
    }

    AotValue CompilerCtx::castReg(const AotValue& value)
    {
        if (value.valueIndex.storageType == ValueStorageType_Reg || value.valueIndex.storageType == ValueStorageType_Ptr  || value.valueIndex.storageType == ValueStorageType_StackPtr)
            return value;
        AotValue regValue = newReg(value.def, AotValue::Temp | (AotValue::Constexpr & value.flags));
        if(!value.isCompare())
        {
            function->appendCode(MOV, regValue.valueIndex, value.valueIndex);
            return regValue;
        }
        assert(value.isCompare());
        switch (value.compareType)
        {
            case AotValue::EqualRes:
            {
                function->appendCode(SETE, regValue.valueIndex);
                return regValue;
            }
            case AotValue::NotEqualRes:
            {
                function->appendCode(SETNE, regValue.valueIndex);
                return regValue;
            }
            case AotValue::AboveRes:
            {
                function->appendCode(SETA, regValue.valueIndex);
                return regValue;
            }
            case AotValue::GreaterRes:
            {
                function->appendCode(SETG, regValue.valueIndex);
                return regValue;
            }
            case AotValue::AboveEqualRes:
            {
                function->appendCode(SETAE, regValue.valueIndex);
                return regValue;
            }
            case AotValue::GreaterEqualRes:
            {
                function->appendCode(SETGE, regValue.valueIndex);
                return regValue;
            }
            default:
                assert(false);
        }

    }

    uint32_t CompilerCtx::newMark()
    {
        return markIndex++;
    }

    void CompilerCtx::setFunction(ScriptFunction* f)
    {
        function = f;
        regIndex = 0;
        memIndex = 0;
        markIndex = 0;
        lValues.clear();
    }

}